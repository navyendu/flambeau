/**
 *  sourcebase.h
 *  
 *  Created on: Jan 2, 2013
 *      Author: nav
 *  
 */

#ifndef SIM_SOURCEBASE_H_
#define SIM_SOURCEBASE_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/sourcebase.h
#endif

#include "flambeau/common.h"

namespace Flambeau
{

class SourceBase
{
    friend class SimCtrl;
private:
    SourceBase(const SourceBase &);
    SourceBase & operator = (const SourceBase &);
protected:
    SourceBase ();
    ~SourceBase();
protected:
    virtual void        ApplyValue  (const Logic * value_a) = 0;
public:
    string      Name;
};

inline SourceBase::SourceBase()
{
    
}

inline SourceBase::~SourceBase()
{
    
}

}   // namespace Flambeau

#endif /* SIM_SOURCEBASE_H_ */
