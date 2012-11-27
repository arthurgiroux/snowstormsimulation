//
//  Snowflake.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Snowflake.h"


Snowflake::Snowflake(Vector3 initialPos){
    pos = initialPos;
}

Snowflake::Snowflake() {
    hasBeenInit = false;
}

float Snowflake::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Snowflake::randomInit(Vector3 min, Vector3 max) {
    if (!hasBeenInit) {
        pos = Vector3(randomFloat(min.x, max.x),
                  randomFloat(min.y, max.y),
                  randomFloat(min.z, max.z));
    }
    else {
        pos = Vector3(randomFloat(min.x, max.x),
                      max.y,
                      randomFloat(min.z, max.z));
    }
    velocity = Vector3(randomFloat(-0.005, 0.005), randomFloat(-0.005, 0), randomFloat(-0.005, 0.005));
}

void Snowflake::updatePosition(Vector3 force, Vector3 min, Vector3 max){
    if (!hasBeenInit) {
        randomInit(min, max);
        hasBeenInit = true;
    }
    // Make the snow fall in world
    pos += velocity;
    pos += force;
    if (pos.x < min.x) {
        pos.x = max.x;
    }
    if (pos.x > max.x) {
        pos.x = min.x;
    }
    
    if (pos.z < min.z) {
        pos.z = max.z;
    }
    if (pos.z > max.z) {
        pos.z = min.z;
    }
    //if (pos.y < min.y || pos.y > max.y || pos.x < min.x || pos.x > max.x || pos.z < min.z || pos.z > max.z) {
    if (pos.y < min.y || pos.y > max.y) {
        randomInit(min, max);
    }
    // Between -0.002 and 0.002
    pos.x += randomFloat(-0.002, 0.002);
}