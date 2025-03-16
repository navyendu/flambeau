/**
 *  signal.cpp
 *  
 *  Created on: Nov 11, 2012
 *      Author: nav
 *  
 */

#include <stdio.h>
#include <stdint.h>

#include <algorithm>

#include "flambeau/signal.h"
#include "flambeau/listener.h"

//#include "flambeau/source.h"
//#include "flambeau/load.h"

namespace Flambeau
{

size_t SignalBase::IdCount = 0;

SignalBase::SignalBase(const string & name)
    : Id                (IdCount++),
      Name              (*new string(name)),
      BusListenerList   (*new vector<const BusListener*>)
{
    printf("Created SignalBase: Id = %lu, this = %lX\n", this->Id, (size_t) this);
}

//SignalBase::SignalBase(const string & name)
//    : Name  (&name),
//      Id    (&id)
//{
//    
//}
//
//SignalBase::SignalBase(const SignalBase & sig)
//    : Parent(sig.Parent),
//      Name  (sig.Name),
//      Id    (sig.Id)
//{
//    
//}
//
//SignalBase & SignalBase::operator = (const SignalBase & sig)
//{
//    this->Parent = sig.Parent;
//    this->Name   = sig.Name;
//    this->Id     = sig.Id;
//}

SignalBase::~SignalBase()
{
    printf("Destroyed SignalBase: Id = %lu, this = %lX\n", this->Id, (size_t) this);
    
    delete &this->BusListenerList;
    delete &this->Name;
}

void SignalBase::TriggerBusListeners(void) const
{
    for(unsigned int i = 0 ; i < this->BusListenerList.size() ; i++){
//        printf("%s:%d i = %u\n", __FILE__, __LINE__, i);
        
        this->BusListenerList.at(i)->Trigger();
    }
}

//---------------------------------------------------------------------------//

Node::Node()
    : Value             (Logic::X),
      LoadCap           (0),
      SourceValueList   (),
      LoadValueList     (),
      LineListenerList  (),
      ContainerList     ()
{
    printf("Created Node: this = %lX\n", (size_t) this);
}

Node::~Node()
{
    printf("Destroyed Node: this = %lX\n", (size_t) this);
}

bool Node::ResolveValue(bool fireListeners)
{
    Logic runningResolution = Logic::Z;
    
    for(unsigned int i = 0 ; i < this->SourceValueList.size() ; i++){
        runningResolution = runningResolution * *this->SourceValueList.at(i);
    }
    
    Logic oldValue = this->Value;
    
    this->Value = runningResolution;
    
    if(fireListeners && (oldValue != this->Value)){
        this->FireListeners(oldValue);
    }
    
    return oldValue != this->Value;
}

//Logic Node::UpdateValue(Logic newValue)
//{
//    Logic oldValue = this->Value;
//    
//    this->Value = newValue;
//    
////    for(unsigned int i = 0 ; i < this->LineListenerList.size() ; i++){
////        this->LineListenerList.at(i)->Trigger(oldValue, this->Value);
////    }
//    
//    // We don't fire the bus-listeners of the parent containers here. They're 
//    // instead fired from SimCtrl::Run
//    
////    for(unsigned int j = 0 ; j < this->ContainerList.size() ; j++){
//////        printf("%s:%d ContainerList[%u] = %lX\n", __FILE__, __LINE__, j, (size_t) ContainerList[j]);
////        
////        this->ContainerList[j]->TriggerBusListeners();
////    }
//    
////    printf("-- %s:%d old = %c, new = %c\n", __FILE__, __LINE__, 
////           oldValue.ToString(), this->Value.ToString());
//    
//    return oldValue;
//}

void Node::FireListeners(Logic oldValue) const
{
    for(unsigned int i = 0 ; i < this->LineListenerList.size() ; i++){
        this->LineListenerList.at(i)->Trigger(oldValue, this->Value);
    }
}

void Node::ResolveCap(void)
{
    uint64_t runningLoad = 0;
    
    for(unsigned int i = 0 ; i < this->LoadValueList.size() ; i++){
        runningLoad += *this->LoadValueList.at(i);
    }
    
    this->LoadCap = runningLoad;
}

void Node::AddSourceValue(const Logic * srcVal_p)
{
    this->SourceValueList.push_back(srcVal_p);
}

void Node::AddLoadValue(const uint64_t * loadVal_p)
{
    this->LoadValueList.push_back(loadVal_p);
}

void Node::AddListener(const LineListener & listener)
{
    this->LineListenerList.push_back(&listener);
}

void Node::AddContainer(const SignalBase & container)
{
    this->ContainerList.push_back(&container);
}

void Node::DelContainer(const SignalBase & container)
{
    this->ContainerList.erase(std::remove(this->ContainerList.begin(),
                                          this->ContainerList.end(),
                                          &container),
                              this->ContainerList.end());
}

//void Node::SetName(const string & name)
//{
//    this->Name.assign(name);
//}
//
//const string & Node::GetName(void) const
//{
//    return this->Name;
//}

}   // namespace Flambeau
