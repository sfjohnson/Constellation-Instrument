//
//  MapParticle.h
//  ArrowMap
//
//  Created by Sam Johnson on 11/09/12.
//
//

#ifndef __ArrowMap__MapParticle__
#define __ArrowMap__MapParticle__

#include "physicsParticle.h"
#include "cinder/Timer.h"
#include "cinder/Rand.h"

class MapParticle : public physicsParticle
{
    float k, c, vAvg;
    MapParticle *link;
    bool mouse, sleep;
    Vec2f mousePos;
    ci::Timer timer;
    float interval, disturbInterval, disturbMag;
    bool timerOn, mouseRipple, disturbOn;
    
public:
    MapParticle(const Vec2f &initial, float stiffness, float damping, float timestep) : physicsParticle(initial, timestep), k(stiffness), c(damping), mouse(false), sleep(false), timerOn(false), timer(true), interval(0.0f), mouseRipple(false), disturbOn(false)
    {
    }
    
    virtual Vec2f a(const Vec2f &x, const Vec2f &v, const float t)
    {
        Vec2f a, edge;
        
        edge = x - link->getx();
        a = -k*(edge-40.0f*edge.normalized()) - c*v;
        
        if (mouse)
            a += 200.0f * (mousePos-x);
        else if (disturbOn)
        {
            a += disturbMag * Vec2f(-edge.y, edge.x);
            if (timer.getSeconds() - disturbInterval >= 0.5f)
                disturbOn = false;
        }
        
        if (sleep)
            return a;
        
        if (!timerOn && v.lengthSquared() < 0.03f)
        {
            timerOn = true;
            interval = timer.getSeconds();
        }
        else if (timerOn && v.lengthSquared() >= 0.03f)
        {
            timerOn = false;
        }
        
        if (timer.getSeconds() - interval >= 5.0f && timerOn)
        {
            interval = timer.getSeconds();
            timerOn = false;
            //sleep = true;
            mouseRipple = false;
        }
        
        return a;
    }
    
    void connect(MapParticle *other)
    {
        link = other;
    }
    
    void mouseGrab()
    {
        wake();
        mouse = true;
        mouseRipple = true;
    }
    
    void mouseDrag(const Vec2f &pos)
    {
        mousePos = pos;
    }
    
    void mouseRelease()
    {
        mouse = false;
    }
    
    bool isMouseOn() const
    {
        return mouse;
    }
    
    bool isMouseActivated() const
    {
        return mouseRipple;
    }
    
    bool isAsleep() const
    {
        return sleep;
    }
    
    void wake()
    {
        sleep = false;
    }
    
    void disturb()
    {
        disturbInterval = timer.getSeconds();
        disturbMag = (Rand::randBool() ? 1.0f : -1.0f) * Rand::randFloat(10.0f, 50.0f);
        disturbOn = true;
    }
    
    void update(float dt)
    {
        if (isAsleep())
        {
            if (!link->isAsleep() && link->isMouseActivated())
            {
                wake();
                mouseRipple = true;
            }
        }
        else
            step(dt);
        
    }
};

#endif /* defined(__ArrowMap__MapParticle__) */
