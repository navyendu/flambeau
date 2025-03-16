/**
 *  fulladder.h
 *  
 *  Created on: Nov 14, 2012
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_FULLADDER_H_
#define LIBRARY_CELLS_FULLADDER_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/fulladder.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"

namespace Flambeau
{

template <Technology Tech = Technology::Virtual, TechParam Param = 0>
class FullAdder : public Module
{
    static_assert(Param == 0, "Param must be 0 for the default FullAdder<> template");
private:
    FullAdder(const FullAdder<Tech> &);
    FullAdder<Tech> & operator = (const FullAdder<Tech> &);
protected:
    Input<1>        A;
    Input<1>        B;
    Input<1>        Ci;
    Output<1>       S;
    Output<1>       Co;
    
    void            OnChange    (void);
public:
    FullAdder (const Signal<1> & s, const Signal<1> & co,
               const Signal<1> & a, const Signal<1> & b, const Signal<1> & ci,
               const string & name = "fulladder<virt>?");
    FullAdder (const Signal<1> & s, const Signal<1> & co,
               const Signal<1> & a, const Signal<1> & b, const Signal<1> & ci,
               TResistance sRes, TCapacitance sCap, TResistance coRes, TCapacitance coCap,
               TCapacitance aCap, TCapacitance bCap, TCapacitance cCap,
               const string & name = "fulladder<virt>?");
    ~FullAdder();
};

template <Technology Tech, TechParam Param>
FullAdder<Tech, Param>::FullAdder(const Signal<1> & s, const Signal<1> & co,
                                  const Signal<1> & a, const Signal<1> & b, const Signal<1> & ci,
                                  const string & name)
    : FullAdder(s, co, a, b, ci, 0, 0, 0, 0, 0, 0, 0, name)
{}

template <Technology Tech, TechParam Param>
FullAdder<Tech, Param>::FullAdder(const Signal<1> & s, const Signal<1> & co,
                                  const Signal<1> & a, const Signal<1> & b, const Signal<1> & ci,
                                  TResistance sRes, TCapacitance sCap, TResistance coRes, TCapacitance coCap,
                                  TCapacitance aCap, TCapacitance bCap, TCapacitance ciCap,
                                  const string & name)
    : Module(name),
      A     (a,  aCap,  this, static_cast<BusListener::THandler>(&FullAdder::OnChange), "A"),
      B     (b,  bCap,  this, static_cast<BusListener::THandler>(&FullAdder::OnChange), "B"),
      Ci    (ci, ciCap, this, static_cast<BusListener::THandler>(&FullAdder::OnChange), "Ci"),
      S     (s,  sCap,  sRes,  "S"),
      Co    (co, coCap, coRes, "Co")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(B.GetSignal());
    this->AddSignal(Ci.GetSignal());
    this->AddSignal(S.GetSignal());
    this->AddSignal(Co.GetSignal());
}

template <Technology Tech, TechParam Param>
FullAdder<Tech, Param>::~FullAdder()
{
    
}

template <Technology Tech, TechParam Param>
void FullAdder<Tech, Param>::OnChange(void)
{
//    printf("%s:%d FullAdder::OnChange: @%lu: A = %s\n", __FILE__, __LINE__, 
//           SimCtrl::GetTime(), A.GetValue().ToString().c_str());
    this->S  = this->A.GetValue() ^ this->B.GetValue() ^ this->Ci.GetValue();
    this->Co = (this->A.GetValue() & this->B.GetValue()) |
               (this->B.GetValue() & this->Ci.GetValue()) | 
               (this->Ci.GetValue() & this->A.GetValue());
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_FULLADDER_H_ */
