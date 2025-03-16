/**
 *  module.h
 *  
 *  Created on: Nov 13, 2012
 *      Author: nav
 *  
 */

#ifndef MODULE_H_
#define MODULE_H_

#ifndef __cplusplus
#error Must use C++ for module.h
#endif

#include "flambeau/common.h"

namespace Flambeau
{

class SignalBase;

class Module
{
    friend class TestBench;
public:
    static Module       ScratchPad;
private:
    Module (const Module &);
    Module &    operator = (const Module &);
public:
//    Module *            Parent;
    string              Name;
    
    vector<const SignalBase*>   SignalList;
    vector<const Module*>       ChildModuleList;
protected:
    Module (const string & name = "");
//    Module (Module & parent, const string & name);
    ~Module();
public:
    void                SetName         (const string & name);
    const string &      GetName         (void) const;
    
    void                AddSignal       (const SignalBase & sig);
    void                AddSignal       (SignalBase & sig, const string & newName);
    void                AddChildModule  (const Module & mod);
    void                AddChildModule  (Module & mod, const string & newName);
    
    const vector<const SignalBase*> GetSignalList       (void) const;
    const vector<const Module*>     GetChildModuleList  (void) const;
};

inline const vector<const SignalBase*> Module::GetSignalList(void) const
{
    return this->SignalList;
}

inline const vector<const Module*> Module::GetChildModuleList(void) const
{
    return this->ChildModuleList;
}

class TestBench : public Module
{
protected:
    TestBench (const string & name);
    ~TestBench();
    
//    template <typename _tDerived>
//    void        AddInit         (void (_tDerived::* initMethod)(void));
//    template <typename _tDerived>
//    void        AddAlways       (void (_tDerived::* alwaysMethod)(void));
};

//template <typename _tDerived>
//void TestBench::AddInit(void (_tDerived::* initMethod)(void))
//{
//    SimCtrl::AddInit(*this, static_cast<void (TestBench::* )(void)>(initMethod));
//}
//
//template <typename _tDerived>
//void TestBench::AddAlways(void (_tDerived::* alwaysMethod)(void))
//{
//    SimCtrl::AddAlways(*this, static_cast<void (TestBench::* )(void)>(alwaysMethod));
//}
//
//inline const vector<void (TestBench::* )(void)> & TestBench::GetInitList(void) const
//{
//    return this->InitList;
//}
//
//inline const vector<void (TestBench::* )(void)> & TestBench::GetAlwaysList(void) const
//{
//    return this->AlwaysList;
//}

}   // namespace Flambeau

#endif /* MODULE_H_ */
