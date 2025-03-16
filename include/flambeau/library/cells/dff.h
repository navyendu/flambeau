/**
 *  flambeau/dff.h
 *  
 *  Created on: Jan 3, 2013
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_DFF_H_
#define LIBRARY_CELLS_DFF_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/dff.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <Technology Tech = Technology::Virtual, TechParam Param = 0>
class DFlipFlop : public Module
{
    static_assert(Param == 0, "Param must be 0 for the default DFlipFlop<> template");
private:
    DFlipFlop(const DFlipFlop<Tech> &);
    DFlipFlop<Tech> & operator = (const DFlipFlop<Tech> &);
private:
    TSimTime        LastPosClkEdge;
    TSimTime        LastChangeOnD;
protected:
    Input<1>        C;
    Input<1>        D;
    Output<1>       Q;
    
    TSimTime        SetupTime;
    TSimTime        HoldTime;
    TSimTime        ClockToQ;
    
    LineListener    LC;             // We need a LineListener for Clock
    
    void            OnClockChange   (void);
    void            OnDChange       (void);
public:
    DFlipFlop (const Signal<1> & q, const Signal<1> & c, const Signal<1> & d,
               bool posedge = true, const string & name = "dff?");
    DFlipFlop (const Signal<1> & q, const Signal<1> & c, const Signal<1> & d,
               TSimTime setupTime, TSimTime holdTime, TSimTime clockToQ,
               bool posedge = true, const string & name = "dff?");
    DFlipFlop (const Signal<1> & q, const Signal<1> & c, const Signal<1> & d,
               TSimTime setupTime, TSimTime holdTime, TSimTime clockToQ,
               TCapacitance cCap, TCapacitance dCap, TCapacitance qCap, TResistance qRes,
               bool posedge = true, const string & name = "dff?");
    ~DFlipFlop();
};

template <Technology Tech, TechParam Param>
DFlipFlop<Tech, Param>::DFlipFlop(const Signal<1> & q, const Signal<1> & c, const Signal<1> & d,
                                  bool posedge, const string & name)
    : DFlipFlop(q, c, d, 0, 0, 0, 0, 0, 0, 0, posedge, name)
{}

template <Technology Tech, TechParam Param>
DFlipFlop<Tech, Param>::DFlipFlop(const Signal<1> & q, const Signal<1> & c, const Signal<1> & d,
                                  TSimTime setupTime, TSimTime holdTime, TSimTime clockToQ,
                                  bool posedge, const string & name)
    : DFlipFlop(q, c, d, setupTime, holdTime, clockToQ, 0, 0, 0, 0, posedge, name)
{}

template <Technology Tech, TechParam Param>
DFlipFlop<Tech, Param>::DFlipFlop(const Signal<1> & q, const Signal<1> & c, const Signal<1> & d,
                                  TSimTime setupTime, TSimTime holdTime, TSimTime clockToQ,
                                  TCapacitance cCap, TCapacitance dCap, TCapacitance qCap, TResistance qRes,
                                  bool posedge, const string & name)
    : Module        (name),
      LastPosClkEdge(-1),
      LastChangeOnD (-1),
      C             (c, cCap, nullptr, nullptr, "C"),   // No BusListener for Clock
      D             (d, dCap, this, static_cast<BusListener::THandler>(&DFlipFlop::OnDChange), "D"),
      Q             (q, qCap, qRes, "Q"),
      SetupTime     (setupTime),
      HoldTime      (holdTime),
      ClockToQ      (clockToQ),
      LC            (this, static_cast<void (Module::*)(void)>(&DFlipFlop::OnClockChange),
                     posedge ? LineListener::PosEdge : LineListener::NegEdge)
{
    this->AddSignal(this->C.GetSignal());
    this->AddSignal(this->D.GetSignal());
    this->AddSignal(this->Q.GetSignal());
    
    this->C.GetSignal().AddLineListener(this->LC);
}

template <Technology Tech, TechParam Param>
DFlipFlop<Tech, Param>::~DFlipFlop()
{
    
}

template <Technology Tech, TechParam Param>
void DFlipFlop<Tech, Param>::OnClockChange(void)
{
    this->LastPosClkEdge = SimCtrl::GetTime();
    
    if(this->LastPosClkEdge - this->LastChangeOnD < this->SetupTime){   // Setup violation
        this->Q = Logic::X;
    }else{
        this->Q = this->D.GetValue();
    }
}

template <Technology Tech, TechParam Param>
void DFlipFlop<Tech, Param>::OnDChange(void)
{
    this->LastChangeOnD = SimCtrl::GetTime();
    
    if(this->LastChangeOnD - this->LastPosClkEdge < this->HoldTime){    // Hold violation
        this->Q = Logic::X;
    }
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_DFF_H_ */
