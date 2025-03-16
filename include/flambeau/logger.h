/**
 *  logger.h
 *  
 *  Created on: Jan 11, 2013
 *      Author: nav
 *  
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#ifndef __cplusplus
#error Must use C++ for logger.h
#endif

#include <iostream>
//#include <stdarg.h>

#include "flambeau/common.h"

namespace Flambeau
{

enum class LogLevel
{
    Error,
    Warning,
    Info,
    Debug,
    Verbose
};

class Logger
{
private:
    static std::ostream *   OutStream_p;
    static const array<string, 5> LevelString;
public:
    static void             Log     (LogLevel level, const string & msg, bool flush = false);
    static std::ostream &   Redirect(std::ostream & outStream); 
};

}   // namespace Flambeau

#endif /* LOGGER_H_ */
