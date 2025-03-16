/**
 *  dump.cpp
 *  
 *  Created on: Dec 19, 2012
 *      Author: nav
 *  
 */

#include "flambeau/dump.h"

namespace Flambeau
{

string VcDumper::GetStrId(uint64_t id)
{
    unsigned int range = (unsigned int)'~' - (unsigned int)'!' + 1;
    uint64_t idcopy = id;
    uint64_t digitCount = 0;
    
    while(idcopy >= range){
        digitCount++;
        
        idcopy /= range;
    }
    digitCount++;
    
    string result(digitCount, (char) 7);
    for(unsigned int i = 0 ; i < digitCount ; i++){
        result.at(digitCount - i - 1) = '!' + id % range;
        id /= range;
    }
    
    return result;
}

VcDumper::VcDumper(const string & fileName)
    : FileName      (fileName),
      Fout          (fileName, std::ios::out),
      LastLogTime   (static_cast<TSimTime>(-1))
{
    
}

VcDumper::~VcDumper()
{
    this->Fout.close();
}

void VcDumper::DumpHeader(const Module & topModule, const string & progName, TSimTime timescale)
{
    this->Fout << "$date " << "2222-22-22" << " $end\n";
    
    this->Fout << "$version " << progName << " $end\n";
    
    this->Fout << "$timescale " << "1ps" << " $end\n";
    
    this->DumpModule(topModule);
//    this->DumpVars(topModule);
    
    this->Fout << "$enddefinition $end\n";
}

void VcDumper::DumpModule(const Module & module)
{
    const vector<const SignalBase *> sigList = module.GetSignalList();
    const vector<const Module *> childModList = module.GetChildModuleList();
    
    this->Fout << "$scope module " << module.GetName() << " $end\n";
    
    for(unsigned int i = 0 ; i < sigList.size() ; i++){
        this->Fout << "$var reg " << sigList.at(i)->GetWidth() << ' '
//                                  << VcDumper::GetStrId(sigList.at(i)->GetId()) << ' '
                                  << sigList.at(i)->GetId() << ' '
                                  << sigList.at(i)->GetName();
        if(sigList.at(i)->GetWidth() > 1){
            this->Fout << "[" << sigList.at(i)->GetWidth() - 1 << ":0]";
        }
        this->Fout << " $end\n";
    }
    
    for(unsigned int i = 0 ; i < childModList.size() ; i++){
        this->DumpModule(*childModList.at(i));
    }
    
    this->Fout << "$upscope $end\n";
}

void VcDumper::Dump(const SignalBase & sig, TSimTime at)
{
    if(this->LastLogTime != at){
        this->Fout << '#' << at << '\n';
        this->LastLogTime = at;
    }
    
    if(sig.GetWidth() == 1){
//        this->Fout << sig.ToString() << VcDumper::GetStrId(sig.GetId()) << '\n';
        this->Fout << sig.ToString() << sig.GetId() << '\n';
    }else{
//        this->Fout << 'b' << sig.ToString() << ' ' << VcDumper::GetStrId(sig.GetId()) << '\n';
        this->Fout << 'b' << sig.ToString() << ' ' << (sig.GetId()) << '\n';
    }
}

void VcDumper::DumpAllSigs(const Module & module, TSimTime at)
{
    const vector<const SignalBase *> sigList = module.GetSignalList();
    const vector<const Module *> childModList = module.GetChildModuleList();
    
    for(unsigned int i = 0 ; i < sigList.size() ; i++){
        this->Dump(*sigList.at(i), at);
    }
    
    for(unsigned int i = 0 ; i < childModList.size() ; i++){
        this->DumpAllSigs(*childModList.at(i), at);
    }
}

void VcDumper::DumpTrailer(void)
{
    
}

}   // namespace Flambeau
