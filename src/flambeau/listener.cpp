/**
 *  listener.cpp
 *  
 *  Created on: Dec 10, 2012
 *      Author: nav
 *  
 */

#include "flambeau/listener.h"

//#include "flambeau/module.h"

namespace Flambeau
{

const unsigned int LineListener::FromLookup[Logic::Count] =
{
    FromL,
    FromH,
    FromZ,
    FromX
};

const unsigned int LineListener::ToLookup[Logic::Count] = 
{
    ToL,
    ToH,
    ToZ,
    ToX
};

LineListener::LineListener(Module * mod, void (Module::* method)(void), unsigned int mask)
    : Mod   (mod),
      Method(method),
      Mask  (mask)
{
    if(mask > LineListener::Any){
        throw GeneralException(__FILE__, __LINE__);
    }
}

LineListener::~LineListener()
{
    
}

void LineListener::Trigger(Logic from, Logic to) const
{
    unsigned int mask = LineListener::FromLookup[from.GetValue()] |
                        LineListener::ToLookup[to.GetValue()];
    
//    printf("%s:%d Trigger: mask = 0x%X, this->Mask = 0x%X\n", __FILE__, __LINE__, 
//           mask, this->Mask);
    
    if((this->Mask & mask) != 0){
//        printf("%s:%d Trigger\n", __FILE__, __LINE__);
        
        if(this->Mod != nullptr && this->Method != nullptr){
            (this->Mod->*this->Method)();
        }
    }
}

BusListener::BusListener(Module * mod, void (Module::* method)())
    : Mod   (mod),
      Method(method)
{
    
}

BusListener::~BusListener()
{
    
}

void BusListener::SetHandler(Module * mod, THandler method)
{
    this->Mod = mod;
    this->Method = method;
}

void BusListener::SetHandlerModule(Module * mod)
{
    this->Mod = mod;
}

void BusListener::SetHandlerMethod(BusListener::THandler method)
{
    this->Method = method;
}

void BusListener::Trigger() const
{
    if(this->Mod != nullptr && this->Method != nullptr){
        (this->Mod->*this->Method)();
    }
}

}   // namespace Flambeau
