/**
 *  expression.cpp
 *  
 *  Created on: Jan 14, 2013
 *      Author: nav
 *  
 */

#include "flambeau/expression.h"

#include "flambeau/library/cells/not.h"
#include "flambeau/library/cells/and.h"
#include "flambeau/library/cells/or.h"
#include "flambeau/library/cells/xor.h"
#include "flambeau/library/cells/nand.h"
#include "flambeau/library/cells/nor.h"
#include "flambeau/library/cells/xnor.h"
#include "flambeau/library/cells/dff.h"
#include "flambeau/library/cells/tris.h"

namespace Flambeau
{

namespace Synth
{

Expression::Term::Term(Node & sigLine)
    : IsSig     (true),
      SigLine_p (&sigLine)
{}

Expression::Term::Term(const Expression & expr)
    : IsSig     (false),
      Expr_p    (&expr)
{}

Expression::Term::Term(const Expression::Term & term)
    : IsSig     (term.IsSig),
      Expr_p    (term.Expr_p)
{}

Expression::Term::~Term()
{
    
}

//---------------------------------------------------------------------------//

int Expression::_IdCount = 0;
int Expression::_AliveCount = 0;

Expression * Expression::Create(ExprFunc func, bool invert)
{
    return new Expression(func, invert);
}

Expression * Expression::Create(const Expression & expr)
{
    return new Expression(expr.Func, expr.Invert);
}

void Expression::Destroy(Expression * expr)
{
    delete expr;
}

Expression::Expression(ExprFunc func, bool invert)
    : _Id       (_IdCount++),
      Func      (func),
      Invert    (invert),
      TermList  ()
{
//    printf("%s:%d Id=%d, Func=%d, Invert=%d, TermCount=%lu\n", __FILE__, __LINE__,
//           this->_Id, (int)this->Func, this->Invert, this->TermList.size());
    
    Expression::_AliveCount++;
}

Expression::~Expression()
{
//    printf("%s:%d Id=%d, Func=%d, Invert=%d, TermCount=%lu\n", __FILE__, __LINE__,
//           this->_Id, (int)this->Func, this->Invert, this->TermList.size());
    
    Expression::_AliveCount--;
}

//Expression::operator Node() const
//{
//    return this->Synthesize();
//}

void Expression::SetFunc(ExprFunc op)
{
    this->Func = op;
}

void Expression::SetInvert(bool invert)
{
    this->Invert = invert;
}

Expression & Expression::operator << (Node & sigLine)
{
    if(this->TermList.size() >= 4){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    this->TermList.push_back(Expression::Term(sigLine));
    
    return *this;
}

Expression & Expression::operator << (const Expression & expr)
{
    if(this->TermList.size() >= 4){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    this->TermList.push_back(Expression::Term(expr));
    
    return *this;
}

Node & Expression::Synthesize(void) const
{
    printf("%s:%d Func=%d, Invert=%d, TermCount=%lu\n", __FILE__, __LINE__,
           (int)this->Func, this->Invert, this->TermList.size());
    
    Node * result_p;
    
    switch(this->Func){
        case ExprFunc::None: {
            // The code just shouldn't come here
            throw GeneralException(__FILE__, __LINE__);
            
            break;
        }
        case ExprFunc::Pass: {
            printf("%s:%d Pass\n", __FILE__, __LINE__);
            
            this->SynthesizePass(result_p);
            
            break;
        }
        case ExprFunc::And: {
            printf("%s:%d And\n", __FILE__, __LINE__);
            
            this->SynthesizeAnd(result_p);
            
            break;
        }
        case ExprFunc::Or: {
            printf("%s:%d Or\n", __FILE__, __LINE__);
            
            this->SynthesizeOr(result_p);
            
            break;
        }
        case ExprFunc::Xor: {
            printf("%s:%d Xor\n", __FILE__, __LINE__);
            
            this->SynthesizeXor(result_p);
            
            break;
        }
        case ExprFunc::Add: {
            break;
        }
        case ExprFunc::Sub: {
            break;
        }
        case ExprFunc::Wire: {
            break;
        }
        case ExprFunc::Tris: {
            printf("%s:%d Tris\n", __FILE__, __LINE__);
            
            this->SynthesizeTris(result_p);
            
            break;
        }
        case ExprFunc::Flop: {
            printf("%s:%d Flop\n", __FILE__, __LINE__);
            
            this->SynthesizeFlop(result_p);
            
            break;
        }
        default: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
    }
    
    if(this->Invert){
        printf("%s:%d Invert\n", __FILE__, __LINE__);
        
        static uint64_t count = 0;
        
        array<Node *, 1> sigLineArr{{ result_p }};
        
        Signal<1> invInput(sigLineArr);
        Signal<1> invOutput;
        
        stringstream ss;
        ss << "__expr_inf_not_" << count;
        count++;
        
        Not<Technology::Virtual> * not1 = new Not<Technology::Virtual>(invOutput, invInput, ss.str());
        Module::ScratchPad.AddChildModule(*not1);
        
        result_p = invOutput.GetSigLineVec().at(0);
    }
    
    return *result_p;
}

void Expression::SynthesizePass(Node *& sigLine_p) const
{
//    printf("%s:%d TermList.size = %lu\n", __FILE__, __LINE__, this->TermList.size());
    
    if(this->TermList.size() != 1){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    sigLine_p = this->TermList.at(0).IsSig ? this->TermList.at(0).SigLine_p :
                                             &this->TermList.at(0).Expr_p->Synthesize();
}

void Expression::SynthesizeAnd(Node *& sigLine_p) const
{
    switch(this->TermList.size()){
        case 0: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
        case 1: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
        case 2: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 2> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            
            Signal<2> andInput(sigLineArr);
            Signal<1> andOutput;
            
            stringstream ss;
            ss << "__expr_inf_and2_" << count++;
            
            And<2, Technology::Virtual> * and1 =
                            new And<2, Technology::Virtual>(andOutput, andInput, ss.str());
            Module::ScratchPad.AddChildModule(*and1);
            
            sigLine_p = andOutput.GetSigLineVec().at(0);
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            break;
        }
        case 3: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 3> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            sigLineArr.at(2) = this->TermList.at(2).IsSig ?
                               this->TermList.at(2).SigLine_p :
                               &this->TermList.at(2).Expr_p->Synthesize();
            
            Signal<3> andInput(sigLineArr);
            Signal<1> andOutput;
            
            stringstream ss;
            ss << "__expr_inf_and3_" << count++;
            
            And<3, Technology::Virtual> * and1 =
                            new And<3, Technology::Virtual>(andOutput, andInput, ss.str());
            Module::ScratchPad.AddChildModule(*and1);
            
            sigLine_p = andOutput.GetSigLineVec().at(0);
            
            break;
        }
        case 4: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 4> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            sigLineArr.at(2) = this->TermList.at(2).IsSig ?
                               this->TermList.at(2).SigLine_p :
                               &this->TermList.at(2).Expr_p->Synthesize();
            sigLineArr.at(3) = this->TermList.at(3).IsSig ?
                               this->TermList.at(3).SigLine_p :
                               &this->TermList.at(3).Expr_p->Synthesize();
            
            Signal<4> andInput(sigLineArr);
            Signal<1> andOutput;
            
            stringstream ss;
            ss << "__expr_inf_and4_" << count++;
            
            And<4, Technology::Virtual> * and1 =
                            new And<4, Technology::Virtual>(andOutput, andInput, ss.str());
            Module::ScratchPad.AddChildModule(*and1);
            
            sigLine_p = andOutput.GetSigLineVec().at(0);
            
            break;
        }
        default: {
            // The code shouldn't reach here
            throw GeneralException(__FILE__, __LINE__);
            
            break;
        }
    }
}

void Expression::SynthesizeOr(Node *& sigLine_p) const
{
    switch(this->TermList.size()){
        case 0: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
        case 1: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
        case 2: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 2> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            
            Signal<2> orInput(sigLineArr);
            Signal<1> orOutput;
            
            stringstream ss;
            ss << "__expr_inf_or2_" << count++;
            
            Or<2, Technology::Virtual> * or1 =
                            new Or<2, Technology::Virtual>(orOutput, orInput, ss.str());
            Module::ScratchPad.AddChildModule(*or1);
            
            sigLine_p = orOutput.GetSigLineVec().at(0);
            
//            printf("%s:%d sigLine_p = %lX\n", __FILE__, __LINE__, (size_t) sigLine_p);
            
            break;
        }
        case 3: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 3> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            sigLineArr.at(2) = this->TermList.at(2).IsSig ?
                               this->TermList.at(2).SigLine_p :
                               &this->TermList.at(2).Expr_p->Synthesize();
            
            Signal<3> orInput(sigLineArr);
            Signal<1> orOutput;
            
            stringstream ss;
            ss << "__expr_inf_or3_" << count++;
            
            Or<3, Technology::Virtual> * or1 =
                            new Or<3, Technology::Virtual>(orOutput, orInput, ss.str());
            Module::ScratchPad.AddChildModule(*or1);
            
            sigLine_p = orOutput.GetSigLineVec().at(0);
            
            break;
        }
        case 4: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 4> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            sigLineArr.at(2) = this->TermList.at(2).IsSig ?
                               this->TermList.at(2).SigLine_p :
                               &this->TermList.at(2).Expr_p->Synthesize();
            sigLineArr.at(3) = this->TermList.at(3).IsSig ?
                               this->TermList.at(3).SigLine_p :
                               &this->TermList.at(3).Expr_p->Synthesize();
            
