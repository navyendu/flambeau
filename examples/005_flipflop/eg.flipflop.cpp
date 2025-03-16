/**
 *  eg.mux.cpp
 *  
 *  Created on: Mar 18, 2013
 *      Author: nav
 *  
 */

#include "flambeau/module.h"
#include "flambeau/signal.h"
#include "flambeau/source.h"

#include "flambeau/library/cells/dff.h"
#include "flambeau/library/cells/clock.h"

using namespace Flambeau;

class DffTest : public TestBench
{
    Signal<1>   D1;
    Signal<1>   C1;
    Signal<1>   Q1;
    Signal<1>   Q2;
    
    DFlipFlop<> Dff1    { Q1, C1, D1, true };
    DFlipFlop<> Dff2    { Q2, C1, D1, 150, 70, 200, 3, 5, 7, 11, true };
    
    ClockGen    Clk1    { C1, 1000, false };
    
    Source<1>   SD1     { D1 };
public:
    DffTest() : TestBench("DffTest")
    {
        AddSignal(D1, "D1");
        AddSignal(C1, "C1");
        AddSignal(Q1, "Q1");
        AddSignal(Q2, "Q2");
        
        AddChildModule(Dff1, "Dff1");
        AddChildModule(Dff2, "Dff2");
        AddChildModule(Clk1, "Clk1");
        
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&DffTest::Init));
    }
    
    void Init()
    {
        SimCtrl::Delay(300);
        
        SD1.SetValue(Logic::L);     // @300
        
        SimCtrl::Delay(1000);
        
        SD1.SetValue(Logic::H);     // @1300
        
        SimCtrl::Delay(1000);
        
        SD1.SetValue(Logic::L);     // @2300
        
        SimCtrl::Delay(1560);       // Introduce a setup violation
        
        SD1.SetValue(Logic::H);     // @3860
        
        SimCtrl::Delay(2000);       // Another setup violation
        
        SD1.SetValue(Logic::L);     // @5860
        
        SimCtrl::Delay(2200);       // Now, a hold violation
        
        SD1.SetValue(Logic::H);     // @8060
        
        SimCtrl::Delay(2000);       // Another hold violation
        
        SD1.SetValue(Logic::L);     // @10060
        
        // Only Dff2 will suffer from setup and hold violations. Dff1 has 
        //  setup and hold times equal to 0
    }
};

int main()
{
    DffTest UUT;
    
    SimCtrl::Run(UUT, 12000);   // TODO: Must have a version of Run that runs
                                //  infinitely
    
    return 0;
}
