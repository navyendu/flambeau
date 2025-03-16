/**
 *  fulladder.h
 *  
 *  Created on: Nov 14, 2012
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_HALFADDER_H_
#define LIBRARY_CELLS_HALFADDER_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/fulladder.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"

namespace Flambeau
{

template <Technology Tech = Technology::Virtual, TechParam Param = 0>
class HalfAdder : public Module
{
    static_assert(Param == 0, "Param must be 0 for the default HalfAdder<> template");
private:
    HalfAdder(const HalfAdder<Tech> &);
    HalfAdder<Tech> & operator = (const HalfAdder<Tech> &);
protected:
    Input<1>        A;
    Input<1>        B;
    Output<1>       S;
    Output<1>       C;
    
    void            OnChange    (void);
public:
    HalfAdder (const Signal<1> & s, const Signal<1> & c,
               const Signal<1> & a, const Signal<1> & b,
               const string & name = "halfadder<virt>?");
    HalfAdder (const Signal<1> & s, const Signal<1> & c,
               const Signal<1> & a, const Signal<1> & b,
               TResistance sRes, TCapacitance sCap, TResistance cRes, TCapacitance cCap,
               TCapacitance aCap, TCapacitance bCap,
               const string & name = "halfadder<virt>?");
    ~HalfAdder();
};

template <Technology Tech, TechParam Param>
HalfAdder<Tech, Param>::HalfAdder(const Signal<1> & s, const Signal<1> & c,
                                  const Signal<1> & a, const Signal<1> & b,
                                  const string & name)
    : HalfAdder(s, c, a, b, 0, 0, 0, 0, 0, 0, name)
{}

template <Technology Tech, TechParam Param>
HalfAdder<Tech, Param>::HalfAdder(const Signal<1> & s, const Signal<1> & c,
                                  const Signal<1> & a, const Signal<1> & b,
                                  TResistance sRes, TCapacitance sCap, TResistance cRes, TCapacitance cCap,
                                  TCapacitance aCap, TCapacitance bCap,
                                  const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&HalfAdder::OnChange), "A"),
      B     (b, bCap, this, static_cast<BusListener::THandler>(&HalfAdder::OnChange), "B"),
      S     (s, sCap, sRes, "S"),
      C     (c, cCap, cRes, "C")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(B.GetSignal());
    this->AddSignal(S.GetSignal());
    this->AddSignal(C.GetSignal());
}

template <Technology Tech, TechParam Param>
HalfAdder<Tech, Param>::~HalfAdder()
{
    
}

template <Technology Tech, TechParam Param>
void HalfAdder<Tech, Param>::OnChange(void)
{
//    printf("%s:%d HalfAdder::OnChange: @%lu: A = %s\n", __FILE__, __LINE__, 
//           SimCtrl::GetTime(), A.GetValue().ToString().c_str());
    this->S = this->A.GetValue() ^ this->B.GetValue();
    this->C = this->A.GetValue() & this->B.GetValue();
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_HALFADDER_H_ */
