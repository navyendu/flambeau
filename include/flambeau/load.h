/**
 *  load.h
 *  
 *  Created on: Nov 12, 2012
 *      Author: nav
 *  
 */

#ifndef LOAD_H_
#define LOAD_H_

#ifndef __cplusplus
#error Must use C++ for load.h
#endif

#include "flambeau/common.h"
#include "flambeau/logic.h"
#include "flambeau/signal.h"

namespace Flambeau
{

template <unsigned int Width>
class Load
{
private:
    Load (const Load<Width> &);
    Load<Width> & operator= (const Load<Width> &);
protected:
    array<uint64_t, Width>  Capacitance;
    array<Node *, Width>    SigLines;
public:
    Load (const Signal<Width> & attachedSig, uint64_t capacitance = 0);
    Load (const Signal<Width> & attachedSig, const uint64_t * capacitanceVec);
    Load (const Signal<Width> & attachedSig, const array<uint64_t, Width> & capacitanceVec);
    ~Load();
    
    void                    SetCapacitance  (uint64_t cap);
    void                    SetCapacitance  (const uint64_t * capVec);
    void                    SetCapacitance  (const array<uint64_t, Width> & capVec);
    const array<uint64_t, Width> &
                            GetCapacitance  (void) const;
};

template <unsigned int Width>
Load<Width>::Load(const Signal<Width> & attachedSig, uint64_t capacitance)
    : Capacitance (),
      SigLines    (attachedSig.GetSigLineVec())
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->SigLines.at(i)->AddLoadValue(&this->Capacitance.at(i));
    }
    
    this->SetCapacitance(capacitance);
}

template <unsigned int Width>
Load<Width>::Load(const Signal<Width> & attachedSig, const uint64_t * capacitanceVec_a)
    : Capacitance (),
      SigLines    (attachedSig.GetSigLineVec())
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->SigLines.at(i)->AddLoadValue(&this->Capacitance.at(i));
    }
    
    this->SetCapacitance(capacitanceVec_a);
}

template <unsigned int Width>
Load<Width>::Load(const Signal<Width> & attachedSig, const array<uint64_t, Width> & capacitanceVec)
    : Capacitance (),
      SigLines    (attachedSig.GetSigLineVec())
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->SigLines.at(i)->AddLoadValue(&this->Capacitance.at(i));
    }
    
    this->SetCapacitance(capacitanceVec);
}

template <unsigned int Width>
Load<Width>::~Load()
{
    
}

template <unsigned int Width>
void Load<Width>::SetCapacitance(uint64_t cap)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Capacitance.at(i) = cap;
        this->SigLines.at(i)->ResolveCap();
    }
}

template <unsigned int Width>
void Load<Width>::SetCapacitance(const uint64_t * capVec_a)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Capacitance.at(i) = capVec_a[i];
        this->SigLines.at(i)->ResolveCap();
    }
}

template <unsigned int Width>
void Load<Width>::SetCapacitance(const array<uint64_t, Width> & capVec)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Capacitance.at(i) = capVec.at(i);
        this->SigLines.at(i)->ResolveCap();
    }
}

template <unsigned int Width>
inline const array<uint64_t, Width> & Load<Width>::GetCapacitance(void) const
{
    return this->Capacitance;
}

//template <unsigned int Width>
//inline Signal<Width> & Load<Width>::GetSignal(void)
//{
//    return this->AttachedSig;
//}
//
//template <unsigned int Width>
//inline const Signal<Width> & Load<Width>::GetSignal(void) const
//{
//    return this->AttachedSig;
//}

}   // namespace Flambeau

#endif /* LOAD_H_ */
