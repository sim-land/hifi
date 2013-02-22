//
//  Hand.h
//  interface
//
//  Created by Philip Rosedale on 10/13/12.
//  Copyright (c) 2012 High Fidelity, Inc. All rights reserved.
//

#ifndef __interface__Hand__
#define __interface__Hand__

#include <glm/glm.hpp>
#include <iostream>
#include "Util.h"
#include "Field.h"
#include "world.h"
#include "InterfaceConfig.h"

class Hand {
public:
    Hand(glm::vec3 color);
    void simulate (float deltaTime);
    void render ();
    void reset ();
    void setNoise (float mag) { noise = mag; };
    void addVel (glm::vec3 v) { velocity += v; };
    glm::vec3 getPos() { return position; };
    void setPos(glm::vec3 p) { position = p; };
    void setTarget(glm::vec3 t) { target = t; };
private:
    glm::vec3 position, target, velocity, color, scale;
    float noise;
    
};


#endif
