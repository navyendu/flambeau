/**
 *  output.h
 *  
 *  Created on: Nov 20, 2012
 *      Author: nav
 *  
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#ifndef __cplusplus
#error Must use C++ for output.h
#endif

#include "flambeau/source.h"
#include "flambeau/load.h"

namespace Flambeau
{

template <unsigned int Width>
class Output
{
private:
    Output (const Output<Width> &);
    Output<Width> & operator = (const Output<Width> &);
private:
    Signal<Width>   Sig;
    Load<Width>     Ld;         // Diffusion capacitance of internal transistors
    
    Source<Width>   Src;        // Resistance of internal transistors
public:
    Output (const Signal<Width> & feedSig, TCapacitance capacitance = 0, TResistance resistance = 0,
            const string & name = "");
    Output (const Signal<Width> & feedSig, const TCapacitance * capVec_a, const TResistance * resVec_a,
            const string & name = "");
    Output (const Signal<Width> & feedSig, 
            const array<TCapacitance, Width> & capVec, const array<TResistance, Width> & resVec,
            const string & name = "");
    ~Output();
    
    void                    operator =  (const LogicVec<Width> & valueVec);
    
    Signal<Width> &         GetSignal   (void);
    const Signal<Width> &   GetSignal   (void) const;
    
    Load<Width> &           GetLoad     (void);
    const Load<Width> &     GetLoad     (void) const;
    
    Source<Width> &         GetSource   (void);
    const Source<Width> &   GetSource   (void) const;
};

template <unsigned int Width>
Output<Width>::Output(const Signal<Width> & feedSig,
                      TCapacitance capacitance, TResistance resistance,
                      const string & name)
    : Sig   (feedSig, name),
      Ld    (Sig, capacitance),
      Src   (Sig, resistance)
{
    
}

template <unsigned int Width>
Output<Width>::Output(const Signal<Width> & feedSig,
                      const TCapacitance * capVec_a, const TResistance * resVec_a,
                      const string & name)
    : Sig   (feedSig, name),
      Ld    (Sig, capVec_a),
      Src   (Sig, resVec_a)
{
    
}

template <unsigned int Width>
Output<Width>::Output(const Signal<Width> & feedSig, 
                      const array<TCapacitance, Width> & capVec, const array<TResistance, Width> & resVec,
                      const string & name)
    : Sig   (feedSig, name),
      Ld    (Sig, capVec),
      Src   (Sig, resVec)
{
    
}

template <unsigned int Width>
Output<Width>::~Output()
{
    
}

template <unsigned int Width>
inline void Output<Width>::operator = (const LogicVec<Width> & valueVec)
{
    this->Src.SetValue(valueVec);
}

template <unsigned int Width>
inline Signal<Width> & Output<Width>::GetSignal(void)
{
    return this->Sig;
}

template <unsigned int Width>
inline const Signal<Width> & Output<Width>::GetSignal(void) const
{
    return this->Sig;
}

template <unsigned int Width>
inline Load<Width> & Output<Width>::GetLoad(void)
{
    return this->Ld;
}

template <unsigned int Width>
inline const Load<Width> & Output<Width>::GetLoad(void) const
{
    return this->Ld;
}

template <unsigned int Width>
inline Source<Width> & Output<Width>::GetSource(void)
{
    return this->Src;
}

template <unsigned int Width>
inline const Source<Width> & Output<Width>::GetSource(void) const
{
    return this->Src;
}

}   // namespace Flambeau

#endif /* OUTPUT_H_ */
