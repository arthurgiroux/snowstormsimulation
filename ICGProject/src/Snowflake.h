//
//  Snowflake.h
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#ifndef __ICGProject__Snowflake__
#define __ICGProject__Snowflake__

#include <iostream>
#include <vector>
#include "../math/Vector3.h"
#include "../math/Matrix4.h"
#include "../src/Cone.h"




class Snowflake
{
public:
    
    Vector3 pos;
    
    Vector3 acceleration;
    Vector3 velocity;
    float size;
    int texture;
    
    
    Snowflake(Vector3 initialPos);
    Snowflake();
    ~Snowflake();
    
    
    void updatePosition(Vector3 force, Vector3 min, Vector3 max, std::vector<Cone*> storms);
    Vector3 computeAccelerationDueToCone(const Cone* cone);
    
    
private:
    bool hasBeenInit;
    void randomInit(Vector3 min, Vector3 max);
    float randomFloat(float a, float b);
    };










#endif /* defined(__ICGProject__Snowflake__) */





