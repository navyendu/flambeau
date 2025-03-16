/**
 *  logger.cpp
 *  
 *  Created on: Jan 11, 2013
 *      Author: nav
 *  
 */

#include "flambeau/logger.h"

namespace Flambeau
{

std::ostream * Logger::OutStream_p = &std::cout;
const array<string, 5> Logger::LevelString =
{{
    "ERROR: ",
    "WARNING: ",
    "INFO: ",
    "DEBUG: ",
    "VERBOSE: "
}};

void Logger::Log(LogLevel level, const string & msg, bool flush)
{
    (*Logger::OutStream_p) << LevelString.at((unsigned int) level) << msg;
    if(flush){
        Logger::OutStream_p->flush();
    }
}

std::ostream & Logger::Redirect(std::ostream & outStream)
{
    std::ostream & oldStream = *Logger::OutStream_p;
    
    Logger::OutStream_p = &outStream;
    
    return oldStream;
}

}   // namespace Flambeau
