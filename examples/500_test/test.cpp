/**
 *  test.cpp
 *  
 *  Created on: Oct 27, 2012
 *      Author: nav
 *  
 */

#include <stdio.h>
#include <sys/time.h>

#include <iostream>
#include <vector>
#include <string>
#include <array>

using std::cout;
using std::array;
using std::vector;
using std::string;

#include "flambeau/logic.h"
#include "flambeau/signal.h"
#include "flambeau/source.h"
#include "flambeau/load.h"
#include "flambeau/listener.h"

#include "flambeau/module.h"

#include "flambeau/output.h"
#include "flambeau/input.h"

#include "flambeau/ctrl.h"

#include "flambeau/library/cells/clock.h"

#include "flambeau/library/cells/buffer.h"
#include "flambeau/library/cells/not.h"
#include "flambeau/library/cells/and.h"
#include "flambeau/library/cells/or.h"
#include "flambeau/library/cells/xor.h"
#include "flambeau/library/cells/nand.h"
#include "flambeau/library/cells/nor.h"
#include "flambeau/library/cells/xnor.h"
#include "flambeau/library/cells/tris.h"
#include "flambeau/library/cells/trisinv.h"
#include "flambeau/library/cells/mux.h"
#include "flambeau/library/cells/dff.h"
#include "flambeau/library/cells/halfadder.h"
#include "flambeau/library/cells/fulladder.h"

#include "flambeau/library/cells/cmos/not.h"

#include "flambeau/datapath.h"

using namespace Flambeau;
using namespace Flambeau::Synth;

//printf("%s:%d\n", __FILE__, __LINE__);

timeval TimeDiff(timeval a, timeval b)
{
    while(a.tv_usec < b.tv_usec){
        a.tv_usec += 1000000;
        a.tv_sec -= 1;
    }
    
    a.tv_sec -= b.tv_sec;
    a.tv_usec -= b.tv_usec;
    
    while(a.tv_usec >= 1000000){
        a.tv_usec -= 1000000;
        a.tv_sec += 1;
    }
    
    return a;
}

void ModuleDump(const Module & mod, unsigned int indentCount)
{
    for(unsigned int i = 0 ; i < mod.SignalList.size() ; i++){
        printf("%*s%s.%s (%lu) %lX\n", 4*indentCount, "", mod.GetName().c_str(), 
               mod.SignalList.at(i)->GetName().c_str(),
               mod.SignalList.at(i)->GetId(),
               (size_t) mod.SignalList.at(i));
    }
    
    for(unsigned int i = 0 ; i < mod.ChildModuleList.size() ; i++){
        printf("%*s%s:%s\n", 4*indentCount, "", mod.GetName().c_str(), 
               mod.ChildModuleList.at(i)->GetName().c_str());
        
        ModuleDump(*mod.ChildModuleList.at(i), indentCount + 1);
    }
}

class TestBool : public TestBench
{
    Signal<3>   X1              { "X1" };
    Signal<3>   X2              { "X2" };
    Signal<1>   XA              { "XA" };
    Signal<1>   XB              { "XB" };
    Signal<1>   XC              { "XC" };
    Signal<1>   XD              { "XD" };
    Signal<1>   X_BufY          { "X_BufY" };
    Signal<1>   X_NotY          { "X_NotY" };
    Signal<1>   X_AndY          { "X_AndY" };
    Signal<1>   X_OrY           { "X_OrY" };
    Signal<1>   X_XorY          { "X_XorY" };
    Signal<1>   X_NandY         { "X_NandY" };
    Signal<1>   X_NorY          { "X_NorY" };
    Signal<1>   X_XnorY         { "X_XnorY" };
    Signal<1>   X_TrisY         { "X_TrisY" };
    Signal<1>   X_TrisInvY      { "X_TrisInvY" };
    Signal<1>   X_MuxY          { "X_MuxY" };
    Signal<1>   X_HalfAddS      { "X_HalfAddS" };
    Signal<1>   X_HalfAddC      { "X_HalfAddC" };
    Signal<1>   X_FullAddS      { "X_FullAddS" };
    Signal<1>   X_FullAddC      { "X_FullAddC" };
    Signal<1>   X_Clk           { "X_Clk" };
    Signal<1>   X_DffQ          { "X_DffQ" };
    Signal<1>   X_ClkBar        { "X_ClkBar" };
    Signal<1>   X_ClkBar1_Cmos  { "X_ClkBar1_Cmos" };
    Signal<1>   X_ClkBar2_Cmos  { "X_ClkBar2_Cmos" };
    Signal<1>   X_ClkBar3_Cmos  { "X_ClkBar3_Cmos" };
    Signal<1>   X_ClkBar4_Cmos  { "X_ClkBar4_Cmos" };
    Signal<1>   X_NotY_Synth    { "X_NotY_Synth" };
    Signal<3>   X_Not_Synth     { "X_Not_Synth" };
    Signal<3>   X_And_Synth     { "X_And_Synth" };
    Signal<3>   X_Or_Synth      { "X_Or_Synth" };
    Signal<3>   X_Xor_Synth     { "X_Xor_Synth" };
    Signal<3>   X_Nand_Synth    { "X_Nand_Synth" };
    Signal<3>   X_Nor_Synth     { "X_Nor_Synth" };
    Signal<3>   X_Xnor_Synth    { "X_Xnor_Synth" };
    Signal<3>   X_Flop_Synth    { "X_Flop_Synth" };
    Signal<3>   X_Tris_Synth    { "X_Tris_Synth" };
    Signal<1>   X_Not_S         { "X_Not_S" };
    Signal<1>   X_And_S         { "X_And_S" };
    Signal<1>   X_Or_S          { "X_Or_S" };
    Signal<1>   X_Xor_S         { "X_Xor_S" };
    Signal<1>   X_Nand_S        { "X_Nand_S" };
    Signal<1>   X_Nor_S         { "X_Nor_S" };
    Signal<1>   X_Xnor_S        { "X_Xnor_S" };
    Signal<1>   X_Flop_S        { "X_Flop_S" };
    Signal<1>   X_Tris_S        { "X_Tris_S" };
    
