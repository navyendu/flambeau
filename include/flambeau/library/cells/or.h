/**
 *  or.h
 *  
 *  Created on: Jan 19, 2013
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_OR_H_
#define LIBRARY_CELLS_OR_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/or.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <unsigned int InCount = 2, Technology Tech = Technology::Virtual, TechParam Param = 0>
class Or : public Module
{
    static_assert(InCount >= 2, "Or gate must have atleast 2 input lines");
    static_assert(Param == 0, "Param must be 0 for the default Or<> template");
private:
    Or<InCount, Tech, Param>(const Or<InCount, Tech, Param> &);
    Or<InCount, Tech, Param> & operator = (const Or<InCount, Tech, Param> &);
protected:
    Input<InCount>  A;
    Output<1>       Y;
    
    void            OnChange    (void);
public:
    Or (const Signal<1> & y, const Signal<InCount> & a,
        const string & name = "or<virt>?");
    Or (const Signal<1> & y, const Signal<InCount> & a,
        TResistance yRes, TCapacitance yCap, TCapacitance aCap,
        const string & name = "or<virt>?");
    Or (const Signal<1> & y, const Signal<InCount> & a,
        TResistance yRes, TCapacitance yCap, const array<TCapacitance, InCount> & aCapVec,
        const string & name = "or<virt>?");
    Or (const Signal<1> & y, const array<Signal<1>, InCount> & aArr,
        TResistance yRes, TCapacitance yCap, const array<TCapacitance, InCount> & aCapVec,
        const string & name = "or<virt>?");
    ~Or();
};

template <unsigned int InCount, Technology Tech, TechParam Param>
Or<InCount, Tech, Param>::Or(const Signal<1> & y, const Signal<InCount> & a,
                             const string & name)
    : Module(name),
      A     (a, 0UL, this, static_cast<BusListener::THandler>(&Or::OnChange), "A"),
      Y     (y, 0UL, 0UL, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <unsigned int InCount, Technology Tech, TechParam Param>
Or<InCount, Tech, Param>::Or(const Signal<1> & y, const Signal<InCount> & a,
                             TResistance yRes, TCapacitance yCap, TCapacitance aCap,
                             const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&Or::OnChange), "A"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <unsigned int InCount, Technology Tech, TechParam Param>
Or<InCount, Tech, Param>::Or(const Signal<1> & y, const Signal<InCount> & a,
                             TResistance yRes, TCapacitance yCap, const array<TCapacitance, InCount> & aCapVec,
                             const string & name)
    : Module(name),
      A     (a, aCapVec, this, static_cast<BusListener::THandler>(&Or::OnChange), "A"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
    
    // debug only
//    Y.GetSource().Name = "Or.Y.S";
}

template <unsigned int InCount, Technology Tech, TechParam Param>
Or<InCount, Tech, Param>::~Or()
{
    
}

template <unsigned int InCount, Technology Tech, TechParam Param>
void Or<InCount, Tech, Param>::OnChange(void)
{
//    printf("%s:%d Or::OnChange: @%lu: A = %s, B = %s\n", __FILE__, __LINE__,
//           SimCtrl::GetTime(),
//           A.GetValue().ToString().c_str(),
//           B.GetValue().ToString().c_str());
    Logic result = Logic::L;
    
    for(unsigned int i = 0 ; i < InCount ; i++){
        result = result | this->A.GetValue()[i];
    }
    
    this->Y = result;
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_OR_H_ */
