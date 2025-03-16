/**
 *  expression.h
 *  
 *  Created on: Jan 12, 2013
 *      Author: nav
 *  
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#ifndef __cplusplus
#error Must use C++ for expression.h
#endif

#include "flambeau/signal.h"

namespace Flambeau
{

namespace Synth
{

enum class ExprFunc
{
    None,
    Pass,
    And,
    Or,
    Xor,
    Add,
    Sub,
    Wire,
    Tris,
    Flop
};

class Expression
{
public:
    struct Term
    {
    public:
        bool    IsSig;
        union {
            Node *           SigLine_p;
            const Expression *  Expr_p;
        };
    public:
        Term (Node & sig);
        Term (const Expression & expr);
        Term (const Term & term);
        ~Term();
    };
private:
    static int          _IdCount;
    static int          _AliveCount;
    
    int                 _Id;
    
    ExprFunc            Func;
    bool                Invert;
    
    vector<Term>        TermList;
private:
    void                SynthesizePass  (Node *& sigLine_p) const;
    void                SynthesizeAnd   (Node *& sigLine_p) const;
    void                SynthesizeOr    (Node *& sigLine_p) const;
    void                SynthesizeXor   (Node *& sigLine_p) const;
    void                SynthesizeFlop  (Node *& sigLine_p) const;
    void                SynthesizeTris  (Node *& sigLine_p) const;
public:
    static Expression * Create  (ExprFunc op, bool invert);
    static Expression * Create  (const Expression & expr);
    static void         Destroy (Expression * expr);
private:
    Expression (ExprFunc op, bool invert);
    Expression (const Expression & expr) = delete;
    ~Expression();
public:
//    operator Node() const;
    
    void                SetFunc         (ExprFunc op);
    ExprFunc            GetFunc         (void) const;
    
    void                SetInvert       (bool invert);
    bool                HasInvert       (void) const;
    
    const vector<Term>& GetTermList     (void) const;
    
    Expression &        operator <<     (Node & sigLine);
    Expression &        operator <<     (const Expression & expr);
    
    Node &              Synthesize      (void) const;
    
    void Dump(unsigned int w) const;
};

inline ExprFunc Expression::GetFunc(void) const
{
    return this->Func;
}

inline bool Expression::HasInvert(void) const
{
    return this->Invert;
}

inline const vector<Expression::Term> & Expression::GetTermList(void) const
{
    return this->TermList;
}

//---------------------------------------------------------------------------//

template <unsigned int Width>
class ExpressionVec
{
//private:
//    ExpressionVec(const ExpressionVec &);
//    ExpressionVec & operator = (const ExpressionVec &);
private:
    array<Expression *, Width>  ExprArr;
public:
    ExpressionVec (const Signal<Width> & sig);
    ExpressionVec (const array<Expression *, Width> & exprArr);
    ~ExpressionVec();
    
    operator Signal<Width> () const;
    
    array<Expression *, Width> &        GetExprArray(void);
    const array<Expression *, Width> &  GetExprArray(void) const;
    
    Signal<Width>   Synthesize  (void) const;
    
    void Dumpp(void) const;
};

template <unsigned int Width>
ExpressionVec<Width>::ExpressionVec(const Signal<Width> & sig)
{
    const array<Node *, Width> & sigLineVec = sig.GetSigLineVec();
    
//    printf("%s:%d Width = %u\n", __FILE__, __LINE__, Width);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->ExprArr.at(i) = Expression::Create(ExprFunc::Pass, false);
        
        *this->ExprArr.at(i) << *sigLineVec.at(i);
    }
}

template <unsigned int Width>
ExpressionVec<Width>::ExpressionVec(const array<Expression *, Width> & exprArr)
{
//    printf("%s:%d Width = %u\n", __FILE__, __LINE__, Width);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->ExprArr.at(i) = exprArr.at(i);
    }
}

template <unsigned int Width>
ExpressionVec<Width>::~ExpressionVec()
{
//    printf("%s:%d Width = %u\n", __FILE__, __LINE__, Width);
    
    
}

template <unsigned int Width>
inline ExpressionVec<Width>::operator Signal<Width> () const
{
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return this->Synthesize();
}

template <unsigned int Width>
inline array<Expression *, Width> & ExpressionVec<Width>::GetExprArray(void)
{
    return this->ExprArr;
}

template <unsigned int Width>
inline const array<Expression *, Width> & ExpressionVec<Width>::GetExprArray(void) const
{
    return this->ExprArr;
}

template <unsigned int Width>
Signal<Width> ExpressionVec<Width>::Synthesize(void) const
{
    array<Node *, Width> synthSiglineList;
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    for(unsigned int i = 0 ; i < Width ; i++){
//        printf("%s:%d here\n", __FILE__, __LINE__);
//        this->ExprArr.at(i)->Dump(0);
        
        synthSiglineList.at(i) = &this->ExprArr.at(i)->Synthesize();
//        printf("%s:%d synthSiglineList.at(i)=%lX\n", __FILE__, __LINE__, (size_t) synthSiglineList.at(i));
    }
    
//    printf("%s:%d here\n", __FILE__, __LINE__);
    
    return Signal<Width>(synthSiglineList);
}

template <unsigned int Width>
void ExpressionVec<Width>::Dumpp() const
{
    printf("begindumpp\n");
    for(unsigned int i = 0 ; i < Width ; i++){
        this->ExprArr.at(i)->Dump(0);
    }
    printf("enddumpp\n");
}

}   // namespace Synth

}   // namespace Flambeau

#endif /* EXPRESSION_H_ */
