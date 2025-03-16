/**
 *  module.cpp
 *  
 *  Created on: Nov 13, 2012
 *      Author: nav
 *  
 */

#include "flambeau/module.h"
#include "flambeau/signal.h"

namespace Flambeau
{

Module Module::ScratchPad("__scratch_pad");

Module::Module(const string & name)
    : Name            (name),
      SignalList      (),
      ChildModuleList ()
{
    
}

//Module::Module(Module & parent, const string & name)
//    : Parent(&parent),
//      Name  (name)
//{
//}

Module::~Module()
{
}

void Module::SetName(const string & name)
{
    this->Name.assign(name);
}

const string & Module::GetName(void) const
{
    return this->Name;
}

void Module::AddSignal(const SignalBase & sig)
{
    this->SignalList.push_back(&sig);
}

void Module::AddSignal(SignalBase & sig, const string & newName)
{
    sig.SetName(newName);
    
    this->SignalList.push_back(&sig);
}

void Module::AddChildModule(const Module & mod)
{
    this->ChildModuleList.push_back(&mod);
}

void Module::AddChildModule(Module & mod, const string & newName)
{
    mod.SetName(newName);
    
    this->ChildModuleList.push_back(&mod);
}

//----

TestBench::TestBench(const string & name)
    : Module    (name)
{
    
}

TestBench::~TestBench()
{
    
}

}   // namespace Flambeau
