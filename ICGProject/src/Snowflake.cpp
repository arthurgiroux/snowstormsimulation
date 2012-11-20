//
//  Snowflake.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Snowflake.h"


Snowflake::Snowflake(float _x, float _y, float _z){
    
    x = _x;
    y = _y;
    z = _z;
    
}

Snowflake::Snowflake() {
    randomInit();
}

void Snowflake::randomInit() {
    x = 40 * (double)rand() / (double)RAND_MAX - 20;
    y = 20 * (double)rand() / (double)RAND_MAX;
    z = 40 * (double)rand() / (double)RAND_MAX - 20;
}

void Snowflake::updatePosition(Vector3 force){
    
    //make the snow fall !
    x += force.x;
    y += force.y;
    z += force.z;
    if (y <= 0) {
        randomInit();
    }
    // Between -0.002 and 0.002
    x += ((4 * ((double)rand() / (double)RAND_MAX)) - 2) / 1000.0;
}