/*
 *  physicsParticle.h
 *  SDLOpenGLIntro
 *
 *  Created by Sam Johnson on 10/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PHYSICSPARTICLE_H
#define _PHYSICSPARTICLE_H

#include "cinder/Vector.h"

using namespace ci;

class physicsParticle
{
    Vec2f x, v, renderPos;
	float t, h, accum;

	Vec2f lerp(const Vec2f &a, const Vec2f &b, const float t) const;
	void RK4();
	
	protected:
	physicsParticle(const Vec2f &x, float timestep) : x(x), t(0), accum(0), h(timestep) { }
	void step(float dt);
	virtual Vec2f a(const Vec2f &x, const Vec2f &v, const float t) = 0;

	public:
	const Vec2f getx() const { return renderPos; }
};

#endif