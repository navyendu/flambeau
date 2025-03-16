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

#include "flambeau/library/cells/mux.h"

using namespace Flambeau;

class MuxTest : public TestBench
{
    Signal<1>   A1;
    Signal<1>   B1;
    Signal<1>   S1;
    Signal<1>   O1;
    Signal<1>   O2;
    
    Mux<>       Mux1    { O1, S1, { A1, B1 } };
    Mux<>       Mux2    { O2, S1, { A1, B1 }, 5, 7, {{ 3 }}, {{ 11, 11 }} };
    
    Source<1>   SA1 { A1 };
    Source<1>   SB1 { B1 };
    Source<1>   SS1 { S1 };
public:
    MuxTest() : TestBench("MuxTest")
    {
        AddSignal(A1, "A1");
        AddSignal(B1, "B1");
        AddSignal(S1, "S1");
        AddSignal(O1, "O1");
        AddSignal(O2, "O2");
        
        AddChildModule(Mux1, "Mux1");
        AddChildModule(Mux2, "Mux2");
        
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&MuxTest::Init));
    }
    
    void Init()
    {
        for(unsigned int i = 0 ; i < 2 ; i++){
            SA1.SetValue(LogicVec<1>(i));
            
            for(unsigned int j = 0 ; j < 2 ; j++){
                SB1.SetValue(LogicVec<1>(j));
                
                for(unsigned int k = 0 ; k < 2 ; k++){
                    SS1.SetValue(LogicVec<1>(k));
                    
                    SimCtrl::Delay(1000);
                }
            }
        }
    }
};

int main()
{
    MuxTest UUT;
    
    SimCtrl::Run(UUT, 500000);  // TODO: Must have a version of Run that runs
                                //  infinitely
    
    return 0;
}
