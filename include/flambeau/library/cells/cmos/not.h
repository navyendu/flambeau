/**
 *  not.h
 *  
 *  Created on: Jan 9, 2013
 *      Author: nav
 *  
 */

#ifndef CMOS_NOT_H_
#define CMOS_NOT_H_

#ifndef __cplusplus
#error Must use C++ for not.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"

#include "flambeau/library/cells/not.h"

namespace Flambeau
{

/*
 * CMOS not gate
 * 
 *              ___
 *               |
 *               |
 *              --
 *         ---o|
 *        |     --
 *        |      |
 *   >----|      |---->
 *        |      |
 *        |     --
 *         ----|
 *              --
 *               |
 *               |
 *               V
 *    
 */
template <TechParam FeatureLength>
class Not<Technology::Cmos, FeatureLength> : public Module
{
    static_assert(FeatureLength > 0, "`FeatureLength` must be non-zero for CMOS technology");
private:
    Not<Technology::Cmos, FeatureLength>(const Not<Technology::Cmos, FeatureLength> &);
    Not<Technology::Cmos, FeatureLength> & operator = (const Not<Technology::Cmos, FeatureLength> &);
protected:
    Input<1>        A;
    
    // We need some tweaks over here
    Signal<1>       YSig;
    Source<2>       YSrc;
    Load<2>         YLoad;
    
    void            OnChange    (void);
public:
    Not (const Signal<1> & y, const Signal<1> & a,
         const string & name = "not<cmos>?");
    Not (const Signal<1> & y, const Signal<1> & a,
         uint64_t scaleFactor,
         const string & name = "not<cmos>?");
    Not (const Signal<1> & y, const Signal<1> & a, 
         uint64_t nmosScaleFactor, uint64_t pmosScaleFactor,
         const string & name = "not<cmos>?");
    ~Not();
};

template <TechParam FeatureLength>
Not<Technology::Cmos, FeatureLength>::Not(const Signal<1> & a, const Signal<1> & y,
                                          const string & name)
    : Not(y, a, 1, 1, name)
{}

template <TechParam FeatureLength>
Not<Technology::Cmos, FeatureLength>::Not(const Signal<1> & a, const Signal<1> & y,
                                          uint64_t scaleFactor,
                                          const string & name)
    : Not(y, a, scaleFactor, 2 * scaleFactor, name)
{}

template <TechParam FeatureLength>
Not<Technology::Cmos, FeatureLength>::Not(const Signal<1> & a, const Signal<1> & y, 
                                          uint64_t nmosScaleFactor, uint64_t pmosScaleFactor,
                                          const string & name)
    : Module(name),
      A     (a, TechDescr<Technology::Cmos, FeatureLength>::UnitNmosGateCap * nmosScaleFactor +
                TechDescr<Technology::Cmos, FeatureLength>::UnitPmosGateCap * pmosScaleFactor,
             this, static_cast<BusListener::THandler>(&Not::OnChange), "A"),
      YSig  (y, "Y"),
      YSrc  (Signal<2>(YSig, YSig),
             array<TResistance, 2>({{TechDescr<Technology::Cmos, FeatureLength>::UnitNmosRes / nmosScaleFactor, TechDescr<Technology::Cmos, FeatureLength>::UnitPmosRes / pmosScaleFactor}})),
      YLoad (Signal<2>(YSig, YSig),
             array<TCapacitance, 2>({{TechDescr<Technology::Cmos, FeatureLength>::UnitNmosDiffCap * nmosScaleFactor, TechDescr<Technology::Cmos, FeatureLength>::UnitPmosDiffCap * pmosScaleFactor}}))
{
    if(nmosScaleFactor == 0 || pmosScaleFactor == 0){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    this->AddSignal(A.GetSignal());
    this->AddSignal(YSig);
    
    YSrc.Name = "Cmos::Not::YSrc";
    
//    printf("%s:%d pcap = %lu, ncap = %lu, pres = %lu, nres = %lu, y.cap = %lu\n", __FILE__, __LINE__,
//           YLoad.GetCapacitance()[1], YLoad.GetCapacitance()[0],
//           YSrc.GetResistance()[1], YSrc.GetResistance()[0],
//           YSig.At(0).GetLoadCap());
}

template <TechParam FeatureLength>
Not<Technology::Cmos, FeatureLength>::~Not()
{
    
}

template <TechParam FeatureLength>
void Not<Technology::Cmos, FeatureLength>::OnChange(void)
{
//    printf("%s:%d Cmos::Not::OnChange: @%lu: A = %s\n", __FILE__, __LINE__, 
//           SimCtrl::GetTime(), A.GetValue().ToString().c_str());
    
    Logic input;
    LogicVec<2> result;
    
    input = A.GetValue()[0];
    
    result[0] = input == Logic::H ? Logic::L : Logic::Z;
    result[1] = input == Logic::H ? Logic::Z : Logic::H;
    
    YSrc.SetValue(result);  //~A.GetValue();
}

}   // namespace Flambeau

#endif /* NOT_H_ */
