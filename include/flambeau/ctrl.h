/**
 *  simctrl.h
 *  
 *  Created on: Nov 16, 2012
 *      Author: nav
 *  
 */

#ifndef SIMCTRL_H_
#define SIMCTRL_H_

#ifndef __cplusplus
#error Must use C++ for simctrl.h
#endif

#include <signal.h>
#include <ucontext.h>

#include "flambeau/common.h"
#include "flambeau/sigbase.h"
#include "flambeau/sourcebase.h"
#include "flambeau/module.h"
#include "flambeau/listener.h"
#include "flambeau/dump.h"

namespace Flambeau
{

class TestBench;

struct ThreadDescr
{
public:
    static const unsigned int StackSize = 8192;
public:
    TestBench *         TesterModule_p;
    void (TestBench::*  Method) (void);
    
    ucontext_t          Context;
    uint8_t             Stack_a [ThreadDescr::StackSize];
public:
    ThreadDescr (TestBench & testerModule,
                 void (TestBench::* method)(void),
                 ucontext_t & linkContext,
                 void (* entryFunc)(void));
    ~ThreadDescr();
};

struct SourceValueMap
{
    SourceBase &    SrcBase;
    const Logic *   Value_a;
public:
    SourceValueMap (SourceBase & srcBase, const Logic * value_a);
    ~SourceValueMap();
};

class SimEvent
{
public:
    enum EventType
    {
        UPDATE_SOURCE,
        WAKE_THREAD,
        FINISH
    };
private:
    static uint64_t     IdCount;
public:
    static SimEvent &   Create  (TSimTime at, SourceValueMap & srcValMap);
    static SimEvent &   Create  (TSimTime at, ThreadDescr & thr);
    static void         Destroy (SimEvent & ev);
public:
    uint64_t    Id;
    EventType   EvType;
    TSimTime    At;
    union
    {
//        vector<LineValueMap> *  MapVec_p;
        const SourceValueMap *  SrcMap_p;
//        vector<const SignalBase *> * Sigs_p;
        ThreadDescr *           Thr_p;
    };
private:
    SimEvent (EventType evType, TSimTime at, void * ptr_p);
    ~SimEvent();
};

class SimCtrl
{
private:
    SimCtrl ();
    ~SimCtrl();
    
    static void     InitThrFn   (void);
    static void     AlwaysThrFn (void);
    static void     DoRun       (void);
public:
    static TSimTime                 CurrTime;
    static uint32_t                 TimeDeltas;
    static deque<SimEvent*>         EventList;
    static TestBench *              TesterModule_p;
    static TSimTime                 Duration;
    
    static vector<ThreadDescr *>    InitList;
    static vector<ThreadDescr *>    AlwaysList;
    static ucontext_t               MainContext;
    
    static ThreadDescr *            CurrThread_p;
    
    static VcDumper *               DumpMan_p;
    
    static void     PushEvent           (SimEvent & ev);
public:
    static TSimTime GetTime             (void);
    
    static void     Delay               (TSimTime duration);
//    static void     QueueUpdateLines    (vector<LineValueMap> & lineList, TSimTime after);
    static void     QueueUpdateSource   (const SourceBase & srcBase, const Logic * value_a, TSimTime after);
//    static void     QueueBusListeners   (vector<const SignalBase *> & sigBaseList, TSimTime after);
    
    static void     AddInit             (TestBench & handlerModule, void (TestBench::* method)(void));
    static void     AddAlways           (TestBench & handlerModule, void (TestBench::* method)(void));
    
    static void     Run                 (TestBench & testBench, TSimTime duration = (TSimTime)(-1));
};

}   // namespace Flambeau

#endif /* SIMCTRL_H_ */
