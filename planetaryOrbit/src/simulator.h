#pragma once

#include <vector>
#include "ofMain.h"
#include "SpiceUsr.h"
#include <initializer_list>

struct Body {
    const char* name;
    SpiceDouble GM; // mass times gravitational constant
    SpiceDouble state[9]; // first 3 are xyz pos, next 3 are vel, last 3 are accel
    ofPolyline realPath;
    ofPolyline cowellPath;
};

class Simulator {

    public:
        Simulator(std::initializer_list<const char*> bodyNames, const char* observerName, const char* startTime, const char* endTime, float deltaTimeSeconds, int worldScale);
        void generateCowellPaths();
        void generateRealPaths();
        std::vector<ofPolyline> getPaths(const char* type);
        int getScale();

    private:
        const char* m_observerName;
        SpiceDouble m_observerGM;
        SpiceDouble m_deltaTime;
        SpiceDouble m_startTime, m_endTime;
        std::vector<Body> m_bodies;
        int m_worldScale;
    
};