    Source<3>   S1      { X1 };
    Source<3>   S2      { X2 };
    
//    Load<1>     L_NotY  { X_NotY,  11 };
//    Load<1>     L_AndY  { X_AndY,  11 };
//    Load<1>     L_OrY   { X_OrY,   11 };
//    Load<1>     L_XorY  { X_XorY,  11 };
//    Load<1>     L_NandY { X_NandY, 11 };
//    Load<1>     L_NorY  { X_NorY,  11 };
//    Load<1>     L_XnorY { X_XnorY, 11 };
//    Load<1>     L_DffQ  { X_DffQ,  11 };
    
    Buffer<>    Buf1        { X_BufY, X1[1], 7, 5, 5, "Buf1" };
    Not<>       Not1        { X_NotY, X1[1], 7, 5, 5, "Not1" };
    And<>       And1        { X_AndY, X1.SubVector<2>(0), 7, 5, 5, "And1" };
    Or<>        Or1         { X_OrY, X1.SubVector<2>(0), 7, 5, 5, "Or1" };
    Xor<>       Xor1        { X_XorY, X1.SubVector<2>(0), 7, 5, 5, "Xor1" };
    Nand<>      Nand1       { X_NandY, X1.SubVector<2>(0), 7, 5, 5, "Nand1" };
    Nor<>       Nor1        { X_NorY, X1.SubVector<2>(0), 7, 5, 5, "Nor1" };
    Xnor<>      Xnor1       { X_XnorY, X1.SubVector<2>(0), 7, 5, 5, "Xnor1" };
    Tris<>      Tris1       { X_TrisY, X1[0], X1[1], 7, 5, 5, 5, "Tris1" };
    TrisInv<>   TrisInv1    { X_TrisInvY, X1[0], X1[1], 7, 5, 5, 5, "TrisInv1" };
    Mux<>       Mux1        { X_MuxY, X1[0], X1.SubVector<2>(1), 7, 5, 5, 5, "Mux1" };
    HalfAdder<> Ha1         { X_HalfAddS, X_HalfAddC, X1[1], X1[0], "Ha1" };
    FullAdder<> Fa1         { X_FullAddS, X_FullAddC, X1[2], X1[1], X1[0], "Fa1" };
    ClockGen    Clk1        { X_Clk, 1000, 1000, false, 200, "Clk1" };
    DFlipFlop<> Dff1        { X_DffQ, X_Clk, X_XorY, 100, 10, 150, 5, 5, 5, 7, true, "Dff1" };
    
