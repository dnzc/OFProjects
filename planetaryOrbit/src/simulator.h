#pragma once

#include "ofMain.h"
#include "SpiceUsr.h"
#include <vector>
#include <initializer_list>
#include <utility>

struct Body {
    const char* name;
    SpiceDouble GM; // mass times gravitational constant
    SpiceDouble state[9]; // first 3 are xyz positions, next 3 are velocities, last 3 are accelerations
    SpiceDouble initialState[9]; // readonly

    // list of points containing positions at constant time interval
    ofPolyline realPath; 
    ofPolyline keplerPath;
    ofPolyline cowellPath;
};

class Simulator {

    public:
        Simulator(std::initializer_list<const char*> bodyNames, const char* observerName, const char* startTime, const char* endTime, float deltaTimeSeconds, int worldScale);

        void generateRealPaths();
        void generateKeplerPaths();
        void generateCowellPaths();
        
        // util functions
        std::vector<ofPolyline> getPaths(const char* type);
        int getScale();
        const char* getStartTime();
        const char* getEndTime();
        double getTimeRange(); // get the simulation time period
        double getDeltaTime();
        std::vector<std::pair<const char*, ofPoint>> getNamedBarycentersAtTime(double time, const char* type);

    private:
        const char* m_observerName;
        SpiceDouble m_observerGM;
        SpiceDouble m_deltaTime;
        SpiceDouble m_startTime, m_endTime;
        const char* m_startTimeStr;
        const char* m_endTimeStr;
        std::vector<Body> m_bodies;
        int m_worldScale;
    
};
