#include "simulator.h"
#include "SpiceUsr.h"
#include "ofMain.h"
#include <vector>
#include <initializer_list>
#include <utility>
#include <iostream>
#include <cmath>

Simulator::Simulator(std::initializer_list<const char*> bodyNames, const char* observerName, const char* startTime, const char* endTime, float deltaTimeSeconds, int worldScale) : m_observerName(observerName), m_deltaTime(deltaTimeSeconds), m_worldScale(worldScale), m_startTimeStr(startTime), m_endTimeStr(endTime) {

    // load the kernels (when run by make, the executable is run inside planetaryOrbit/bin)
    furnsh_c("../src/kernels/metaKernel.txt");

    // load start+end times
    str2et_c(startTime, &m_startTime); // SPICE API
    str2et_c(endTime, &m_endTime); // SPICE API

    // temporary storage containers needed by spice
    SpiceDouble lightTime;
    SpiceDouble state[6];
    SpiceInt dim;
    SpiceDouble values[1];

    // load observer GM (mass times gravitational constant)
    bodvrd_c(m_observerName, "GM", 1, &dim, values); // SPICE API
    m_observerGM = values[0];

    // create bodies
    for(const char* name : bodyNames) {
        Body b = Body();
        b.name = name;

        // load GM data
        bodvrd_c(name, "GM", 1, &dim, values); // SPICE API
        b.GM = values[0];

        // load pos+vel (state) data
        // use the J2000 coordinate system, do not perform aberration corrections
        spkezr_c(name, m_startTime, "J2000", "NONE", m_observerName, state, &lightTime); // SPICE API
        for(int i=0; i<6; ++i) {
            b.initialState[i] = state[i];
            b.state[i] = state[i];
        }

        m_bodies.push_back(b);
    }

}

std::vector<ofPolyline> Simulator::getPaths(const char* type) {
    std::vector<ofPolyline> paths;
    for(Body b : m_bodies) {
        if(type == "kepler")
            paths.push_back(b.keplerPath);
        else if(type == "cowell")
            paths.push_back(b.cowellPath);
        else
            paths.push_back(b.realPath);
    }
    return paths;
}

std::vector<std::pair<const char*, ofPoint>> Simulator::getNamedBarycentersAtTime(double timeAfterStart, const char* type) {
    std::vector<std::pair<const char*, ofPoint>> out;
    if(timeAfterStart < 0 || timeAfterStart > m_endTime - m_startTime) return out; // check that wanted time is valid
    double fractionElapsed = timeAfterStart / (m_endTime - m_startTime); // current fraction of simulation completed
    for(Body b : m_bodies) {
        if(type == "kepler")
            out.push_back(std::make_pair(b.name, b.keplerPath.getPointAtPercent(fractionElapsed)));
        else if(type == "cowell")
            out.push_back(std::make_pair(b.name, b.cowellPath.getPointAtPercent(fractionElapsed)));
        else
            out.push_back(std::make_pair(b.name, b.realPath.getPointAtPercent(fractionElapsed)));
    }
    return out;
}

void Simulator::generateRealPaths() {
    SpiceDouble pos[3];
    SpiceDouble lightTime; // output from spkpos but it will not be used
    for(auto it = std::begin(m_bodies); it != std::end(m_bodies); ++it) {
        // load position datapoints
        for(double time=m_startTime; time<m_endTime; time+=m_deltaTime) {
            // use the J2000 coordinate system, do not perform aberration corrections
            spkpos_c(it->name, time, "J2000", "NONE", m_observerName, pos, &lightTime); // SPICE API

            float xPos = pos[0] / pow(10, m_worldScale);
            float yPos = pos[1] / pow(10, m_worldScale);
            float zPos = pos[2] / pow(10, m_worldScale);
            // add datapoint to path
            it->realPath.addVertex(glm::vec3(xPos, yPos, zPos));

        }
    }
}

