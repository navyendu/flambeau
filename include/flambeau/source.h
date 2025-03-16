/**
 *  source.h
 *  
 *  Created on: Nov 11, 2012
 *      Author: nav
 *  
 */

#ifndef SOURCE_H_
#define SOURCE_H_

#ifndef __cplusplus
#error Must use C++ for source.h
#endif

#include "flambeau/sourcebase.h"
#include "flambeau/common.h"
#include "flambeau/logic.h"
#include "flambeau/signal.h"
#include "flambeau/ctrl.h"

namespace Flambeau
{

class Link
{
private:
    uint64_t        Resistance;
    bool            Connected;
    
};

template <unsigned int Width>
class Source : public SourceBase
{
private:
    Source (const Source<Width> &);
    Source<Width> & operator = (const Source<Width> &);
protected:
    LogicVec<Width>         Value;
    array<uint64_t, Width>  Resistance;
    array<Node *, Width>    SigLines;
public:
    Source (const Signal<Width> & feedSig, uint64_t resistance = 0);
    Source (const Signal<Width> & feedSig, const uint64_t * resistanceVec_a);
    Source (const Signal<Width> & feedSig, const array<uint64_t, Width> & resistanceVec);
    ~Source();
    
    void                    ApplyValue      (const Logic * value_a);
    
    void                    SetValue        (Logic value) const;
    void                    SetValue        (const LogicVec<Width> & value) const;
    const LogicVec<Width> & GetValue        (void) const;
    
    void                    SetResistance   (uint64_t res);
    void                    SetResistance   (const uint64_t * resVec_a);
    void                    SetResistance   (const array<uint64_t, Width> & resVec);
    const array<uint64_t,Width> &
                            GetResistance   (void) const;
    
//    Signal<Width> &         GetSignal       (void);
//    const Signal<Width> &   GetSignal       (void) const;
};

template <unsigned int Width>
Source<Width>::Source(const Signal<Width> & feedSig, uint64_t resistance)
    : Value      (),
      Resistance (),
      SigLines   (feedSig.GetSigLineVec())
{
    printf("S = %lX, feedSig = %lX (%lu)\n", (size_t) this, (size_t) &feedSig, feedSig.GetId());
    printf("Created Source: { ");
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->SigLines.at(i)->AddSourceValue(&this->Value[i]);
        printf("%lX, ", (size_t) this->SigLines.at(i));
    }
    printf("}\n");
    
    this->SetResistance(resistance);
}

template <unsigned int Width>
Source<Width>::Source(const Signal<Width> & feedSig, const uint64_t * resistanceVec_a)
    : Value      (),
      Resistance (),
      SigLines   (feedSig.GetSigLineVec())
{
    printf("S = %lX, feedSig = %lX (%lu)\n", (size_t) this, (size_t) &feedSig, feedSig.GetId());
    printf("Created Source: { ");
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->SigLines.at(i)->AddSourceValue(&this->Value[i]);
        printf("%lX, ", (size_t) this->SigLines.at(i));
    }
    printf("}\n");
    
    this->SetResistance(resistanceVec_a);
}

template <unsigned int Width>
Source<Width>::Source(const Signal<Width> & feedSig, const array<uint64_t, Width> & resistanceVec)
    : Value      (),
      Resistance (),
      SigLines   (feedSig.GetSigLineVec())
{
    printf("S = %lX, feedSig = %lX (%lu)\n", (size_t) this, (size_t) &feedSig, feedSig.GetId());
    printf("Created Source: { ");
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->SigLines.at(i)->AddSourceValue(&this->Value[i]);
        printf("%lX, ", (size_t) this->SigLines.at(i));
    }
    printf("}\n");
    
    this->SetResistance(resistanceVec);
}

template <unsigned int Width>
Source<Width>::~Source()
{
    
}

//        printf("%s:%d Delay = %lu * %lu = %lu\n", __FILE__, __LINE__, 
//               this->Resistance.at(i), this->SigLines.at(i).GetLoadCap(), delay);

template <unsigned int Width>
void Source<Width>::SetValue(Logic value) const
{
    this->SetValue(LogicVec<Width>(value));
}

