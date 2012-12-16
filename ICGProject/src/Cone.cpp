//
//  Cone.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 29.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Cone.h"


Cone::Cone(Vector3 initialPos, double initialRadius, double initialHeight, Vector3 initialDirection, Vector3 initialVelocity, Vector3 initialAcceleration){
    pos = initialPos;
    radius = initialRadius;
    height = initialHeight;
    direction = initialDirection;
    velocity = initialVelocity;
    acceleration = initialAcceleration;
}

Cone::Cone(){
    
}

Cone::~Cone() {
    
}


float Cone::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}


void Cone::updatePosition(Vector3 minPos, Vector3 maxPos, float deltaTime) {
    pos += deltaTime * velocity + 0.5 * acceleration * deltaTime*deltaTime;
    if (pos.x > maxPos.x || pos.x < minPos.x ||
       pos.z > maxPos.z || pos.z < minPos.z)
    {
        pos = Vector3(randomFloat(minPos.x, maxPos.x), 0, randomFloat(minPos.z, maxPos.z));
    }
}


