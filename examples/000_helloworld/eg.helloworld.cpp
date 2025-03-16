/**
 *  hello.cpp
 *  
 *  Created on: Mar 15, 2013
 *      Author: nav
 *  
 */

#include "flambeau/module.h"
#include "flambeau/signal.h"
#include "flambeau/source.h"

#include "flambeau/library/cells/not.h"
#include "flambeau/library/cells/and.h"

using namespace Flambeau;

/* Create a (non-synthesizeable) Module. A non-synthesizeable module is
 * basically a class derived from the class TestBench. A non-synthesizeable 
 * module can have constructs that cannot be synthesized, such as scripts that
 * run on startup and at regular intervals (Verilog equivalents of 'initial' 
 * and 'always')
 */
class HelloModule : public TestBench
{
    Signal<1>   X_In    { "X_In" };     // Create a signal of width 1-bit, and name it "X_In"
    Signal<1>   X_1     { "X_1" };      // ! This style of object-initialization  
    Signal<1>   X_Out   { "X_Out" };    //  works only with C++11
    
    Source<1>   S1      { X_In };       // Create a voltage source and attach
                                        //  it to X_In
    
    // Create a Not-gate
    Not<>       Not1    { X_1,          // Output signal
                          X_In,         // Input signal
                          7,            // Output resistance
                          5,            // Output capacitance
                          5,            // Input capacitance
                          "Not1" };
    // and an And-gate
    And<>       And1    { X_Out, Signal<2>(X_1, X_In), 7, 5, 5, "And1" };
public:
    HelloModule() : TestBench("HelloModule")
    {
        AddSignal(X_In);
        AddSignal(X_1);
        AddSignal(X_Out);
        
        AddChildModule(Not1);
        AddChildModule(And1);
        
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&HelloModule::Init));
    }
    
    void Init(void)
    {
        for(unsigned int i = 0 ; i < 8 ; i++){
            S1.SetValue(LogicVec<1>(i));
            
            SimCtrl::Delay(1000);
        }
    }
};

int main()
{
    HelloModule UUT;
    
    SimCtrl::Run(UUT, 500000);  // TODO: Must have a version of Run that runs
                                //  infinitely
    
    return 0;
}
