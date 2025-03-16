/**
 *  clock.h
 *  
 *  Created on: Jan 21, 2013
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_CLOCK_H_
#define LIBRARY_CELLS_CLOCK_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/clock.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

class ClockGen : public TestBench
{
private:
    Output<1>   Clock;
    
    TSimTime    StartDelay;
    bool        StartOnHi;
    TSimTime    OnTime;
    TSimTime    OffTime;
    
    void        Process (void);
public:
    ClockGen (const Signal<1> & clockSig, TSimTime period, bool startOnHi = false,
              const string & name = "");
    ClockGen (const Signal<1> & clockSig, TSimTime onTime, TSimTime offTime, 
              bool startOnHi = false, TSimTime startDelay = 0, const string & name = "");
    ~ClockGen();
};

ClockGen::ClockGen(const Signal<1> & clockSig, TSimTime period, bool startOnHi,
                   const string & name)
    : ClockGen(clockSig, period / 2, period - period / 2, startOnHi, 0, name)
{
    
}

ClockGen::ClockGen(const Signal<1> & clockSig, TSimTime onTime, TSimTime offTime,
                   bool startOnHi, TSimTime startDelay, const string & name)
    : TestBench     (name),
      Clock         (clockSig),
      StartDelay    (startDelay),
      StartOnHi     (startOnHi),
      OnTime        (onTime),
      OffTime       (offTime)
{
    Clock.GetSignal().SetName("Clock");
    
    this->AddSignal(Clock.GetSignal());
    
    SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&ClockGen::Process));
}

ClockGen::~ClockGen()
{
    
}

void ClockGen::Process(void)
{
    array<TSimTime, 2> halfPeriods;
    
    if(this->StartOnHi){
        halfPeriods.at(0) = this->OffTime;
        halfPeriods.at(1) = this->OnTime;
        
        this->Clock.GetSource().SetValue(Logic::H);
    }else{
        halfPeriods.at(0) = this->OnTime;
        halfPeriods.at(1) = this->OffTime;
        
        this->Clock.GetSource().SetValue(Logic::L);
    }
    
    SimCtrl::Delay(this->StartDelay);
    
    while(1){
        for(unsigned int i = 0 ; i < 2 ; i++){
            this->Clock.GetSource().SetValue(~this->Clock.GetSource().GetValue());
            
            SimCtrl::Delay(halfPeriods.at(i));
        }
    }
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_CLOCK_H_ */
