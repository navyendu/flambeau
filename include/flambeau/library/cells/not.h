/**
 *  not.h
 *  
 *  Created on: Nov 13, 2012
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_NOT_H_
#define LIBRARY_CELLS_NOT_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/not.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <Technology Tech = Technology::Virtual, TechParam Param = 0>
class Not : public Module
{
    static_assert(Param == 0, "Param must be 0 for the default Not<> template");
private:
    Not(const Not<Tech> &);
    Not<Tech> & operator = (const Not<Tech> &);
protected:
    Input<1>        A;
    Output<1>       Y;
    
    void            OnChange    (void);
public:
    Not (const Signal<1> & y, const Signal<1> & a,
         const string & name = "not<virt>?");
    Not (const Signal<1> & y, const Signal<1> & a,
         TResistance yRes, TCapacitance yCap, TCapacitance aCap,
         const string & name = "not<virt>?");
    ~Not();
};

template <Technology Tech, TechParam Param>
Not<Tech, Param>::Not(const Signal<1> & y, const Signal<1> & a,
                      const string & name)
    : Not(y, a, 0UL, 0UL, 0UL, name)
{}

template <Technology Tech, TechParam Param>
Not<Tech, Param>::Not(const Signal<1> & y, const Signal<1> & a,
                      TResistance yRes, TCapacitance yCap, TCapacitance aCap,
                      const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&Not::OnChange), "A"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <Technology Tech, TechParam Param>
Not<Tech, Param>::~Not()
{
    
}

template <Technology Tech, TechParam Param>
void Not<Tech, Param>::OnChange(void)
{
//    printf("%s:%d Not::OnChange: @%lu: A = %s\n", __FILE__, __LINE__, 
//           SimCtrl::GetTime(), A.GetValue().ToString().c_str());
    
    Y = ~A.GetValue();
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_NOT_H_ */
