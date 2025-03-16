/**
 *  dump.h
 *  
 *  Created on: Dec 19, 2012
 *      Author: nav
 *  
 */

#ifndef DUMP_H_
#define DUMP_H_

#ifndef __cplusplus
#error Must use C++ for dump.h
#endif

#include <fstream>

#include "flambeau/common.h"
#include "flambeau/sigbase.h"
#include "flambeau/module.h"

namespace Flambeau
{

class VcDumper
{
public:
    static string   GetStrId    (uint64_t id);
private:
    string          FileName;
    std::ofstream   Fout;
    TSimTime        LastLogTime;
public:
    VcDumper (const string & fileName);
    ~VcDumper();
    
    void        Dump        (const SignalBase & sig, TSimTime at);
    void        DumpAllSigs (const Module & module, TSimTime at);
    
    void        DumpHeader  (const Module & topModule, const string & progName, TSimTime timescale);
    void        DumpVars    (const Module & topModule);
    
    void        DumpTrailer (void);
    
    void        DumpModule  (const Module & module);
};

}   // namespace Flambeau

#endif /* DUMP_H_ */
