/**
 *  simctrl.cpp
 *  
 *  Created on: Nov 16, 2012
 *      Author: nav
 *  
 */

#include "flambeau/ctrl.h"
#include "flambeau/source.h"
#include "flambeau/logger.h"

//TSimTime::TSimTime()
//{
//    
//}
//
//TSimTime::TSimTime(uint64_t units, uint64_t deltas)
//    : Units(units),
//      Deltas(deltas)
//{
//    
//}
//
//TSimTime::~TSimTime()
//{
//    
//}
//
//TSimTime TSimTime::operator + (const TSimTime & t)
//{
//    return TSimTime(this->Units + t.Units, this->Deltas + t.Units);
//}

//LineValueMap::LineValueMap(SigLine & line, Logic value)
//    : Line (line),
//      Value(value)
//{
//    
//}

namespace Flambeau
{

ThreadDescr::ThreadDescr(TestBench & testerModule,
                         void (TestBench::* method) (void),
                         ucontext_t & linkContext,
                         void (* entryFunc)(void))
    : TesterModule_p(&testerModule),
      Method        (method)
{
    if(getcontext(&this->Context) == -1){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    this->Context.uc_stack.ss_sp = this->Stack_a;
    this->Context.uc_stack.ss_size = sizeof(this->Stack_a);
    this->Context.uc_link = &linkContext;
    
    makecontext(&this->Context, entryFunc, 0);
}

ThreadDescr::~ThreadDescr()
{
    
}

//----

SourceValueMap::SourceValueMap(SourceBase & srcBase, const Logic * value_a)
    : SrcBase(srcBase),
      Value_a(value_a)
{
    
}

SourceValueMap::~SourceValueMap()
{
    
}

//----

uint64_t SimEvent::IdCount = 0;

//SimEvent & SimEvent::Create(TSimTime at, vector<LineValueMap> & lineList)
//{
//    SimEvent & ev = *new SimEvent(SimEvent::UPDATE_LINES, at, &lineList);
//    
////    printf("%s:%d SimEvent::Create(): &ev = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n",
////           __FILE__, __LINE__, (size_t) &ev, ev.Id, ev.EvType, ev.At, (size_t) ev.Line_p);
////    fflush(stdout);
//    
//    return ev;
//}

SimEvent & SimEvent::Create(TSimTime at, SourceValueMap & srcValMap)
{
    SimEvent & ev = *new SimEvent(SimEvent::UPDATE_SOURCE, at, &srcValMap);
    
    return ev;
}

//SimEvent & SimEvent::Create(vector<const SignalBase *> & sigBaseList, TSimTime at)
//{
//    SimEvent & ev = *new SimEvent(SimEvent::TRIGGER_BUS, at, &sigBaseList);
//    
////    printf("%s:%d SimEvent::Create(): &ev = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n",
////           __FILE__, __LINE__, (size_t) &ev, ev.Id, ev.EvType, ev.At, (size_t) ev.Line_p);
////    fflush(stdout);
//    
//    return ev;
//}

SimEvent & SimEvent::Create(TSimTime at, ThreadDescr & thr)
{
    SimEvent & ev = *new SimEvent(SimEvent::WAKE_THREAD, at, &thr);
    
//    printf("%s:%d SimEvent::Create(): &ev = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n",
//           __FILE__, __LINE__, (size_t) &ev, ev.Id, ev.EvType, ev.At, (size_t) ev.Line_p);
//    fflush(stdout);
    
    return ev;
}

void SimEvent::Destroy(SimEvent & ev)
{
//    printf("%s:%d SimEvent::Destroy(): &ev = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n",
//           __FILE__, __LINE__, (size_t) &ev, ev.Id, ev.EvType, ev.At, (size_t) ev.Line_p);
//    fflush(stdout);
    
    delete &ev;
    
//    printf("%s:%d SimEvent::Destroy after delete\n", __FILE__, __LINE__);
//    fflush(stdout);
}

SimEvent::SimEvent(EventType evType, TSimTime at, void * ptr_p)
    : Id    (IdCount++),
      EvType(evType),
      At    (at)
{
    switch(evType){
        case SimEvent::UPDATE_SOURCE: {
            this->SrcMap_p = static_cast<SourceValueMap *>(ptr_p); 
            break;
        }
        case SimEvent::WAKE_THREAD: {
            this->Thr_p  = static_cast<ThreadDescr *>(ptr_p);
            break;
        }
        case SimEvent::FINISH: {
            this->Thr_p = nullptr;
            break;
        }
        default: {
            throw GeneralException(__FILE__, __LINE__);
        }
    }
    
    printf("%s:%d SimEvent(): this = %lX { Id = %lu, Type = %d, At = %lu }\n",
           __FILE__, __LINE__, (size_t) this, this->Id, this->EvType, this->At);
    fflush(stdout);
}

SimEvent::~SimEvent()
{
//    printf("%s:%d ~SimEvent(): this = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n",
//           __FILE__, __LINE__, (size_t) this, this->Id, this->EvType, this->At, (size_t) this->Line_p);
//    fflush(stdout);
}

//----

TSimTime                SimCtrl::CurrTime;
TSimTime                SimCtrl::Duration;
uint32_t                SimCtrl::TimeDeltas;
deque<SimEvent*>        SimCtrl::EventList;
TestBench *             SimCtrl::TesterModule_p;
vector<ThreadDescr *>   SimCtrl::InitList;
vector<ThreadDescr *>   SimCtrl::AlwaysList;
ucontext_t              SimCtrl::MainContext;
ThreadDescr *           SimCtrl::CurrThread_p;
VcDumper *              SimCtrl::DumpMan_p;

TSimTime SimCtrl::GetTime(void)
{
    return SimCtrl::CurrTime;
}

void SimCtrl::PushEvent(SimEvent & ev)
{
    deque<SimEvent*>::iterator iter;
    int count = 0;
    
    for(iter = SimCtrl::EventList.begin() ; iter != SimCtrl::EventList.end() && (*iter)->At <= ev.At ; iter++)
    {
        count++;
    }
    
    SimCtrl::EventList.insert(iter, &ev);
    
//    printf("%s:%d Push: count = %d, ev = { %d, %lu, %lX, %lX }, size = %lu\n", __FILE__, __LINE__, 
//           count, ev.EvType, ev.At, (size_t) ev.Sig, (size_t) ev.Thr,
//           SimCtrl::EventList.size());
}

void SimCtrl::Delay(TSimTime duration)
{
    if(SimCtrl::CurrThread_p == nullptr){
        throw GeneralException(__FILE__, __LINE__);
    }
    
//    printf("%s:%d Delay(): [Enter] SC::CurrThr = %lX\n", __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p);
//    fflush(stdout);
    
    SimEvent & ev = SimEvent::Create(SimCtrl::CurrTime + duration, *SimCtrl::CurrThread_p);
    
//    printf("%s:%d Delay(): &ev = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n",
//           __FILE__, __LINE__, (size_t) &ev, ev.Id, ev.EvType, ev.At, (size_t) ev.Line_p);
//    fflush(stdout);
    
    SimCtrl::PushEvent(ev);
    
//    printf("%s:%d Delay: ev = { %d, %lu, %lX, %lX }, size = %lu\n", __FILE__, __LINE__, 
//           ev.EvType, ev.At, (size_t) ev.Sig, (size_t) ev.Thr,
//           SimCtrl::EventList.size());
//    printf("%s:%d Delay(): SC::CurrThr = %lX\n", __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p);
//    fflush(stdout);
    
    if(swapcontext(&CurrThread_p->Context, &SimCtrl::MainContext) == -1){
        throw GeneralException(__FILE__, __LINE__);
    }
    
//    printf("%s:%d Delay(): [Leave] SC::CurrThr = %lX\n", __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p);
//    fflush(stdout);
}

void SimCtrl::QueueUpdateSource(const SourceBase & srcBase, const Logic * value_a, TSimTime after)
{
    SimEvent & ev = SimEvent::Create(SimCtrl::CurrTime + after, 
                                     *new SourceValueMap(const_cast<SourceBase &>(srcBase), value_a));
    
    SimCtrl::PushEvent(ev);
}

void SimCtrl::AddInit(TestBench & handlerModule, void (TestBench::* handlerMethod)(void))
{
    SimCtrl::InitList.push_back(new ThreadDescr(handlerModule,
                                                handlerMethod,
                                                SimCtrl::MainContext,
                                                SimCtrl::InitThrFn));
}

void SimCtrl::AddAlways(TestBench & handlerModule, void (TestBench::* handlerMethod)(void))
{
    SimCtrl::AlwaysList.push_back(new ThreadDescr(handlerModule,
                                                  handlerMethod,
                                                  SimCtrl::MainContext,
                                                  SimCtrl::AlwaysThrFn));
}

void SimCtrl::DoRun(void)
{
//    int retval;
    
    SimCtrl::CurrThread_p = nullptr;
    
    printf("%s:%d InitList.size = %ld\n", __FILE__, __LINE__, SimCtrl::InitList.size());
    for(unsigned int i = 0 ; i < SimCtrl::InitList.size() ; i++){
        printf("%s:%d &ThrDscr[%u] = %lX\n", __FILE__, __LINE__, i,
               (size_t) &SimCtrl::InitList.at(i));
    }
    
    printf("%s:%d AlwaysList.size = %ld\n", __FILE__, __LINE__, SimCtrl::AlwaysList.size());
    for(unsigned int i = 0 ; i < SimCtrl::AlwaysList.size() ; i++){
        printf("%s:%d &ThrDscr[%u] = %lX\n", __FILE__, __LINE__, i,
               (size_t) &SimCtrl::AlwaysList.at(i));
    }
    
    for(unsigned int i = 0 ; i < SimCtrl::InitList.size() ; i++){
        // Create events to wake up 'init threads' at SimCtrl::CurrTime == 0
        SimEvent & trigEv = SimEvent::Create(0, *SimCtrl::InitList.at(i));
        
        SimCtrl::PushEvent(trigEv);
    }
    
    for(unsigned int i = 0 ; i < SimCtrl::AlwaysList.size() ; i++){
        // Create events to wake up 'always threads' at SimCtrl::CurrTime == 0
        SimEvent & trigEv = SimEvent::Create(0, *SimCtrl::AlwaysList.at(i));
        
        SimCtrl::PushEvent(trigEv);
    }
    
    //----
    
//    for(unsigned int i = 0 ; i < initList.size() + alwaysList.size() ; i++){
//        SimCtrl::CurrThread = &SimCtrl::ThreadDescrList_p->at(i);
//        
//        sem_post(&SimCtrl::CurrThread->Sem);
//        
//        // Main stalls here until thread suspends...
//        
//        sem_wait(&SimCtrl::MainSem);
//    }
    bool finished = false;
    
    while(!SimCtrl::EventList.empty() && SimCtrl::CurrTime < SimCtrl::Duration && !finished){
//        printf("%s:%d Start of while{}\n", __FILE__, __LINE__);
//        fflush(stdout);
        
        SimEvent & ev = *SimCtrl::EventList.front();
        
//        printf("size=%lu\n", SimCtrl::EventList.size());
//        
//        printf("%s:%d Ev = %lX { Id = %lu, Type = %d, At = %lu, Line_p = %lX }\n", __FILE__, __LINE__,
//               (size_t) &ev, ev.Id, ev.EvType, ev.At, (size_t) ev.Thr_p);
//        fflush(stdout);
        
        if(ev.At < SimCtrl::CurrTime){
//            printf("Ev = %d, At = %lu, Sig = %lX, Thr = %lX\n",
//                   ev.EvType,
//                   ev.At,
//                   (size_t) ev.Sig,
//                   (size_t) ev.Thr);
            
            throw GeneralException(__FILE__, __LINE__);
        }
        
//        printf("%s:%d\n", __FILE__, __LINE__);
//        fflush(stdout);
        
        SimCtrl::CurrTime = ev.At;
        
//        printf("%s:%d\n", __FILE__, __LINE__);
//        fflush(stdout);
        
        switch(ev.EvType){
            case SimEvent::UPDATE_SOURCE: {
//                printf("%s:%d UPDATE_SOURCE\n", __FILE__, __LINE__);
//                fflush(stdout);
                
                ev.SrcMap_p->SrcBase.ApplyValue(ev.SrcMap_p->Value_a);
                
//                printf("%s:%d UPDATE_SOURCE\n", __FILE__, __LINE__);
//                fflush(stdout);
                
                break;
            }
            case SimEvent::WAKE_THREAD: {
//                printf("%s:%d Ev = %d, At = %lu, Sig = %lX, Thr = %lX\n", __FILE__, __LINE__,
//                       ev.EvType,
//                       ev.At,
//                       (size_t) ev.Sig,
//                       (size_t) ev.Thr);
                
                SimCtrl::CurrThread_p = ev.Thr_p;
                
                // Main stalls here until the thread calls delay
                if(swapcontext(&SimCtrl::MainContext, &SimCtrl::CurrThread_p->Context) == -1){
                    throw GeneralException(__FILE__, __LINE__);
                }
                
                SimCtrl::CurrThread_p = nullptr;
                
                break;
            }
            case SimEvent::FINISH: {
                finished = true;
//                SimCtrl::CurrTime = SimCtrl::Duration;  // A quick and dirty 
//                                                        // way to come out of 
//                                                        // this while loop
                break;
            }
            default: {
                throw GeneralException(__FILE__, __LINE__);
            }
        }
        
//        printf("%s:%d After switch{}\n", __FILE__, __LINE__);
//        fflush(stdout);
        
        SimCtrl::EventList.pop_front();
//        printf("%s:%d After pop()\n", __FILE__, __LINE__);
        
        SimEvent::Destroy(ev);
        
//        printf("%s:%d End of while{}\n", __FILE__, __LINE__);
//        fflush(stdout);
    }
    
    //----
    
    for(unsigned int i = 0 ; i < SimCtrl::InitList.size() ; i++){
        delete SimCtrl::InitList.at(i);
    }
    
    for(unsigned int i = 0 ; i < SimCtrl::AlwaysList.size() ; i++){
        delete SimCtrl::AlwaysList.at(i);
    }
}

void SimCtrl::InitThrFn(void)
{
//    printf("%s:%d InitThrFn CurrThr_p = %lX {tm_p=%lX, mth=%lX, &ctx=%lX, stk_a=%lX\n",
//           __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p,
//           (size_t) SimCtrl::CurrThread_p->TesterModule_p,
//           (size_t) ((void*) SimCtrl::CurrThread_p->Method),
//           (size_t) &SimCtrl::CurrThread_p->Context,
//           (size_t) SimCtrl::CurrThread_p->Stack_a);
    
    (SimCtrl::CurrThread_p->TesterModule_p->*SimCtrl::CurrThread_p->Method)();
    
//    printf("%s:%d InitThrFn CurrThr_p = %lX {tm_p=%lX, mth=%lX, &ctx=%lX, stk_a=%lX\n",
//           __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p,
//           (size_t) SimCtrl::CurrThread_p->TesterModule_p,
//           (size_t) ((void*) SimCtrl::CurrThread_p->Method),
//           (size_t) &SimCtrl::CurrThread_p->Context,
//           (size_t) SimCtrl::CurrThread_p->Stack_a);
}

void SimCtrl::AlwaysThrFn(void)
{
    while(1){
//        printf("%s:%d AlwaysThrFn CurrThr_p = %lX {tm_p=%ld, mth=%lX, &ctx=%lX, stk_a=%lX\n",
//               __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p,
//               (size_t) SimCtrl::CurrThread_p->TesterModule_p,
//               (size_t) ((void*) SimCtrl::CurrThread_p->Method),
//               (size_t) &SimCtrl::CurrThread_p->Context,
//               (size_t) SimCtrl::CurrThread_p->Stack_a);
        
        (SimCtrl::CurrThread_p->TesterModule_p->*SimCtrl::CurrThread_p->Method)();
        
//        printf("%s:%d AlwaysThrFn CurrThr_p = %lX {tm_p=%ld, mth=%lX, &ctx=%lX, stk_a=%lX\n",
//               __FILE__, __LINE__, (size_t) SimCtrl::CurrThread_p,
//               (size_t) SimCtrl::CurrThread_p->TesterModule_p,
//               (size_t) ((void*) SimCtrl::CurrThread_p->Method),
//               (size_t) &SimCtrl::CurrThread_p->Context,
//               (size_t) SimCtrl::CurrThread_p->Stack_a);
    }
}

void SimCtrl::Run(TestBench & testBench, TSimTime duration)
{
    SimCtrl::TesterModule_p = &testBench;
    SimCtrl::Duration = duration;
    SimCtrl::TimeDeltas = 0;
    SimCtrl::CurrTime = 0;
    SimCtrl::EventList.clear();
    SimCtrl::CurrThread_p = nullptr;
//    SimCtrl::ConsoleLogLevel = LogLevel::Warning;
    
    SimCtrl::DumpMan_p = new VcDumper("wavedump.vcd");
    
    SimCtrl::TesterModule_p->AddChildModule(Module::ScratchPad);
    
    
    DumpMan_p->DumpHeader(testBench, "Nav", 0);
    
    // Knuckle up...
    
    try{
        SimCtrl::DoRun();
    }catch(GeneralException & e){
        printf("Caught GeneralException { %s:%lu }\n", e.FileName, e.LineNum);
    }catch(...){
        printf("?? Caught random exception. Terminating\n");
    }
    
    Logger::Log(LogLevel::Info, "Simulation complete\n");
    
    DumpMan_p->DumpAllSigs(*SimCtrl::TesterModule_p, SimCtrl::CurrTime);
    
    DumpMan_p->DumpTrailer();
    
    delete SimCtrl::DumpMan_p;
}

}   // namespace Flambeau