            Signal<4> orInput(sigLineArr);
            Signal<1> orOutput;
            
            stringstream ss;
            ss << "__expr_inf_or4_" << count++;
            
            Or<4, Technology::Virtual> * or1 =
                            new Or<4, Technology::Virtual>(orOutput, orInput, ss.str());
            Module::ScratchPad.AddChildModule(*or1);
            
            sigLine_p = orOutput.GetSigLineVec().at(0);
            
            break;
        }
        default: {
            // The code shouldn't reach here
            throw GeneralException(__FILE__, __LINE__);
            
            break;
        }
    }
}

void Expression::SynthesizeXor(Node *& sigLine_p) const
{
    switch(this->TermList.size()){
        case 0: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
        case 1: {
            throw GeneralException(__FILE__, __LINE__);
            break;
        }
        case 2: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 2> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            
            Signal<2> xorInput(sigLineArr);
            Signal<1> xorOutput;
            
            stringstream ss;
            ss << "__expr_inf_xor2_" << count++;
            
            Xor<2, Technology::Virtual> * xor1 =
                            new Xor<2, Technology::Virtual>(xorOutput, xorInput, ss.str());
            Module::ScratchPad.AddChildModule(*xor1);
            
            sigLine_p = xorOutput.GetSigLineVec().at(0);
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            break;
        }
        case 3: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 3> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            sigLineArr.at(2) = this->TermList.at(2).IsSig ?
                               this->TermList.at(2).SigLine_p :
                               &this->TermList.at(2).Expr_p->Synthesize();
            
            Signal<3> xorInput(sigLineArr);
            Signal<1> xorOutput;
            
            stringstream ss;
            ss << "__expr_inf_xor3_" << count++;
            
            Xor<3, Technology::Virtual> * xor1 =
                            new Xor<3, Technology::Virtual>(xorOutput, xorInput, ss.str());
            Module::ScratchPad.AddChildModule(*xor1);
            
            sigLine_p = xorOutput.GetSigLineVec().at(0);
            
            break;
        }
        case 4: {
            static uint64_t count = 0;
            
//            printf("%s:%d here\n", __FILE__, __LINE__);
            
            array<Node *, 4> sigLineArr;
            sigLineArr.at(0) = this->TermList.at(0).IsSig ?
                               this->TermList.at(0).SigLine_p :
                               &this->TermList.at(0).Expr_p->Synthesize();
            sigLineArr.at(1) = this->TermList.at(1).IsSig ?
                               this->TermList.at(1).SigLine_p :
                               &this->TermList.at(1).Expr_p->Synthesize();
            sigLineArr.at(2) = this->TermList.at(2).IsSig ?
                               this->TermList.at(2).SigLine_p :
                               &this->TermList.at(2).Expr_p->Synthesize();
            sigLineArr.at(3) = this->TermList.at(3).IsSig ?
                               this->TermList.at(3).SigLine_p :
                               &this->TermList.at(3).Expr_p->Synthesize();
            
            Signal<4> xorInput(sigLineArr);
            Signal<1> xorOutput;
            
            stringstream ss;
            ss << "__expr_inf_xor4_" << count++;
            
            Xor<4, Technology::Virtual> * xor1 =
                            new Xor<4, Technology::Virtual>(xorOutput, xorInput, ss.str());
            Module::ScratchPad.AddChildModule(*xor1);
            
            sigLine_p = xorOutput.GetSigLineVec().at(0);
            
            break;
        }
        default: {
            // The code shouldn't reach here
            throw GeneralException(__FILE__, __LINE__);
            
            break;
        }
    }
}

