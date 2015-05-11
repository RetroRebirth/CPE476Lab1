#ifndef __Extra__
#define __Extra__

#include "includes.h"

class Extra {
public:
    // constructors & destructors
    Extra(Object* _object);
    virtual ~Extra();
    
    Object* object;
