/**
 *  signal_base.h
 *  
 *  Created on: Dec 11, 2012
 *      Author: nav
 *  
 */

#ifndef SIGNAL_BASE_H_
#define SIGNAL_BASE_H_

#ifndef __cplusplus
#error Must use C++ for signal_base.h
#endif

#include "flambeau/common.h"
#include "flambeau/logic.h"

namespace Flambeau
{

class BusListener;

/**
 *  @brief
 *      Serves as a common base class for all Signal<X>
 *  
 */
class SignalBase
{
private:
    SignalBase(const SignalBase &);
    SignalBase & operator = (const SignalBase &);
private:
    static size_t   IdCount;
protected:
    const size_t                    Id;
    string &                        Name;
    vector<const BusListener *> &   BusListenerList;
protected:
    SignalBase (const string & name);
    ~SignalBase();
public:
    void                    SetName             (const string & name);
    
    const string &          GetName             (void) const;
    size_t                  GetId               (void) const;
    
    void                    AddBusListener      (const BusListener & listener);
    const vector<const BusListener *> &
                            GetBusListenerList  (void) const;
    
    void                    TriggerBusListeners (void) const;
    
    virtual unsigned int    GetWidth            (void) const = 0;
    virtual string          ToString            (void) const = 0;
    virtual Logic *         ToLogicArray        (unsigned int & width) const = 0;
};

inline void SignalBase::SetName(const string & name)
{
    this->Name.assign(name);
}

inline const string & SignalBase::GetName(void) const
{
    return this->Name;
}

inline size_t SignalBase::GetId(void) const
{
    return this->Id;
}

inline void SignalBase::AddBusListener(const BusListener & listener)
{
    this->BusListenerList.push_back(&listener);
}

inline const vector<const BusListener *> & SignalBase::GetBusListenerList(void) const
{
    return this->BusListenerList;
}

}   // namespace Flambeau

#endif /* SIGNAL_BASE_H_ */
