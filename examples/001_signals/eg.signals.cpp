/**
 *  eg.signals.cpp
 *  
 *  Created on: Mar 17, 2013
 *      Author: nav
 *  
 */

#include "flambeau/module.h"
#include "flambeau/signal.h"
#include "flambeau/source.h"

using namespace Flambeau;

class SignalsTest : public TestBench
{
    Signal<2>   X1;             // Create a 2-bit wide signal 
    Signal<3>   X2  { "X2" };   // 3-bit wide signal, with a name. Name is not
                                //  really necessary for simulation, but 
                                //  required for the wavedump
    
    Signal<5>   X3  { X1, X2, "X3" };   // Concatenate two signals, and name the resultant
    Signal<5>   X4  { X1, X2 };         // Concatenate, no name
    Signal<5>   X5  = { X1, X2 };       // Another way to concatenate
    
    Signal<10>  X6  { X1, "X_6" };      // Replicate a signal, and name the resultant
    Signal<10>  X7  { X5 };             // Replication, without naming
    
    Signal<7>   X8  { X7, 0 };          // Sub-vector: Gives X7[6:0] (Verilog)
    Signal<7>   X9  = { X7, 1 };        // Sub-vector: Gives X7[8:2] (Verilog)
    
    Signal<1>   X10 = X5[3];            // 1-bit subvector
    
    // And finally, attach a source for running some tests
    Source<5>   S5  { X5 };
public:
    SignalsTest() : TestBench("SignalsTest")
    {
        X1.SetName("X1");       // We can name the signals here or in-class.
        X2.SetName("X2");       //  Naming inside a constructor/member function
        X3.SetName("X3");       //  overrides the naming done in-class.
        X4.SetName("X4");
        X5.SetName("X5_");
        X6.SetName("X6_");
        
        AddSignal(X1);          // Add the signal into the module hierarchy
        AddSignal(X2);          // This is required for the wave-dump tool
        AddSignal(X3);          // But not really required by the simulator
        AddSignal(X4);
        AddSignal(X5, "X5");    // We can also specify the names here
        AddSignal(X6, "X6");
        AddSignal(X7, "X7");
        AddSignal(X8, "X8");
        AddSignal(X9, "X9");
        AddSignal(X10, "X10");
        
        // Set SignalsTest::Init to run during startup. Similar to an inital...
        //  construct of Verilog
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&SignalsTest::Init));
    }
    
    void Init()
    {
        for(unsigned int i = 0 ; i < 32 ; i++){
            S5.SetValue(LogicVec<5>(i));
            
            SimCtrl::Delay(1000);
        }
        
        // TODO: Should have something similar to $finish of Verilog
    }
};

int main()
{
    SignalsTest UUT;
    
    SimCtrl::Run(UUT, 50000);   // TODO: Must have a version of Run that runs
                                //  infinitely
    
    return 0;
}
