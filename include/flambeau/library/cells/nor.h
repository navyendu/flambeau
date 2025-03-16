/**
 *  nor.h
 *  
 *  Created on: Jan 20, 2013
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_NOR_H_
#define LIBRARY_CELLS_NOR_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/nor.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <unsigned int InCount = 2, Technology Tech = Technology::Virtual, TechParam Param = 0>
class Nor : public Module
{
    static_assert(InCount >= 2, "Nor gate must have atleast 2 input lines");
    static_assert(Param == 0, "Param must be 0 for the default Nor<> template");
private:
    Nor<InCount, Tech, Param>(const Nor<InCount, Tech, Param> &);
    Nor<InCount, Tech, Param> & operator = (const Nor<InCount, Tech, Param> &);
protected:
    Input<InCount>  A;
    Output<1>       Y;
    
    void            OnChange    (void);
public:
    Nor (const Signal<1> & y, const Signal<InCount> & a,
         const string & name = "nor<virt>?");
    Nor (const Signal<1> & y, const Signal<InCount> & a,
         TResistance yRes, TCapacitance yCap, TCapacitance aCap,
         const string & name = "nor<virt>?");
    Nor (const Signal<1> & y, const Signal<InCount> & a,
         TResistance yRes, TCapacitance yCap, const array<TCapacitance, InCount> & aCapVec,
         const string & name = "nor<virt>?");
    Nor (const Signal<1> & y, const array<Signal<1>, InCount> & aArr,
         TResistance yRes, TCapacitance yCap, const array<TCapacitance, InCount> & aCapVec,
         const string & name = "nor<virt>?");
    ~Nor();
};

template <unsigned int InCount, Technology Tech, TechParam Param>
Nor<InCount, Tech, Param>::Nor(const Signal<1> & y, const Signal<InCount> & a,
                               const string & name)
    : Module(name),
      A     (a, 0UL, this, static_cast<BusListener::THandler>(&Nor::OnChange), "A"),
      Y     (y, 0UL, 0UL, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <unsigned int InCount, Technology Tech, TechParam Param>
Nor<InCount, Tech, Param>::Nor(const Signal<1> & y, const Signal<InCount> & a,
                               TResistance yRes, TCapacitance yCap, TCapacitance aCap,
                               const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&Nor::OnChange), "A"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <unsigned int InCount, Technology Tech, TechParam Param>
Nor<InCount, Tech, Param>::Nor(const Signal<1> & y, const Signal<InCount> & a,
                               TResistance yRes, TCapacitance yCap, const array<TCapacitance, InCount> & aCapVec,
                               const string & name)
    : Module(name),
      A     (a, aCapVec, this, static_cast<BusListener::THandler>(&Nor::OnChange), "A"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
    
    // debug only
//    Y.GetSource().Name = "Nor.Y.S";
}

template <unsigned int InCount, Technology Tech, TechParam Param>
Nor<InCount, Tech, Param>::~Nor()
{
    
}

template <unsigned int InCount, Technology Tech, TechParam Param>
void Nor<InCount, Tech, Param>::OnChange(void)
{
//    printf("%s:%d Nor::OnChange: @%lu: A = %s, B = %s\n", __FILE__, __LINE__,
//           SimCtrl::GetTime(),
//           A.GetValue().ToString().c_str(),
//           B.GetValue().ToString().c_str());
    Logic result = Logic::L;
    
    for(unsigned int i = 0 ; i < InCount ; i++){
        result = result | this->A.GetValue()[i];
    }
    
    this->Y = ~result;
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_NOR_H_ */