void Expression::SynthesizeFlop(Node *& sigLine_p) const
{
    if(this->TermList.size() != 2){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    static uint64_t count = 0;
    
//            printf("%s:%d here\n", __FILE__, __LINE__);
    
    array<Node *, 1> sigLineArrD;
    sigLineArrD.at(0) = this->TermList.at(0).IsSig ?
                        this->TermList.at(0).SigLine_p :
                        &this->TermList.at(0).Expr_p->Synthesize();
    array<Node *, 1> sigLineArrC;
    sigLineArrC.at(0) = this->TermList.at(1).IsSig ?
                        this->TermList.at(1).SigLine_p :
                        &this->TermList.at(1).Expr_p->Synthesize();
    
    Signal<1> dffD(sigLineArrD);
    Signal<1> dffClock(sigLineArrC);
    Signal<1> dffQ;
    
    stringstream ss;
    ss << "__expr_inf_dff_" << count++;
    
    DFlipFlop<Technology::Virtual> * dff1 = 
                    new DFlipFlop<Technology::Virtual>(dffQ, dffClock, dffD,
                                                       true, ss.str());
    Module::ScratchPad.AddChildModule(*dff1);
    
    sigLine_p = dffQ.GetSigLineVec().at(0);
    
//            printf("%s:%d here\n", __FILE__, __LINE__);
}

void Expression::SynthesizeTris(Node *& sigLine_p) const
{
    if(this->TermList.size() != 2){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    static uint64_t count = 0;
    
//            printf("%s:%d here\n", __FILE__, __LINE__);
    
    array<Node *, 1> sigLineArrA;
    sigLineArrA.at(0) = this->TermList.at(0).IsSig ?
                        this->TermList.at(0).SigLine_p :
                        &this->TermList.at(0).Expr_p->Synthesize();
    array<Node *, 1> sigLineArrS;
    sigLineArrS.at(0) = this->TermList.at(1).IsSig ?
                        this->TermList.at(1).SigLine_p :
                        &this->TermList.at(1).Expr_p->Synthesize();
    
    Signal<1> trisA(sigLineArrA);
    Signal<1> trisS(sigLineArrS);
    Signal<1> trisY;
    
    stringstream ss;
    ss << "__expr_inf_tris_" << count++;
    
    Tris<Technology::Virtual> * tris1 = 
                    new Tris<Technology::Virtual>(trisY, trisS, trisA, ss.str());
    Module::ScratchPad.AddChildModule(*tris1);
    
    sigLine_p = trisY.GetSigLineVec().at(0);
    
//            printf("%s:%d here\n", __FILE__, __LINE__);
}

void Expression::Dump(unsigned int w) const
{
    printf("%*s{\n", w*4, "");
    printf("%*sid=%d, func=%d, inv=%d, size=%lu\n", (w+1)*4, "",
           this->_Id, (int) this->Func, (int) this->Invert, this->TermList.size());
    for(unsigned int i = 0 ; i < this->TermList.size() ; i++){
        if(this->TermList.at(i).IsSig){
            printf("%*sTerm[%d]: sigLine=%lX\n", (w+1)*4, "", i, (size_t) this->TermList.at(i).SigLine_p);
        }else{
            printf("%*sTerm[%d]:\n", (w+1)*4, "", i);
            this->TermList.at(i).Expr_p->Dump(w+1);
        }
    }
    printf("%*s}\n", w*4, "");
}

}   // namespace Synth

}   // namespace Flambeau
