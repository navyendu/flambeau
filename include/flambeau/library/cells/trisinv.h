/**
 *  trisinv.h
 *  
 *  Created on: Jan 21, 2013
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_TRISINV_H_
#define LIBRARY_CELLS_TRISINV_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/trisinv.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <Technology Tech = Technology::Virtual, TechParam Param = 0>
class TrisInv : public Module
{
    static_assert(Param == 0, "Param must be 0 for the default TrisInv<> template");
private:
    TrisInv(const TrisInv<Tech> &);
    TrisInv<Tech> & operator = (const TrisInv<Tech> &);
protected:
    Input<1>        A;
    Input<1>        S;
    Output<1>       Y;
    
    void            OnChange    (void);
public:
    TrisInv (const Signal<1> & y, const Signal<1> & s, const Signal<1> & a,
             const string & name = "trisinv<virt>?");
    TrisInv (const Signal<1> & y, const Signal<1> & s, const Signal<1> & a,
             TResistance yRes, TCapacitance yCap, TCapacitance sCap, TCapacitance aCap,
             const string & name = "trisinv<virt>?");
    ~TrisInv();
};

template <Technology Tech, TechParam Param>
TrisInv<Tech, Param>::TrisInv(const Signal<1> & y, const Signal<1> & s, const Signal<1> & a,
                              const string & name)
    : TrisInv(y, s, a, 0, 0, 0, 0, name)
{}

template <Technology Tech, TechParam Param>
TrisInv<Tech, Param>::TrisInv(const Signal<1> & y, const Signal<1> & s, const Signal<1> & a,
                              TResistance yRes, TCapacitance yCap, TCapacitance sCap, TCapacitance aCap,
                              const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&TrisInv::OnChange), "A"),
      S     (s, sCap, this, static_cast<BusListener::THandler>(&TrisInv::OnChange), "S"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(S.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <Technology Tech, TechParam Param>
TrisInv<Tech, Param>::~TrisInv()
{
    
}

template <Technology Tech, TechParam Param>
void TrisInv<Tech, Param>::OnChange(void)
{
//    printf("%s:%d TrisInv::OnChange: @%lu: A = %s\n", __FILE__, __LINE__, 
//           SimCtrl::GetTime(), A.GetValue().ToString().c_str());
    
    Y = ~A.GetValue() / S.GetValue();
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_TRISINV_H_ */