template <unsigned int Width>
void Source<Width>::SetValue(const LogicVec<Width> & newValue) const
{
//    array<bool, Width> deltaVec;
//    
//    bool changed = false;
//    for(unsigned int i = 0 ; i < Width ; i++){
//        deltaVec.at(i) = (newValue[i] != this->Value[i]);
//        
//        if(deltaVec.at(i)){
//            changed = true;
//        }
//    }
//    
//    printf("%s:%d %s: CurValue = %s, NewValue = %s, DeltaVec = ", __FILE__, __LINE__, 
//           this->Name.c_str(), this->Value.ToString().c_str(), newValue.ToString().c_str());
//    for(unsigned int i = 0 ; i < Width ; i++){
//        printf("%c", deltaVec.at(i) ? '1' : '0');
//    }
//    printf("\n");
//    
//    if(!changed){
//        return;
//    }
//    
    
//    printf("%s:%d: @%lu: %lX (%s): NewVal = %s\n", __FILE__, __LINE__,
//           SimCtrl::GetTime(), (size_t) this, this->Name.c_str(),
//           newValue.ToString().c_str());
    
    array<TSimTime, Width> delayVec;
    vector<TSimTime> uniqueDelayVec;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        delayVec.at(i) = this->Resistance.at(i) * this->SigLines.at(i)->GetLoadCap();
        
        InsertIfAbsent(uniqueDelayVec, delayVec.at(i));
    }
    
    // Sort uniqueDelayVec. Use a faster algorithm later
    for(unsigned int i = 0 ; i < uniqueDelayVec.size() - 1 ; i++){
        for(unsigned int j = i + 1 ; j < uniqueDelayVec.size() ; j++){
            if(uniqueDelayVec.at(i) > uniqueDelayVec.at(j)){
                TSimTime temp = uniqueDelayVec.at(i);
                uniqueDelayVec.at(i) = uniqueDelayVec.at(j);
                uniqueDelayVec.at(j) = temp;
            }
        }
    }
    
    LogicVec<Width> transVec = this->Value;
    
    for(unsigned int i = 0 ; i < uniqueDelayVec.size() ; i++){
        for(unsigned int j = 0 ; j < Width ; j++){
            if(delayVec.at(j) == uniqueDelayVec.at(i)){
                transVec[j] = newValue[j];
            }
        }
        
        Logic * value_a = new Logic[Width];
        for(unsigned int j = 0 ; j < Width ; j++){
            value_a[j] = transVec[j];
        }
        
//        printf("%s:%d: @%lu: Queuing up %lX (%s) == ", __FILE__, __LINE__,
//               SimCtrl::GetTime(), (size_t) this, this->Name.c_str());
//        for(unsigned int j = 0 ; j < Width ; j++){
//            printf("%c", value_a[j].ToString());
//        }
//        printf(" (%s) @ %ld\n", transVec.ToString().c_str(), 
//               SimCtrl::GetTime() + uniqueDelayVec.at(i));
        
        SimCtrl::QueueUpdateSource(*this, value_a, uniqueDelayVec.at(i));
    }
}

template <unsigned int Width>
void Source<Width>::ApplyValue(const Logic * value_a)
{
    LogicVec<Width> oldValue = this->Value;
    
    this->Value = LogicVec<Width>(value_a);
    
//    printf("%s:%d @%lu: %lX (%s) = %s to %s\n", __FILE__, __LINE__,
//           SimCtrl::GetTime(), (size_t) this, this->Name.c_str(),
//           oldValue.ToString().c_str(), this->Value.ToString().c_str());
//    fflush(stdout);
    
    vector<const BusListener *> busListenerList;
    vector<const SignalBase *> containerList;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        if(oldValue[i] != this->Value[i]){          // Try commenting this out later
            bool changedAfterResolve = this->SigLines.at(i)->ResolveValue(true);
            
            if(changedAfterResolve){
                const vector<const SignalBase *> & lineContList = this->SigLines.at(i)->GetContainerList();
                
                for(unsigned int j = 0 ; j < lineContList.size() ; j++){
//                    printf("%s:%d InsertIfAbsent %lX <- %lX\n", __FILE__, __LINE__,
//                           (size_t) &containerList, (size_t) lineContList.at(j));
//                    fflush(stdout);
                    
                    InsertIfAbsent(containerList, lineContList.at(j));
                }
            }
        }
    }
    
//    printf("%s:%d\n", __FILE__, __LINE__);
//    fflush(stdout);
    
    for(unsigned int i = 0 ; i < containerList.size() ; i++){
        const vector<const BusListener *> & contBuslstnList = containerList.at(i)->GetBusListenerList();
        
        for(unsigned int j = 0 ; j < contBuslstnList.size() ; j++){
            InsertIfAbsent(busListenerList, contBuslstnList.at(j));
        }
    }
    
    for(unsigned int i = 0 ; i < busListenerList.size() ; i++){
        busListenerList.at(i)->Trigger();
    }
    
    for(unsigned int i = 0 ; i < containerList.size() ; i++){
//        printf("LOG: @%lu %s (%lu) = %s\n", SimCtrl::GetTime(), 
//               containerList.at(i)->GetName().c_str(),
//               containerList.at(i)->GetId(),
//               containerList.at(i)->ToString().c_str());
        SimCtrl::DumpMan_p->Dump(*containerList.at(i), SimCtrl::GetTime());
    }
}

template <unsigned int Width>
const LogicVec<Width> & Source<Width>::GetValue(void) const
{
    return this->Value;
}

template <unsigned int Width>
void Source<Width>::SetResistance(uint64_t res)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Resistance.at(i) = res;
    }
}

template <unsigned int Width>
void Source<Width>::SetResistance(const uint64_t * resVec_a)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Resistance.at(i) = resVec_a[i];
    }
}

template <unsigned int Width>
void Source<Width>::SetResistance(const array<uint64_t, Width> & resVec)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->Resistance.at(i) = resVec.at(i);
    }
}

template <unsigned int Width>
const array<uint64_t,Width> & Source<Width>::GetResistance(void) const
{
    return this->Resistance;
}

//template <unsigned int Width>
//Signal<Width> & Source<Width>::GetSignal(void)
//{
//    return this->SigLines;
//}
//
//template <unsigned int Width>
//const Signal<Width> & Source<Width>::GetSignal(void) const
//{
//    return this->SigLines;
//}

}   // namespace Flambeau

#endif /* SOURCE_H_ */
