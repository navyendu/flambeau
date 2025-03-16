/**
 *  buffer.h
 *  
 *  Created on: Jan 21, 2013
 *      Author: nav
 *  
 */

#ifndef LIBRARY_CELLS_BUFFER_H_
#define LIBRARY_CELLS_BUFFER_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/library/cells/buffer.h
#endif

#include "flambeau/input.h"
#include "flambeau/output.h"
#include "flambeau/module.h"
#include "flambeau/library/technology.h"

namespace Flambeau
{

template <Technology Tech = Technology::Virtual, TechParam Param = 0>
class Buffer : public Module
{
    static_assert(Param == 0, "Param must be 0 for the default Buffer<> template");
private:
    Buffer(const Buffer<Tech> &);
    Buffer<Tech> & operator = (const Buffer<Tech> &);
protected:
    Input<1>        A;
    Output<1>       Y;
    
    void            OnChange    (void);
public:
    Buffer (const Signal<1> & y, const Signal<1> & a,
            const string & name = "buffer<virt>?");
    Buffer (const Signal<1> & y, const Signal<1> & a,
            TResistance yRes, TCapacitance yCap, TCapacitance aCap,
            const string & name = "buffer<virt>?");
    ~Buffer();
};

template <Technology Tech, TechParam Param>
Buffer<Tech, Param>::Buffer(const Signal<1> & y, const Signal<1> & a,
                            const string & name)
    : Buffer(y, a, 0, 0, 0, name)
{}

template <Technology Tech, TechParam Param>
Buffer<Tech, Param>::Buffer(const Signal<1> & y, const Signal<1> & a,
                            TResistance yRes, TCapacitance yCap, TCapacitance aCap,
                            const string & name)
    : Module(name),
      A     (a, aCap, this, static_cast<BusListener::THandler>(&Buffer::OnChange), "A"),
      Y     (y, yCap, yRes, "Y")
{
    this->AddSignal(A.GetSignal());
    this->AddSignal(Y.GetSignal());
}

template <Technology Tech, TechParam Param>
Buffer<Tech, Param>::~Buffer()
{
    
}

template <Technology Tech, TechParam Param>
void Buffer<Tech, Param>::OnChange(void)
{
//    printf("%s:%d Buffer::OnChange: @%lu: A = %s\n", __FILE__, __LINE__, 
//           SimCtrl::GetTime(), A.GetValue().ToString().c_str());
    
    Y = A.GetValue();
}

}   // namespace Flambeau

#endif /* LIBRARY_CELLS_BUFFER_H_ */
