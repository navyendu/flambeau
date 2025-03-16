/**
 *  eg.andorxor.cpp
 *  
 *  Created on: Mar 17, 2013
 *      Author: nav
 *  
 */

#include "flambeau/module.h"
#include "flambeau/signal.h"
#include "flambeau/source.h"

#include "flambeau/library/cells/and.h"
#include "flambeau/library/cells/or.h"
#include "flambeau/library/cells/xor.h"

#include "flambeau/datapath.h"          // For Y = A & B, A | B, A ^ B

using namespace Flambeau;
using namespace Flambeau::Synth;

class AndOrXor : public TestBench
{
    Signal<1>   A1;
    Signal<1>   B1;
    Signal<1>   Y_And1;
    Signal<1>   Y_And1D;
    Signal<1>   Y_Or1;
    Signal<1>   Y_Or1D;
    Signal<1>   Y_Xor1;
    Signal<1>   Y_Xor1D;
    
    And<>       And1    { Y_And1,  { A1, B1 } };
    And<>       And2    { Y_And1D, { A1, B1 }, 5, 7, 11 };
    Or<>        Or1     { Y_Or1,   { A1, B1 } };
    Or<>        Or2     { Y_Or1D,  { A1, B1 }, 5, 7, 11 };
    Xor<>       Xor1    { Y_Xor1,  { A1, B1 } };
    Xor<>       Xor2    { Y_Xor1D, { A1, B1 }, 5, 7, 11 };
    
    Signal<3>   A2;
    Signal<3>   B2;
    Signal<3>   Y_And2 = A2 & B2;
    Signal<3>   Y_Or2 = A2 | B2;
    Signal<3>   Y_Xor2 = A2 ^ B2;
    
    Source<1>   SA1 { A1 };
    Source<1>   SB1 { B1 };
    Source<3>   SA2 { A2 };
    Source<3>   SB2 { B2 };
public:
    AndOrXor() : TestBench("AndOrXor")
    {
        AddSignal(A1, "A1");
        AddSignal(B1, "B1");
        AddSignal(Y_And1, "Y_And1");
        AddSignal(Y_And1D, "Y_And1D");
        AddSignal(Y_Or1, "Y_Or1");
        AddSignal(Y_Or1D, "Y_Or1D");
        AddSignal(Y_Xor1, "Y_Xor1");
        AddSignal(Y_Xor1D, "Y_Xor1D");
        AddSignal(A2, "A2");
        AddSignal(B2, "B2");
        AddSignal(Y_And2, "Y_And2");
        AddSignal(Y_Or2, "Y_Or2");
        AddSignal(Y_Xor2, "Y_Xor2");
        
        AddChildModule(And1, "And1");
        AddChildModule(And2, "And2");
        AddChildModule(Or1, "Or1");
        AddChildModule(Or2, "Or2");
        AddChildModule(Xor1, "Xor1");
        AddChildModule(Xor2, "Xor2");
        
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&AndOrXor::Init));
    }
    
    void Init()
    {
        for(unsigned int i = 0 ; i < 2 ; i++){
            SA1.SetValue(LogicVec<1>(i));
            
            for(unsigned int j = 0 ; j < 2 ; j++){
                SB1.SetValue(LogicVec<1>(j));
                
                SimCtrl::Delay(1000);
            }
        }
        
        for(unsigned int i = 0 ; i < 8 ; i++){
            SA2.SetValue(LogicVec<3>(i));
            
            for(unsigned int j = 0 ; j < 8 ; j++){
                SB2.SetValue(LogicVec<3>(j));
                
                SimCtrl::Delay(1000);
            }
        }
    }
};

int main()
{
    AndOrXor UUT;
    
    SimCtrl::Run(UUT, 500000);   // TODO: Must have a version of Run that runs
                                //  infinitely
    
    return 0;
}
