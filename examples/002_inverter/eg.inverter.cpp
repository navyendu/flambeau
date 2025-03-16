/**
 *  eg.inverter.cpp
 *  
 *  Created on: Mar 17, 2013
 *      Author: nav
 *  
 */

#include "flambeau/module.h"
#include "flambeau/signal.h"
#include "flambeau/source.h"

#include "flambeau/library/cells/not.h" // For Not<n> {}

#include "flambeau/datapath.h"          // For Y = ~X

using namespace Flambeau;
using namespace Flambeau::Synth;        // Needed for Y = ~X

class InverterTest : public TestBench
{
    Signal<1>   X1;
    Signal<1>   Y1;
    Signal<1>   Z1;
    
    Not<>       Not1    { Y1, X1 };             // Instantiate a Not gate
    Not<>       Not2    { Z1, X1, 5, 7, 11 };   // Another one, but with a delay
    
    Signal<3>   X2;
    Signal<3>   Y2 = ~X2;       // Some datapath modeling
    
    Source<1>   S1  { X1 };
    Source<3>   S2  { X2 };
public:
    InverterTest() : TestBench("InverterTest")
    {
        AddSignal(X1, "X1");
        AddSignal(Y1, "X2");
        AddSignal(Z1, "Z1");
        AddSignal(X2, "X2");
        AddSignal(Y2, "Y2");
        
        AddChildModule(Not1, "Not1");
        AddChildModule(Not2, "Not2_delay");
        
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&InverterTest::Init));
    }
    
    void Init()
    {
        for(unsigned int i = 0 ; i < 8 ; i++){
            S1.SetValue(LogicVec<1>(i));
            S2.SetValue(LogicVec<3>(i));
            
            SimCtrl::Delay(1000);
        }
        
        // TODO: Should have something similar to $finish of Verilog
    }
};

int main()
{
    InverterTest UUT;
    
    SimCtrl::Run(UUT, 50000);   // TODO: Must have a version of Run that runs
                                //  infinitely
    
    return 0;
}
