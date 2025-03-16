/**
 *  io.h
 *  
 *  Created on: Nov 20, 2012
 *      Author: nav
 *  
 */

#ifndef INPUT_H_
#define INPUT_H_

#ifndef __cplusplus
#error Must use C++ for input.h
#endif

#include "flambeau/load.h"
#include "flambeau/listener.h"

namespace Flambeau
{

template <unsigned int Width>
class Input
{
private:
    Input (const Input<Width> &);
    Input<Width> & operator = (const Input<Width> &);
private:
    Signal<Width>   Sig;
    Load<Width>     Ld;
    
    BusListener     Listener;
public:
    Input (const Signal<Width> & attachedSig, TCapacitance capacitance = 0,
           const string & name = "");
    Input (const Signal<Width> & attachedSig, TCapacitance capacitance = 0,
           Module * handlerMod = nullptr, BusListener::THandler handlerMethod = nullptr,
           const string & name = "");
    Input (const Signal<Width> & attachedSig, const TCapacitance * capVec_a,
           Module * handlerMod = nullptr, BusListener::THandler handlerMethod = nullptr,
           const string & name = "");
    Input (const Signal<Width> & attachedSig, const array<TCapacitance, Width> & capVec,
           Module * handlerMod = nullptr, BusListener::THandler handlerMethod = nullptr,
           const string & name = "");
    ~Input();
    
    LogicVec<Width>         GetValue        (void) const;
    
    Signal<Width> &         GetSignal       (void);
    const Signal<Width> &   GetSignal       (void) const;
    
    Load<Width> &           GetLoad         (void);
    const Load<Width> &     GetLoad         (void) const;
    
    BusListener &           GetListener     (void);
    const BusListener &     GetListener     (void) const;
    
    void                    SetupListener   (Module * mod, BusListener::THandler method);
};

template <unsigned int Width>
Input<Width>::Input(const Signal<Width> & attachedSig, TCapacitance capacitance,
                    const string & name)
    : Input(attachedSig, capacitance, nullptr, nullptr, name)
{}

template <unsigned int Width>
Input<Width>::Input(const Signal<Width> & attachedSig, TCapacitance capacitance,
                    Module * handlerMod, BusListener::THandler handlerMethod,
                    const string & name)
    : Sig       (attachedSig, name),
      Ld        (Sig, capacitance),
      Listener  (handlerMod, handlerMethod)
{
    this->Sig.AddBusListener(this->Listener);
    
    this->Listener.SetHandler(handlerMod, handlerMethod);
}

template <unsigned int Width>
Input<Width>::Input(const Signal<Width> & attachedSig, const TCapacitance * capVec_a,
                    Module * handlerMod, BusListener::THandler handlerMethod,
                    const string & name)
    : Sig       (attachedSig, name),
      Ld        (Sig, capVec_a),
      Listener  (handlerMod, handlerMethod)
{
    this->Sig.AddBusListener(this->Listener);
    
    this->Listener.SetHandler(handlerMod, handlerMethod);
}

template <unsigned int Width>
Input<Width>::Input(const Signal<Width> & attachedSig, const array<TCapacitance, Width> & capVec,
                    Module * handlerMod, BusListener::THandler handlerMethod,
                    const string & name)
    : Sig       (attachedSig, name),
      Ld        (Sig, capVec),
      Listener  (handlerMod, handlerMethod)
{
    this->Sig.AddBusListener(this->Listener);
    
    this->Listener.SetHandler(handlerMod, handlerMethod);
}

template <unsigned int Width>
Input<Width>::~Input()
{
    
}

template <unsigned int Width>
inline LogicVec<Width> Input<Width>::GetValue(void) const
{
    return this->Sig.GetValue();
}

template <unsigned int Width>
inline Signal<Width> & Input<Width>::GetSignal(void)
{
    return this->Sig;
}

template <unsigned int Width>
inline const Signal<Width> & Input<Width>::GetSignal(void) const
{
    return this->Sig;
}

template <unsigned int Width>
inline Load<Width> & Input<Width>::GetLoad(void)
{
    return this->Ld;
}

template <unsigned int Width>
inline const Load<Width> & Input<Width>::GetLoad(void) const
{
    return this->Ld;
}

template <unsigned int Width>
inline BusListener & Input<Width>::GetListener(void)
{
    return this->Listener;
}

template <unsigned int Width>
inline const BusListener & Input<Width>::GetListener(void) const
{
    return this->Listener;
}

template <unsigned int Width>
void Input<Width>::SetupListener(Module * mod, BusListener::THandler method)
{
    this->Listener.SetHandler(mod, method);
}

}   // namespace Flambeau

#endif /* INPUT_H_ */