    Not<>       Not2        { X_ClkBar, X_Clk, 0, 0, 0, "Not2" };
    Not<Technology::Cmos, 1>    CmosNot1    { X_ClkBar1_Cmos, X_Clk, 1, "CmosNot1" };
    Not<Technology::Cmos, 2>    CmosNot2    { X_ClkBar2_Cmos, X_Clk, 1, "CmosNot2" };
    Not<Technology::Cmos, 3>    CmosNot3    { X_ClkBar3_Cmos, X_Clk, 1, "CmosNot3" };
    Not<Technology::Cmos, 4>    CmosNot4    { X_ClkBar4_Cmos, X_Clk, 1, "CmosNot4" };
public:
    TestBool(const string & name)
        : TestBench     (name)
    {
        XA = X2[0];
        XB = X2[1];
        XC = X2[2];
        XD = X1[0];
        
        X_NotY_Synth = ~X1[1];
        
        X_Not_Synth = ~X1;
        X_And_Synth = X1 & X2;
        X_Or_Synth = X1 | X2;
        X_Xor_Synth = X1 ^ X2;
        X_Nand_Synth = ~(X1 & X2);
        X_Nor_Synth = ~(X1 | X2);
        X_Xnor_Synth = ~(X1 ^ X2);
        X_Flop_Synth = X1 % Signal<3>(X_Clk);
        X_Tris_Synth = X1 / Signal<3>(X_Clk);
        
        X_Not_S = ~XA;
        X_And_S = XA & XB & XC & XD;
        X_Or_S = XA | XB | XC | XD;
        X_Xor_S = XA ^ XB ^ XC ^ XD;
        X_Nand_S = ~(XA & XB & XC & XD);
        X_Nor_S = ~(XA | XB | XC | XD);
        X_Xnor_S = ~(XA ^ XB ^ XC ^ XD);
        X_Flop_S = XC % X_Clk;
        X_Tris_S = XC / X_Clk;
        
        AddSignal(X1);
        AddSignal(X2);
        AddSignal(XA);
        AddSignal(XB);
        AddSignal(XC);
        AddSignal(XD);
        AddSignal(X_BufY);
        AddSignal(X_NotY);
        AddSignal(X_AndY);
        AddSignal(X_OrY);
        AddSignal(X_XorY);
        AddSignal(X_NandY);
        AddSignal(X_NorY);
        AddSignal(X_XnorY);
        AddSignal(X_TrisY);
        AddSignal(X_TrisInvY);
        AddSignal(X_MuxY);
        AddSignal(X_HalfAddS);
        AddSignal(X_HalfAddC);
        AddSignal(X_FullAddS);
        AddSignal(X_FullAddC);
        AddSignal(X_Clk);
        AddSignal(X_DffQ);
        AddSignal(X_ClkBar);
        AddSignal(X_ClkBar1_Cmos);
        AddSignal(X_ClkBar2_Cmos);
        AddSignal(X_ClkBar3_Cmos);
        AddSignal(X_ClkBar4_Cmos);
        AddSignal(X_NotY_Synth);
        AddSignal(X_Not_Synth);
        AddSignal(X_And_Synth);
        AddSignal(X_Or_Synth);
        AddSignal(X_Xor_Synth);
        AddSignal(X_Nand_Synth);
        AddSignal(X_Nor_Synth);
        AddSignal(X_Xnor_Synth);
        AddSignal(X_Flop_Synth);
        AddSignal(X_Tris_Synth);
        AddSignal(X_Not_S);
        AddSignal(X_And_S);
        AddSignal(X_Or_S);
        AddSignal(X_Xor_S);
        AddSignal(X_Nand_S);
        AddSignal(X_Nor_S);
        AddSignal(X_Xnor_S);
        AddSignal(X_Flop_S);
        AddSignal(X_Tris_S);
        
        AddChildModule(Buf1);
        AddChildModule(Not1);
        AddChildModule(And1);
        AddChildModule(Or1);
        AddChildModule(Xor1);
        AddChildModule(Nand1);
        AddChildModule(Nor1);
        AddChildModule(Xnor1);
        AddChildModule(Tris1);
        AddChildModule(TrisInv1);
        AddChildModule(Mux1);
        AddChildModule(Ha1);
        AddChildModule(Fa1);
        AddChildModule(Clk1);
        AddChildModule(Dff1);
        AddChildModule(Not2);
        AddChildModule(CmosNot1);
        AddChildModule(CmosNot2);
        AddChildModule(CmosNot3);
        AddChildModule(CmosNot4);
        
        SimCtrl::AddInit(*this, static_cast<void (TestBench::*)(void)>(&TestBool::Init));
        
        // debug only
//        S.Name = "SS";
    }
    
    void Init(void)
    {
        for(unsigned int i = 0 ; i < 8 ; i++){
            S1.SetValue(LogicVec<3>(i));
            
            for(unsigned int j = 0 ; j < 8 ; j++){
                S2.SetValue(LogicVec<3>(j));
                
                SimCtrl::Delay(1000);
            }
        }
    }
};

int main()
{
    TestBool test("test_bool");
    ModuleDump(test, 0);
    
    timeval startTime, endTime;
    
    gettimeofday(&startTime, nullptr);
    
    SimCtrl::Run(test, 500000);
    
    gettimeofday(&endTime, nullptr);
    
    printf("Time start = %lu.%06lu\n", startTime.tv_sec, startTime.tv_usec);
    printf("Time end = %lu.%06lu\n", endTime.tv_sec, endTime.tv_usec);
    
    timeval diffTime = TimeDiff(endTime, startTime);
    
    printf("Time elapsed = %lu.%06lu\n", diffTime.tv_sec, diffTime.tv_usec);
    
//    Signal<1> a;
//    Signal<1> b;
//    
//    ExpressionVec<1> ea(a);
//    ExpressionVec<1> eb(b);
//    
//    ea.Dumpp();
//    eb.Dumpp();
//    
////    array<Expression, 1> exprArr;
////    printf("%s:%d here\n", __FILE__, __LINE__);
////    for(unsigned int i = 0 ; i < 1 ; i++){
////        exprArr.at(i).SetFunc(ExprFunc::And);
////        exprArr.at(i).SetInvert(false);
////        exprArr.at(i) << ea.GetExprArray().at(i);
////        exprArr.at(i) << eb.GetExprArray().at(i);
////    }
////    printf("%s:%d here\n", __FILE__, __LINE__);
////    ExpressionVec<1> er(exprArr);
//    
//    ExpressionVec<1> er = a & b;
//    er.Dumpp();
//    er.Synthesize();
//    
//    printf("Alive = %d\n", Expression::_AliveCount);
    
    return 0;
}
