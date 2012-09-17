//
//  Agent.cpp
//  ArrowMap
//
//  Created by Sam Johnson on 13/09/12.
//
//

#include <sstream>
#include "Agent.h"

int Agent::agentCount = 0;

Agent::Agent(const ArrowMap *arrowMap, const std::string &senderHost, const std::string &remoteHost, int senderPort, int remotePort)
{
    map = arrowMap;
    dropped = false;
    agentId = agentCount;
    agentCount++;
    decayTable = NULL;
    
    sender.setup(senderHost, senderPort);
	message.setRemoteEndpoint(remoteHost, remotePort);
}

Agent::~Agent()
{
    if (decayTable != NULL)
        delete [] decayTable;
}

void Agent::step()
{
    if (dropped)
    {
        std::stringstream ss;

        if (steadyCounter == 0 && map->isAttractor(state))
            steadyCounter++;
        else if (steadyCounter > 0)
        {
            if (steadyCounter == steadyLength)
                decayCounter++;
            else
                steadyCounter++;
        }
        
        message.clear();
        message.addFloatArg(strength);
        message.addIntArg(map->networkSize());
        for (int i = map->networkSize() - 1; i >= 0; i--)
            message.addIntArg(state >> i & 1);
    
        ss << "/Cinder/rbn/agent/" << agentId;
        message.setAddress(ss.str());
        sender.sendMessage(message);
    
        state = map->getNextState(state);
    }
}

Vec2f Agent::lerp(const Vec2f &a, const Vec2f &b, const float t)
{
	return a + (b-a)*t;
}

//The agent proceeds as follows:
//1. Gets dropped into a transient area or an attractor
//2. Travels an unlimited number of steps until it is in an attractor (0 - infinity steps)
//3. Travels stepsInAttractor steps
//4. Travels stepsUntilDeath steps while decaying
void Agent::drop(unsigned int initialState, float inflectionAmt, int stepsInAttractor, int stepsUntilDeath)
{
    dropped = true;
    state = initialState;
    decayLength = stepsUntilDeath;
    decayCounter = 0;
    steadyCounter = 0;
    steadyLength = stepsInAttractor;
    strength = 1.0f;
    
    if (decayTable != NULL)
        delete [] decayTable;
    
    //DEBUG: the Bezier code is broken, inflectionAmt doesn't do anything.
    Vec2f a0(0.0f, 1.0f);
    Vec2f a1(inflectionAmt, 1.0f);
    Vec2f a2(1.0f - inflectionAmt, 0.0f);
    Vec2f a3(1.0f, 0.0f);
    Vec2f t0, t1, t2;
    float t = 0.0f;
    
    decayTable = new float[decayLength];
    for (int i = 0; i < decayLength; i++, t = (float)i / (decayLength-1))
    {
        t0 = lerp(a0, a1, t);
        t1 = lerp(a1, a2, t);
        t2 = lerp(a2, a3, t);
        t0 = lerp(t0, t1, t);
        t1 = lerp(t1, t2, t);
        decayTable[i] = lerp(t0, t1, t).y;
    }
}