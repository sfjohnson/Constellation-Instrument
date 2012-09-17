/*
 *  physicsParticle.cpp
 *  SDLOpenGLIntro
 *
 *  Created by Sam Johnson on 10/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "physicsParticle.h"

Vec2f physicsParticle::lerp(const Vec2f &a, const Vec2f &b, const float t) const
{
	return a + (b-a)*t;
}

void physicsParticle::RK4()
{
	//Code adapted from Doswa
	Vec2f a1, a2, a3, a4, x2, x3, x4, v2, v3, v4;
	
	a1 = this->a(x, v, t);
	
	x2 = x + v*0.5f*h;
	v2 = v + a1*0.5f*h;
	a2 = this->a(x2, v2, t+0.5f*h);
	
	x3 = x + v2*0.5f*h;
	v3 = v + a2*0.5f*h;
	a3 = this->a(x3, v3, t+0.5f*h);
	
	x4 = x + v3*h;
	v4 = v + a3*h;
	a4 = this->a(x4, v4, t+h);
	
	x += (v + (v2+v3)*2 + v4) * (h/6.0f);
	v += (a1 + (a2+a3)*2 + a4) * (h/6.0f);
	
	t += h;
}

void physicsParticle::step(float dt)
{
	Vec2f oldx;
	
	accum += dt;
	
	while (accum >= h)
	{
		oldx = x;
		RK4();
		accum -= h;
	}
	
	renderPos = lerp(oldx, x, accum / h);
}