//
//  Snowflake.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Snowflake.h"


Snowflake::Snowflake(Vector3 initialPos){
    cone = Cone::Cone(Vector3(0,0,0), 1, 2 , Vector3(0,1,0));
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

Vector3 Snowflake::computeAccelerationDueToCone(Cone cone){
    double openingAngle = atan((float)cone.radius / cone.height);
    //compute the vector between the origin of the cone (the pic) and the snowflake
    Vector3 originToFlake = pos - cone.pos;
    
    //compute angle between direction of cone and originToFlake vector
    double flakeAngle = atan2(cone.direction.cross(originToFlake).length(), cone.direction.dot(originToFlake));
    
    //if the angle is smaller than the opening angle,
    bool flakeIsInCone = (flakeAngle <= openingAngle);
    
    
    
    if(flakeIsInCone)
    {
        //fprintf(stderr, "flake \n");
        //give a test acceleration (normally, should compute the tangent direction, and give an acceleration relative to the distance from the axis
        Vector3 acceleration = Vector3(1000, 1000, 1000);
        return acceleration;
    }
    //if the flake is not in the cone, no special acceleration added
    
    return Vector3(0,0,0);
    
    
    
}

void Snowflake::updatePosition(Vector3 force, Vector3 min, Vector3 max){
    if (!hasBeenInit) {
        randomInit(min, max);
        hasBeenInit = true;
    }
    // Make the snow fall in world
    pos += velocity;
    pos += force;
    
    //@arthur : c'est comme çA qu'on fait? j'ai ajouté l'acceleration directement, comme j'ai vu que t'avais fait la même chose
   // pos += computeAccelerationDueToCone(cone);
    
    
    Vector3 acc = computeAccelerationDueToCone(cone);
    if(acc.x > 0 || acc.y >0 || acc.z > 0)
    {
        pos -= velocity;
        pos -= force;
    }
    
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
    /*if (pos.y < min.y || pos.y > max.y) {
        randomInit(min, max);
    }*/
    // Between -0.002 and 0.002
    pos.x += randomFloat(-0.002, 0.002);
}