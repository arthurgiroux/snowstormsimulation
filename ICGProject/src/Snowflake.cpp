//
//  Snowflake.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Snowflake.h"
#define Y_REDUCTION 0.01;



Snowflake::Snowflake(float _x, float _y, float _z){
    
    x = _x;
    y = _y;
    z = _z;
    
}

Snowflake::Snowflake() {
    randomInit();
}

void Snowflake::randomInit() {
    x = 2 * (double)rand() / (double)RAND_MAX - 1;
    y = 1;
    z = 2 * (double)rand() / (double)RAND_MAX - 1;
}

void Snowflake::updatePosition(){
    
    //make the snow fall !
    y -= Y_REDUCTION;
    if (y <= -1) {
        randomInit();
    }
    // Between -0.002 and 0.002
    x += ((4 * ((double)rand() / (double)RAND_MAX)) - 2) / 1000.0;
}