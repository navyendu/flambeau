/**
 *  datapath.h
 *  
 *  Created on: Nov 15, 2012
 *      Author: nav
 *  
 */

#ifndef DATAPATH_H_
#define DATAPATH_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/synth/datapath.h
#endif

#include <stdio.h>

#include "flambeau/expression.h"

//#include "flambeau/library/cells/not.h"
//#include "flambeau/library/cells/and.h"
//#include "flambeau/library/cells/or.h"
//#include "flambeau/library/func/cells/xor.h"
//#include "flambeau/library/func/cells/nand.h"
//#include "flambeau/library/func/cells/nor.h"
//#include "flambeau/library/func/cells/xnor.h"

namespace Flambeau
{

namespace Synth
{

template <unsigned int Width>
ExpressionVec<Width> operator ~ (const ExpressionVec<Width> & expVecA)
{
    array<Expression *, Width> exprArr = expVecA.GetExprArray();
    for(unsigned int i = 0 ; i < Width ; i++){
        exprArr.at(i)->SetInvert(!exprArr.at(i)->HasInvert());
    }
    
    return ExpressionVec<Width>(exprArr);
}

template <unsigned int Width>
inline ExpressionVec<Width> operator ~ (const Signal<Width> & sigA)
{
    ExpressionVec<Width> ret(sigA);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ~ret;
}

template <unsigned int Width>
ExpressionVec<Width> operator & (const ExpressionVec<Width> & expVecA, const ExpressionVec<Width> & expVecB)
{
    array<Expression *, Width> exprArr;
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        exprArr.at(i) = Expression::Create(ExprFunc::And, false);
        
//        if(expVecA.GetExprArray().at(i).GetTermList().size() +
//           expVecB.GetExprArray().at(i).GetTermList().size() < 4)
//        {
//            const vector<Expression::Term> & tl = expVecA.GetExprArray().at(i).GetTermList();
//            exprArr.at(i)
//        }else{
            *exprArr.at(i) << *expVecA.GetExprArray().at(i);
            *exprArr.at(i) << *expVecB.GetExprArray().at(i);
//        }
    }
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret(exprArr);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator & (const Signal<Width> & sigA, const ExpressionVec<Width> & expVecB)
{
    return ExpressionVec<Width>(sigA) & expVecB;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator & (const ExpressionVec<Width> & expVecA, const Signal<Width> & sigB)
{
    return expVecA & ExpressionVec<Width>(sigB);
}

template <unsigned int Width>
inline ExpressionVec<Width> operator & (const Signal<Width> & sigA, const Signal<Width> & sigB)
{
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret = ExpressionVec<Width>(sigA) & ExpressionVec<Width>(sigB);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
ExpressionVec<Width> operator | (const ExpressionVec<Width> & expVecA, const ExpressionVec<Width> & expVecB)
{
    array<Expression *, Width> exprArr;
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        exprArr.at(i) = Expression::Create(ExprFunc::Or, false);
        
//        if(expVecA.GetExprArray().at(i).GetTermList().size() +
//           expVecB.GetExprArray().at(i).GetTermList().size() < 4)
//        {
//            const vector<Expression::Term> & tl = expVecA.GetExprArray().at(i).GetTermList();
//            exprArr.at(i)
//        }else{
            *exprArr.at(i) << *expVecA.GetExprArray().at(i);
            *exprArr.at(i) << *expVecB.GetExprArray().at(i);
//        }
    }
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret(exprArr);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator | (const Signal<Width> & sigA, const ExpressionVec<Width> & expVecB)
{
    return ExpressionVec<Width>(sigA) | expVecB;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator | (const ExpressionVec<Width> & expVecA, const Signal<Width> & sigB)
{
    return expVecA | ExpressionVec<Width>(sigB);
}

template <unsigned int Width>
inline ExpressionVec<Width> operator | (const Signal<Width> & sigA, const Signal<Width> & sigB)
{
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret = ExpressionVec<Width>(sigA) | ExpressionVec<Width>(sigB);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
ExpressionVec<Width> operator ^ (const ExpressionVec<Width> & expVecA, const ExpressionVec<Width> & expVecB)
{
    array<Expression *, Width> exprArr;
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        exprArr.at(i) = Expression::Create(ExprFunc::Xor, false);
        
//        if(expVecA.GetExprArray().at(i).GetTermList().size() +
//           expVecB.GetExprArray().at(i).GetTermList().size() < 4)
//        {
//            const vector<Expression::Term> & tl = expVecA.GetExprArray().at(i).GetTermList();
//            exprArr.at(i)
//        }else{
            *exprArr.at(i) << *expVecA.GetExprArray().at(i);
            *exprArr.at(i) << *expVecB.GetExprArray().at(i);
//        }
    }
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret(exprArr);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator ^ (const Signal<Width> & sigA, const ExpressionVec<Width> & expVecB)
{
    return ExpressionVec<Width>(sigA) ^ expVecB;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator ^ (const ExpressionVec<Width> & expVecA, const Signal<Width> & sigB)
{
    return expVecA ^ ExpressionVec<Width>(sigB);
}

template <unsigned int Width>
inline ExpressionVec<Width> operator ^ (const Signal<Width> & sigA, const Signal<Width> & sigB)
{
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret = ExpressionVec<Width>(sigA) ^ ExpressionVec<Width>(sigB);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

// Flip-Flop array
template <unsigned int Width>
ExpressionVec<Width> operator % (const ExpressionVec<Width> & expVecA, const ExpressionVec<Width> & expVecB)
{
    array<Expression *, Width> exprArr;
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        exprArr.at(i) = Expression::Create(ExprFunc::Flop, false);
        
//        if(expVecA.GetExprArray().at(i).GetTermList().size() +
//           expVecB.GetExprArray().at(i).GetTermList().size() < 4)
//        {
//            const vector<Expression::Term> & tl = expVecA.GetExprArray().at(i).GetTermList();
//            exprArr.at(i)
//        }else{
            *exprArr.at(i) << *expVecA.GetExprArray().at(i);
            *exprArr.at(i) << *expVecB.GetExprArray().at(i);
//        }
    }
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret(exprArr);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator % (const Signal<Width> & sigA, const ExpressionVec<Width> & expVecB)
{
    return ExpressionVec<Width>(sigA) % expVecB;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator % (const ExpressionVec<Width> & expVecA, const Signal<Width> & sigB)
{
    return expVecA % ExpressionVec<Width>(sigB);
}

template <unsigned int Width>
inline ExpressionVec<Width> operator % (const Signal<Width> & sigA, const Signal<Width> & sigB)
{
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret = ExpressionVec<Width>(sigA) % ExpressionVec<Width>(sigB);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

// Flip-Flop array
template <unsigned int Width>
ExpressionVec<Width> operator / (const ExpressionVec<Width> & expVecA, const ExpressionVec<Width> & expVecB)
{
    array<Expression *, Width> exprArr;
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        exprArr.at(i) = Expression::Create(ExprFunc::Tris, false);
        
//        if(expVecA.GetExprArray().at(i).GetTermList().size() +
//           expVecB.GetExprArray().at(i).GetTermList().size() < 4)
//        {
//            const vector<Expression::Term> & tl = expVecA.GetExprArray().at(i).GetTermList();
//            exprArr.at(i)
//        }else{
            *exprArr.at(i) << *expVecA.GetExprArray().at(i);
            *exprArr.at(i) << *expVecB.GetExprArray().at(i);
//        }
    }
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret(exprArr);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator / (const Signal<Width> & sigA, const ExpressionVec<Width> & expVecB)
{
    return ExpressionVec<Width>(sigA) / expVecB;
}

template <unsigned int Width>
inline ExpressionVec<Width> operator / (const ExpressionVec<Width> & expVecA, const Signal<Width> & sigB)
{
    return expVecA / ExpressionVec<Width>(sigB);
}

template <unsigned int Width>
inline ExpressionVec<Width> operator / (const Signal<Width> & sigA, const Signal<Width> & sigB)
{
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    ExpressionVec<Width> ret = ExpressionVec<Width>(sigA) / ExpressionVec<Width>(sigB);
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
//    ret.Dumpp();
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return ret;
}


//template <unsigned int Width>
//Signal<Width+1> & operator + (Signal<Width> & a, Signal<Width> & b)
//{
//    static uint64_t count = 0;
//    
//    Signal<Width+1> & result = *new Signal<Width + 1>;
//    Signal<Width> & interConn = *new Signal<Width>;
//    array<FullAdder*, Width> & fullAdderVector = *new array<FullAdder*, Width>;
//    Source & ci0Source = *new Source(interConn[0]);
//    stringstream ss;
//    
//    ci0Source.Set(Logic::Lo);
//    
//    printf("Inferred 1 %u-bit ADDER\n", Width);
//    
//    for(unsigned int i = 0 ; i < Width - 1 ; i++){
//        fullAdderVector.at(i) = new FullAdder(a[i], b[i], interConn[i], result[i], interConn[i+1]);
//    }
//    fullAdderVector.at(Width - 1) = new FullAdder(a[Width - 1], b[Width - 1], interConn[Width - 1], 
//                                                  result[Width - 1], result[Width]);
//    
//    ss << "sig_fa_res_" << Width+1 << "_" << count;
//    result.SetName(ss.str());
//    
//    ss.seekp(0);
//    ss << "sig_fa_cico_" << Width << "_" << count++;
//    interConn.SetName(ss.str());
//    
//    return result;
//}

}   // namespace Synth

}   // namespace Flambeau

#endif /* DATAPATH_H_ */
