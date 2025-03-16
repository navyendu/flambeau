/**
 *  basic.h
 *  
 *  Created on: Nov 15, 2012
 *      Author: nav
 *  
 */

#ifndef BASICS_H_
#define BASICS_H_

#ifndef __cplusplus
#error Must use C++ for common.h
#endif

#include <stdint.h>

#include <string>
#include <array>
#include <vector>
#include <deque>
#include <sstream>

namespace Flambeau
{

using std::string;
using std::array;
using std::vector;
using std::deque;
using std::stringstream;

/**
 *  @brief
 *      Calculates power(n, p) at compile-time
 *  
 */
constexpr uint64_t Power(uint64_t x, uint64_t n)
{
    return n == 0 ? 1 : x * Power(x, n-1);
}

/**
 *  @brief
 *      General exception for debug purposes
 *  
 */
class GeneralException
{
public:
    const char *    FileName;
    size_t          LineNum;
public:
    GeneralException (const char * fileName, size_t lineNum)
        : FileName(fileName),
          LineNum (lineNum)
    {
        printf("GenEx: %s:%lu\n", FileName, LineNum);
    }
    
    GeneralException (const GeneralException & e)
        : FileName(e.FileName),
          LineNum (e.LineNum)
    {
        printf("GenEx Copy: %s:%lu\n", FileName, LineNum);
    }
    
    ~GeneralException()
    {
        
    }
    
    GeneralException & operator = (const GeneralException & e)
    {
        this->FileName = e.FileName;
        this->LineNum = e.LineNum;
        
        return *this;
    }
};

typedef uint64_t    TSimTime;
typedef uint64_t    TResistance;
typedef uint64_t    TCapacitance;

template <typename T>
bool SearchInVector(const std::vector<T> & vec, const T & value, unsigned int * index_p)
{
    for(unsigned int i = 0 ; i < vec.size() ; i++){
        if(vec.at(i) == value){
            if(index_p != nullptr){
                *index_p = i;
            }
            
            return true;        // Value found
        }
    }
    
    return false;               // Value not found
}

template <typename T>
bool InsertIfAbsent(std::vector<T> & vec, const T & value)
{
    for(unsigned int i = 0 ; i < vec.size() ; i++){
        if(vec.at(i) == value){
            return false;                   // No insertion done
        }
    }
    
    vec.push_back(value);
    
    return true;                            // Value inserted
}

}   // namespace Flambeau

#endif /* BASICS_H_ */
