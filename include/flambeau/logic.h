/**
 *  logic.h
 *  
 *  Created on: Oct 27, 2012
 *      Author: nav
 *  
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#ifndef __cplusplus
#error Must use C++ for logic.h
#endif

#include "flambeau/common.h"

namespace Flambeau
{

class Logic
{
public:
    static const unsigned int   Count = 4;
private:
    static const Logic  AndLookup   [Logic::Count][Logic::Count];
    static const Logic  OrLookup    [Logic::Count][Logic::Count];
    static const Logic  XorLookup   [Logic::Count][Logic::Count];
    static const Logic  NotLookup   [Logic::Count];
    static const Logic  DrvLookup   [Logic::Count][Logic::Count];
    static const Logic  TriLookup   [Logic::Count][Logic::Count];
    static const char   StrLookup   [Logic::Count];
public:
    static const Logic  L;
    static const Logic  H;
    static const Logic  X;
    static const Logic  Z;
//public:
//    template <unsigned int Width>
//    static uint64_t             ToInt   (const array<Logic, Width> & logicVec);
//    
//    template <unsigned int Width>
//    static array<Logic, Width>  ToVec   (uint64_t numVec);
private:
    Logic (uint8_t value);
private:
    uint8_t Value;
public:
    Logic ();
    Logic (const Logic & a);
    ~Logic();
    
    Logic &     operator =  (const Logic & a);
    
    Logic       operator ~  (void) const;
    Logic       operator &  (Logic a) const;
    Logic       operator |  (Logic a) const;
    Logic       operator ^  (Logic a) const;
    
    Logic       operator /  (Logic a) const;    // Tristate control
    Logic       operator *  (Logic a) const;    // Multiple drive
    
    bool        operator == (Logic a) const;
    bool        operator != (Logic a) const;
    
    char        ToString    (void) const;
    int         GetValue    (void) const;
};

inline Logic & Logic::operator = (const Logic & a)
{
    this->Value = a.Value;
    
    return *this;
}

inline Logic Logic::operator ~ (void) const
{
    return Logic::NotLookup[this->Value];
}

inline Logic Logic::operator & (Logic a) const
{
    return Logic::AndLookup[this->Value][a.Value];
}

inline Logic Logic::operator | (Logic a) const
{
    return Logic::OrLookup[this->Value][a.Value];
}

inline Logic Logic::operator ^ (Logic a) const
{
    return Logic::XorLookup[this->Value][a.Value];
}

inline Logic Logic::operator / (Logic a) const
{
    return Logic::TriLookup[this->Value][a.Value];
}

inline Logic Logic::operator * (Logic a) const
{
    return Logic::DrvLookup[this->Value][a.Value];
}

inline char Logic::ToString() const
{
    return Logic::StrLookup[this->Value];
}

inline int Logic::GetValue(void) const
{
    return this->Value;
}

inline bool Logic::operator == (Logic a) const
{
    return this->Value == a.Value;
}

inline bool Logic::operator != (Logic a) const
{
    return this->Value != a.Value;
}

//---------------------------------------------------------------------------//

template <unsigned int Width>
class LogicVec
{
private:
    array<Logic, Width> Vec;
public:
    LogicVec (Logic logic = Logic::X);
    LogicVec (uint64_t intVec);
    LogicVec (const array<uint64_t, (Width-1)/64 + 1> & intVecArr);
    LogicVec (const array<Logic, Width> & logicArr);
    LogicVec (const Logic * logicArr_a);
    LogicVec (const array<const Logic *, Width> & logicArr);
    LogicVec (const LogicVec<Width> & logicVec);
    ~LogicVec();
    
    LogicVec<Width> &   operator =  (Logic logic);
    LogicVec<Width> &   operator =  (uint64_t intVec);
    LogicVec<Width> &   operator =  (const array<uint64_t, (Width-1)/64 + 1> & intVecArr);
    LogicVec<Width> &   operator =  (const array<Logic, Width> & logicArr);
    LogicVec<Width> &   operator =  (const Logic * logicArr_a);
    LogicVec<Width> &   operator =  (const array<const Logic *, Width> & logicArr);
    LogicVec<Width> &   operator =  (const LogicVec<Width> & logicVec);
    
    Logic &             operator [] (unsigned int index);
    const Logic &       operator [] (unsigned int index) const;
    
    LogicVec<Width>     operator &  (const LogicVec<Width> & a) const;
    LogicVec<Width>     operator |  (const LogicVec<Width> & a) const;
    LogicVec<Width>     operator ^  (const LogicVec<Width> & a) const;
    LogicVec<Width>     operator ~  (void) const;
    
    LogicVec<Width>     operator /  (const LogicVec<Width> & a) const;
    LogicVec<Width>     operator *  (const LogicVec<Width> & a) const;
    
    bool                operator == (const LogicVec<Width> & a) const;
    bool                operator != (const LogicVec<Width> & a) const;
    
    string              ToString    (void) const;
    uint64_t            ToInt       (void) const;
    array<uint64_t,(Width-1)/64 + 1>
                        ToIntArr    (void) const;
    
    bool                HasX        (void) const;
    bool                HasZ        (void) const;
    bool                IsInt       (void) const;
};

template <unsigned int Width>
LogicVec<Width>::LogicVec(Logic logic)
    : Vec()
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logic;
    }
}

template <unsigned int Width>
LogicVec<Width>::LogicVec(uint64_t intVec)
    : Vec()
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = (intVec & 1) == 1 ? Logic::H : Logic::L;
        
        intVec >>= 1;
    }
}

template <unsigned int Width>
LogicVec<Width>::LogicVec(const array<uint64_t, (Width-1)/64 + 1> & intVecArr)
    : Vec()
{
    for(unsigned int j = 0 ; j < intVecArr.size() ; j++){
        for(unsigned int i = 0 ; i < Width ; i++){
            this->Vec.at(j * 64 + i) = (intVecArr.at(j) & 1) == 1 ? Logic::H : Logic::L;
            
            intVecArr.at(j) >>= 1;
        }
    }
}

template <unsigned int Width>
LogicVec<Width>::LogicVec(const array<Logic, Width> & logicArr)
    : Vec()
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logicArr.at(i);
    }
}

template <unsigned int Width>
LogicVec<Width>::LogicVec(const Logic * logicArr_a)
    : Vec()
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logicArr_a[i];
    }
}

template <unsigned int Width>
LogicVec<Width>::LogicVec(const array<const Logic *, Width> & logicArr)
    : Vec()
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = *logicArr.at(i);
    }
}

template <unsigned int Width>
LogicVec<Width>::LogicVec(const LogicVec<Width> & logicVec)
    : Vec()
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logicVec[i];
    }
}

template <unsigned int Width>
LogicVec<Width>::~LogicVec()
{
    
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (Logic logic)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logic;
    }
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (uint64_t intVec)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = (intVec & 1) == 1 ? Logic::H : Logic::L;
        
        intVec >>= 1;
    }
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (const array<uint64_t, (Width-1)/64 + 1> & intVecArr)
{
    for(unsigned int j = 0 ; j < intVecArr.size() ; j++){
        for(unsigned int i = 0 ; i < Width ; i++){
            this->Vec.at(j * 64 + i) = (intVecArr.at(j) & 1) == 1 ? Logic::H : Logic::L;
            
            intVecArr.at(j) >>= 1;
        }
    }
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (const array<Logic, Width> & logicArr)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logicArr.at(i);
    }
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (const Logic * logicArr_a)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logicArr_a[i];
    }
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (const array<const Logic *, Width> & logicArr)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = *logicArr.at(i);
    }
}

template <unsigned int Width>
LogicVec<Width> & LogicVec<Width>::operator = (const LogicVec<Width> & logicVec)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Vec.at(i) = logicVec[i];
    }
    
    return *this;
}

template <unsigned int Width>
inline Logic & LogicVec<Width>::operator [] (unsigned int index)
{
    return this->Vec.at(index);
}

template <unsigned int Width>
inline const Logic & LogicVec<Width>::operator [] (unsigned int index) const
{
    return this->Vec.at(index);
}

template <unsigned int Width>
LogicVec<Width> LogicVec<Width>::operator ~ (void) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.Vec.at(i) = ~this->Vec.at(i);
    }
    
    return result;
}

template <unsigned int Width>
LogicVec<Width> LogicVec<Width>::operator & (const LogicVec<Width> & a) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.Vec.at(i) = this->Vec.at(i) & a.Vec.at(i);
    }
    
    return result;
}

template <unsigned int Width>
LogicVec<Width> LogicVec<Width>::operator | (const LogicVec<Width> & a) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.Vec.at(i) = this->Vec.at(i) | a.Vec.at(i);
    }
    
    return result;
}

template <unsigned int Width>
LogicVec<Width> LogicVec<Width>::operator ^ (const LogicVec<Width> & a) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.Vec.at(i) = this->Vec.at(i) ^ a.Vec.at(i);
    }
    
    return result;
}

template <unsigned int Width>
LogicVec<Width> LogicVec<Width>::operator / (const LogicVec<Width> & a) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.Vec.at(i) = this->Vec.at(i) / a.Vec.at(i);
    }
    
    return result;
}

template <unsigned int Width>
LogicVec<Width> LogicVec<Width>::operator * (const LogicVec<Width> & a) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.Vec.at(i) = this->Vec.at(i) * a.Vec.at(i);
    }
    
    return result;
}

template <unsigned int Width>
bool LogicVec<Width>::operator == (const LogicVec<Width> & a) const
{
    for(unsigned int i = 0 ; i < Width ; i++){
        if(this->Vec.at(i) != a.Vec.at(i)){
            return false;
        }
    }
    
    return true;
}

template <unsigned int Width>
bool LogicVec<Width>::operator != (const LogicVec<Width> & a) const
{
    for(unsigned int i = 0 ; i < Width ; i++){
        if(this->Vec.at(i) != a.Vec.at(i)){
            return true;
        }
    }
    
    return false;
}

template <unsigned int Width>
string LogicVec<Width>::ToString(void) const
{
    string result(Width, '?');
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.at(Width - i - 1) = this->Vec.at(i).ToString();
    }
    
    return result;
}

template <unsigned int Width>
uint64_t LogicVec<Width>::ToInt(void) const
{
    uint64_t result;
    
    result = 0;
    for(unsigned int i = 0 ; i < Width ; i++){
        if(this->Vec.at(i) == Logic::H || this->Vec.at(i) == Logic::L){
            result |= (this->Vec.at(i) == Logic::H ? 1 : 0) << i;
        }else{
            throw GeneralException(__FILE__, __LINE__);
        }
    }
    
    return result;
}

template <unsigned int Width>
array<uint64_t,(Width-1)/64 + 1> LogicVec<Width>::ToIntArr(void) const
{
    array<uint64_t,(Width-1)/64 + 1> result;
    
    for(unsigned int j = 0 ; j < result.size() ; j++){
        result.at(j) = 0;
    }
    
    return result;
}

template <unsigned int Width>
bool LogicVec<Width>::HasX(void) const
{
    for(unsigned int i = 0 ; i < Width ; i++){
        if(this->Vec.at(i) == Logic::X){
            return true;
        }
    }
    
    return false;
}

template <unsigned int Width>
bool LogicVec<Width>::HasZ(void) const
{
    for(unsigned int i = 0 ; i < Width ; i++){
        if(this->Vec.at(i) == Logic::Z){
            return true;
        }
    }
    
    return false;
}

template <unsigned int Width>
bool LogicVec<Width>::IsInt(void) const
{
    for(unsigned int i = 0 ; i < Width ; i++){
        if(this->Vec.at(i) == Logic::X || this->Vec.at(i) == Logic::Z){
            return false;
        }
    }
    
    return true;
}

}   // namespace Flambeau

#endif /* LOGIC_H_ */
