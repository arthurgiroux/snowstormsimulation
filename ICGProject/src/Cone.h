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
    double radius;
    double height;
    
    
    Cone(Vector3 initalPos, double radius, double height, Vector3 direction);
    Cone();
    
    
    
    
    
    
    
    
};












#endif /* defined(__ICGProject__Cone__) */
