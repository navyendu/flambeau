/**
 *  listener.h
 *  
 *  Created on: Dec 10, 2012
 *      Author: nav
 *  
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#ifndef __cplusplus
#error Must use C++ for listener.h
#endif

#include "flambeau/logic.h"

namespace Flambeau
{

class Module;

class LineListener
{
public:
    enum
    {
        ToL     =   0x01,
        ToH     =   0x02,
        ToZ     =   0x04,
        ToX     =   0x08,
        FromL   =   0x10,
        FromH   =   0x20,
        FromZ   =   0x40,
        FromX   =   0x80
    };
    
    enum
    {
        PosEdge     =   FromL | ToH,
        NegEdge     =   FromH | ToL,
        Edge        =   PosEdge | NegEdge,
        Any         =   ToL | ToH | ToZ | ToX | FromL | FromH | FromZ | FromX
    };
    
    static const unsigned int FromLookup [Logic::Count];
    static const unsigned int ToLookup   [Logic::Count];
private:
    LineListener(const LineListener &);
    LineListener & operator = (const LineListener &);
private:
    Module *            Mod;
    void     (Module::* Method) (void);
    unsigned int        Mask;
public:
    LineListener (Module * mod, void (Module::* method)(void),
                  unsigned int mask = LineListener::Any);
    ~LineListener();
    
    void        Trigger (Logic oldVal, Logic newVal) const;
};

class BusListener
{
public:
    typedef void (Module::* THandler) (void);
private:
    BusListener(const BusListener &);
    BusListener & operator = (const BusListener &);
private:
    Module *            Mod;
    THandler            Method;
public:
    BusListener (Module * mod, THandler method);
    ~BusListener();
    
    void            SetHandler          (Module * mod, THandler method);
    
    void            SetHandlerModule    (Module * mod);
    void            SetHandlerMethod    (THandler method);
    
    const Module *  GetHandlerModule    (void) const;
    THandler        GetHandlerMethod    (void) const;
    
    void            Trigger             (void) const;
};

inline const Module * BusListener::GetHandlerModule(void) const
{
    return this->Mod;
}

inline BusListener::THandler BusListener::GetHandlerMethod(void) const
{
    return this->Method;
}

}   // namespace Flambeau

#endif /* LISTENER_H_ */
