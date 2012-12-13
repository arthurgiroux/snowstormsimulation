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


