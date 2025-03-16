/**
 *  mux.h
 *  
 *  Created on: Nov 15, 2012
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_MUX_H_
#define LIBRARY_CELLS_MUX_H_

#ifndef __cplusplus
#error Must use C++ for library/cells/mux.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <unsigned int AWidth = 1, Technology Tech = Technology::Virtual, TechParam Param = 0>
class Mux : public Module
{
    static_assert(AWidth >= 1, "Mux must have atleast 2 input lines");
    static_assert(Param == 0, "Param must be 0 for the default Mux<> template");
private:
    Mux<AWidth, Tech, Param>(const Mux<AWidth, Tech, Param> &);
    Mux<AWidth, Tech, Param> & operator = (const Mux<AWidth, Tech, Param> &);
protected:
    Input<Power(2, AWidth)> A;
    Input<AWidth>           S;
    Output<1>               Y;
    
    void            OnChange    (void);
public:
    Mux (const Signal<1> & y, const Signal<AWidth> & s, const Signal<Power(2, AWidth)> & a,
         const string & name = "mux<virt>?");
    Mux (const Signal<1> & y, const Signal<AWidth> & s, const Signal<Power(2, AWidth)> & a,
         TResistance yRes, TCapacitance yCap, TCapacitance sCap, TCapacitance aCap,
         const string & name = "mux<virt>?");
    Mux (const Signal<1> & y, const Signal<AWidth> & s, const Signal<Power(2, AWidth)> & a,
         TResistance yRes, TCapacitance yCap,
         const array<TCapacitance, AWidth> & sCapVec,
         const array<TCapacitance, Power(2, AWidth)> & aCapVec,
         const string & name = "mux<virt>?");
    Mux (const Signal<1> & y, const Signal<AWidth> & s, const array<Signal<1>, Power(2, AWidth)> & aArr,
         TResistance yRes, TCapacitance yCap, const array<TCapacitance, Power(2, AWidth)> & aCapVec,
         const string & name = "mux<virt>?");
    ~Mux();
};

template <unsigned int AWidth, Technology Tech, TechParam Param>
Mux<AWidth, Tech, Param>::Mux(const Signal<1> & y, const Signal<AWidth> & s, const Signal<Power(2, AWidth)> & a,
                              const string & name)
    : Module(name),
      A     (a, 0UL, this, static_cast<BusListener::THandler>(&Mux::OnChange), "A"),
      S     (s, 0UL, this, static_cast<BusListener::THandler>(&Mux::OnChange), "S"),
      Y     (y, 0UL, 0UL, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(S.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <unsigned int AWidth, Technology Tech, TechParam Param>
Mux<AWidth, Tech, Param>::Mux(const Signal<1> & y, const Signal<AWidth> & s, const Signal<Power(2, AWidth)> & a,
                              TResistance yRes, TCapacitance yCap, TCapacitance sCap, TCapacitance aCap,
                              const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&Mux::OnChange), "A"),
      S     (s, sCap, this, static_cast<BusListener::THandler>(&Mux::OnChange), "S"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(S.GetSignal());
    this->AddSignal(Y.GetSignal());
    
    // debug only
//    Y.GetSource().Name = "Mux.Y.S";
}

template <unsigned int AWidth, Technology Tech, TechParam Param>
Mux<AWidth, Tech, Param>::Mux(const Signal<1> & y, const Signal<AWidth> & s, const Signal<Power(2, AWidth)> & a,
                              TResistance yRes, TCapacitance yCap,
                              const array<TCapacitance, AWidth> & sCapVec,
                              const array<TCapacitance, Power(2, AWidth)> & aCapVec,
                              const string & name)
    : Module(name),
      A     (a, aCapVec, this, static_cast<BusListener::THandler>(&Mux::OnChange), "A"),
      S     (s, sCapVec, this, static_cast<BusListener::THandler>(&Mux::OnChange), "S"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(S.GetSignal());
    this->AddSignal(Y.GetSignal());
    
    // debug only
//    Y.GetSource().Name = "Mux.Y.S";
}

template <unsigned int AWidth, Technology Tech, TechParam Param>
Mux<AWidth, Tech, Param>::~Mux()
{
    
}

template <unsigned int AWidth, Technology Tech, TechParam Param>
void Mux<AWidth, Tech, Param>::OnChange(void)
{
//    printf("%s:%d Mux::OnChange: @%lu: A = %s, B = %s\n", __FILE__, __LINE__,
//           SimCtrl::GetTime(),
//           A.GetValue().ToString().c_str(),
//           B.GetValue().ToString().c_str());
    
    if(this->S.GetValue().IsInt()){
        uint64_t index = this->S.GetValue().ToInt();
        
        this->Y = this->A.GetValue()[index];
    }else{
        this->Y = Logic::X;
    }
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_MUX_H_ */
