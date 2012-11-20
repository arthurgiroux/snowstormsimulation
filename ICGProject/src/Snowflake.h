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
#include "../math/Vector3.h"





class Snowflake
{
public:
    
    float x;
    float y;
    float z;
    
    
    
    Vector3 acceleration;
    
    
    Snowflake(float x, float y , float z);
    Snowflake();
    
    
    void updatePosition(Vector3 force);
    
private:
    void randomInit();
    
    
    
    
    
};










#endif /* defined(__ICGProject__Snowflake__) */





