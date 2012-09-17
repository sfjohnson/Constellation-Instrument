//
//  Agent.h
//  ArrowMap
//
//  Created by Sam Johnson on 13/09/12.
//
//

#ifndef __ArrowMap__Agent__
#define __ArrowMap__Agent__

#include <iostream>
#include "cinder/Vector.h"
#include "ArrowMap.h"
#include "OscSender.h"

using namespace ci;

class Agent
{
    static int agentCount;
    int agentId;
    const ArrowMap *map;
    unsigned int state;
    float strength;
    int decayLength; //How many steps it takes for the agent to go from full strength to zero strength
    int decayCounter;
    int steadyLength; //How many steps the agent is in the attractor for until is starts decaying
    int steadyCounter;
    float *decayTable;
    bool dropped;
    osc::Sender sender;
    osc::Message message;
    
public:
    Agent(const ArrowMap *arrowMap, const std::string &senderHost, const std::string &remoteHost, int senderPort, int remotePort);
    ~Agent();
    
    void step();
    static Vec2f lerp(const Vec2f &a, const Vec2f &b, const float t);
    void drop(unsigned int initialState, float inflectionAmt, int stepsInAttractor, int stepsUntilDeath);
};

#endif /* defined(__ArrowMap__Agent__) */
