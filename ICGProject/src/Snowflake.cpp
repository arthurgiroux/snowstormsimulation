//
//  Snowflake.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Snowflake.h"


Snowflake::Snowflake() {
    size = randomFloat(0.005, 0.01);
    hasBeenInit = false;
    texture = rand() % 16;
}

Snowflake::~Snowflake()
{
    
}

float Snowflake::randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void Snowflake::randomInit(Vector3 min, Vector3 max) {
    acceleration = Vector3(0.0, -0.00000981f, 0.0);
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
    velocity = Vector3(randomFloat(-0.0003, 0.0003), randomFloat(-0.0003, 0), randomFloat(-0.0003, 0.0003));
}

Vector3 Snowflake::computeAccelerationDueToCone(const Cone* cone){
    double openingAngle = atan((float)cone->radius / cone->height);
    //compute the vector between the origin of the cone (the pic) and the snowflake
    Vector3 originToFlake = pos - cone->pos;
    
    //compute angle between direction of cone and originToFlake vector
    double flakeAngle = atan2(cone->direction.cross(originToFlake).length(), cone->direction.dot(originToFlake));
    
    //if the angle is smaller than the opening angle,
    bool flakeIsInCone = (flakeAngle <= openingAngle && pos.y <= (cone->pos.y + cone->height));
    
    
    
    if (flakeIsInCone)
    {
        //fprintf(stderr, "flake \n");
        //give a test acceleration (normally, should compute the tangent direction, and give an acceleration relative to the distance from the axis
      //  Vector3 coneDirection = cone->direction;
       // Vector3 heightVector = cone->height * coneDirection.normalize();

        Vector3 flakeToAxis = Vector3(cone->pos.x, pos.y, cone->pos.z) - pos;
    
        Vector3 acc =  originToFlake.cross(flakeToAxis).normalize();
                
        return (acc + 0.3 * flakeToAxis.normalize()) * 0.00005 * (cone->radius/flakeToAxis.length());

    }
    
    //if the flake is not in the cone, no special acceleration added
    
    return Vector3(0,0,0);
    

}

void Snowflake::updatePosition(float deltaT, Vector3 min, Vector3 max, std::vector<Cone*> storms){
    if (!hasBeenInit) {
        randomInit(min, max);
        hasBeenInit = true;
        
    }

    for (unsigned int i = 0; i < storms.size(); ++i) {
        acceleration += computeAccelerationDueToCone(storms[i]);
    }
    
    pos += deltaT * velocity + 0.5 * acceleration * deltaT*deltaT;
    

    
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
    //pos.x += randomFloat(-0.002, 0.002);
    
}