//
//  Cone.h
//  ICGProject
//
//  Created by Valentin Richoz on 29.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#ifndef __ICGProject__Cone__
#define __ICGProject__Cone__

#include <iostream>
#include "../math/Vector3.h"


class Cone
{
public:
    
    Vector3 pos;
    Vector3 direction;
    
    Vector3 velocity;
    Vector3 acceleration;
    
    double radius;
    double height;
    
    float randomFloat(float a, float b);
    
    
    Cone(Vector3 initalPos, double radius, double height, Vector3 direction, Vector3 initialVelocity, Vector3 initialAcceleration);
    void updatePosition(Vector3 minPos, Vector3 maxPos, float deltaTime);
    Cone();
    ~Cone();
    
    
    
    
    
    
    
    
    
};












#endif /* defined(__ICGProject__Cone__) */
