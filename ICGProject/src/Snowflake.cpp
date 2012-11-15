//
//  Snowflake.cpp
//  ICGProject
//
//  Created by Valentin Richoz on 15.11.12.
//  Copyright (c) 2012 Arthur Giroux. All rights reserved.
//

#include "Snowflake.h"
#define Y_REDUCTION 0.5;



Snowflake::Snowflake(double _x, double _y, double _z){
    
    x = _x;
    y = _y;
    z = _z;
    
}

void Snowflake::updatePosition(){
    
    //make the snow fall !
    y -= Y_REDUCTION;
}