void Simulator::generateKeplerPaths() {

    // see SPICE documentation for oscelt_c
    // elts[0] is perifocal distance
    // elts[1] is eccentricity
    // elts[5] is mean anomaly at epoch
    // elts[6] is epoch
    SpiceDouble elts[8]; 

    for(auto it = std::begin(m_bodies); it != std::end(m_bodies); ++it) {
        // get orbital elements from initial state
        oscelt_c(it->initialState, m_startTime, m_observerGM, elts); // SPICE API

        double a = elts[0] / (1 - elts[1]); // semi major axis: perifocal dist = smajor * (1 - eccentricity)
        double T = 2*M_PI*sqrt(a*a*a / m_observerGM); // orbital period: Kepler's Third Law

        // follow orbit ellipse, computing position at current time
        for(double time=m_startTime; time<m_endTime; time+=m_deltaTime) {
            SpiceDouble curElts[8];
            for(int i=0; i<8; ++i) curElts[i] = elts[i];

            double meanAnomaly = 2*M_PI*(time - m_startTime)/T + elts[5];
            curElts[5] = meanAnomaly;
            curElts[6] = time;

            // determine state from elements
            SpiceDouble curState[6]; // xyz pos, xyz vel
            conics_c(curElts, time, curState); // SPICE API

            float xPos = curState[0] / pow(10, m_worldScale);
            float yPos = curState[1] / pow(10, m_worldScale);
            float zPos = curState[2] / pow(10, m_worldScale);
            // add datapoint to path
            it->keplerPath.addVertex(glm::vec3(xPos, yPos, zPos));
        }
    }
}

void Simulator::generateCowellPaths() {

    for(float time=m_startTime; time<m_endTime; time+=m_deltaTime) {

        // calculate accelerations caused by observer
        for(auto it = std::begin(m_bodies); it != std::end(m_bodies); ++it) {

            // calculate distance to observer, using Pythagoras
            float dist = 0;
            for(int i=0; i<3; ++i) {
                dist += pow(it->state[i], 2);
            }
            dist = sqrt(dist);

            for(int i=0; i<3; ++i) {
                it->state[i+6] = - m_observerGM * it->state[i] / pow(dist, 3);
            }
        }

        // factor in net perturbation accelerations by summing interactions between every pair of bodies
        for(auto it1 = std::begin(m_bodies)+1; it1 != std::end(m_bodies); ++it1) {
            for(auto it2 = std::begin(m_bodies); it2 != it1; ++it2) {

                // calculate distance between bodies, using Pythagoras
                float dist = 0;
                for(int i=0; i<3; ++i) {
                    dist += pow(it1->state[i] - it2->state[i], 2);
                }
                dist = sqrt(dist);

                // handle x,y,z separately
                for(int i=0; i<3; ++i) {
                    it1->state[i+6] += it2->GM * (it2->state[i] - it1->state[i]) / pow(dist, 3);
                    it2->state[i+6] += it1->GM * (it1->state[i] - it2->state[i]) / pow(dist, 3);
                }    

            }
        }

        // apply accelerations
        for(auto it = std::begin(m_bodies); it != std::end(m_bodies); ++it) {
            for(int i=0; i<3; ++i) {
                it->state[i+3] += it->state[i+6]*m_deltaTime;
                it->state[i] += it->state[i+3]*m_deltaTime;
            }

            float worldX = it->state[0] / pow(10, m_worldScale);
            float worldY = it->state[1] / pow(10, m_worldScale);
            float worldZ = it->state[2] / pow(10, m_worldScale);
            it->cowellPath.addVertex(glm::vec3(worldX, worldY, worldZ));
            
        }
    }
}

int Simulator::getScale() {
    return m_worldScale;
}

const char* Simulator::getStartTime() {
    return m_startTimeStr;
}

const char* Simulator::getEndTime() {
    return m_endTimeStr;
}

double Simulator::getTimeRange() {
    return m_endTime - m_startTime;
}

double Simulator::getDeltaTime() {
    return m_deltaTime;
}
