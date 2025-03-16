/**
 *  signal.h
 *  
 *  Created on: Oct 28, 2012
 *      Author: nav
 *  
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#ifndef __cplusplus
#error Must use C++ for flambeau/signal.h
#endif

#include "flambeau/common.h"
#include "flambeau/sigbase.h"

namespace Flambeau
{

template <unsigned int Width> class Source;
template <unsigned int Width> class Load;
class LineListener;
class BusListener;

/**
 *  @brief
 *      Describes one line of a signal
 *  
 *  @detail
 *      Lines described by SigLine may be shared among multiple Signal<W>'s. So
 *      instances of this class are allocated on the heap and are not deleted
 *      until the end of the program
 *  
 */
class Node
{
    template <unsigned int Width> friend class Signal;
    template <unsigned int Width> friend class Source;
    template <unsigned int Width> friend class Load;
private:
    Node(const Node &);
    Node & operator = (const Node &);
private:
    Logic                           Value;
    uint64_t                        LoadCap;
    
    vector<const Logic *>           SourceValueList;
    vector<const uint64_t *>        LoadValueList;
    vector<const LineListener *>    LineListenerList;
    
    vector<const SignalBase *>      ContainerList;
private:
    Node ();
    ~Node();
    
    void        AddSourceValue  (const Logic * srcVal_p);
    void        AddLoadValue    (const uint64_t * loadVal_p);
    
    void        AddListener     (const LineListener & listener);
    
    void        AddContainer    (const SignalBase & container);
    void        DelContainer    (const SignalBase & container);
public:
    bool        ResolveValue    (bool fireListeners = true);
    void        FireListeners   (Logic oldValue) const;
    
    void        ResolveCap      (void);
    
    Logic       GetValue        (void) const;
    uint64_t    GetLoadCap      (void) const;
    
    const vector<const SignalBase *> &  GetContainerList    (void) const;
};

inline Logic Node::GetValue(void) const
{
    return this->Value;
}

inline uint64_t Node::GetLoadCap(void) const
{
    return this->LoadCap;
}

inline const vector<const SignalBase *> & Node::GetContainerList(void) const
{
    return this->ContainerList;
}

/**
 *  @brief
 *      Signal bus of a fixed width
 *  
 */
template <unsigned int Width>
class Signal : public SignalBase
{
    template <unsigned int W> friend class Signal;
    
    // static_assert(Width > 0, "Can't create signal of zero width");
private:
    array<Node *, Width>    BusLine;
public:
    Signal (const string & name = "-");
    template <unsigned int Width1>
    Signal (const Signal<Width1> & sig1, const Signal<Width-Width1> & sig2, const string & name = "-");
    template <unsigned int Width1>
    Signal (const Signal<Width1> & sig, const string & name = "-");
    template <unsigned int Width1>
    Signal (const Signal<Width1> & sig, unsigned int loIndex, const string & name = "-");
    template <unsigned int Width1>
    Signal (const array<Signal<Width1>, Width/Width1> & sigArray, const string & name = "-");
    Signal (const array<Node *, Width> & nodeArr, const string & name = "-");
    Signal (const Signal<Width> & sig, const string & name = "-");
    ~Signal();
    
    Signal<Width> &         operator =      (const Signal<Width> & sig);
    
    template <unsigned int Width1>
    Signal<Width1>          SubVector       (unsigned int loIndex) const;
    
    template <unsigned int Width1>
    Signal<Width1>          operator ()     (unsigned int hiIndex, unsigned int loIndex) const;
    Signal<1>               operator []     (unsigned int index) const;
    
//    Node &               At              (unsigned int index);
//    const Node &         At              (unsigned int index) const;
    
//    void                    ResolveValue    (unsigned int index);
//    void                    ResolveValue    (void);
    
//    void                    UpdateCap       (unsigned int index);
//    void                    UpdateCap       (void);
    
    LogicVec<Width>         GetValue        (void) const;
    const array<Node *, Width> &
                            GetSigLineVec   (void) const;
    
//    void                    AddSource       (const Source<Width> & src);
//    void                    AddLoad         (const Load<Width> & ld);
    
//    void                    AddLineListener (const LineListener & listener, unsigned int index);
    void                    AddLineListener (const LineListener & listener);
    
    unsigned int            GetWidth        (void) const;
    string                  ToString        (void) const;
    Logic *                 ToLogicArray    (unsigned int & width) const;
};

template <unsigned int Width>
Signal<Width>::Signal(const string & name)
    : SignalBase(name),
      BusLine   ()
{
    printf("Created Signal: \"%s\" %lu, %lX\n", name.c_str(), this->Id, (size_t) this);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->BusLine.at(i) = new Node;
        this->BusLine.at(i)->AddContainer(*this);
    }
}

template <unsigned int Width>
template <unsigned int Width1>
Signal<Width>::Signal(const Signal<Width1> & sig1, const Signal<Width-Width1> & sig2, const string & name)
    : SignalBase(name),
      BusLine   ()
{
    const unsigned int Width2 = Width - Width1;
    
    printf("Created Signal(&,&): \"%s\" %lu||%lu->%lu, %lX||%lX->%lX\n",  name.c_str(),
           sig1.Id, sig2.Id, this->Id, (size_t) &sig1, (size_t) &sig2, (size_t) this);
    
    for(unsigned int i = 0 ; i < Width2 ; i++){
        this->BusLine.at(i) = sig2.BusLine.at(i);
        this->BusLine.at(i)->AddContainer(*this);
    }
    for(unsigned int i = 0 ; i < Width1 ; i++){
        this->BusLine.at(Width2 + i) = sig1.BusLine.at(i);
        this->BusLine.at(Width2 + i)->AddContainer(*this);
    }
}

template <unsigned int Width>
template <unsigned int Width1>
Signal<Width>::Signal(const Signal<Width1> & sig, const string & name)
    : SignalBase(name),
      BusLine   ()
{
    const unsigned int Count = Width / Width1;
    
    static_assert(Count * Width1 == Width, "Width not an integral multiple");
    
    printf("Created Signal(&) * n: \"%s\" %lu->%lu, %lX->%lX\n", name.c_str(),
           sig.Id, this->Id, (size_t) &sig, (size_t) this);
    
    for(unsigned int c = 0 ; c < Count ; c++){
        for(unsigned int i = 0 ; i < Width1 ; i++){
            this->BusLine.at(c * Width1 + i) = sig.BusLine.at(i);
            this->BusLine.at(c * Width1 + i)->AddContainer(*this);
        }
    }
}

template <unsigned int Width>
template <unsigned int Width1>
Signal<Width>::Signal(const Signal<Width1> & sig, unsigned int loIndex, const string & name)
    : SignalBase(name),
      BusLine   ()
{
    if(loIndex + Width > Width1){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    printf("Created Signal(&,0,0): \"%s\" %lu->%lu, %lX->%lX\n", name.c_str(),
           sig.Id, this->Id, (size_t) &sig, (size_t) this);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->BusLine.at(i) = sig.BusLine.at(loIndex + i);
        this->BusLine.at(i)->AddContainer(*this);
    }
}

template <unsigned int Width>
template <unsigned int Width1>
Signal<Width>::Signal(const array<Signal<Width1>, Width/Width1> & sigArray,
                      const string & name)
    : SignalBase(name),
      BusLine   ()
{
    static_assert(true, "Not yet implemented");
}

template <unsigned int Width>
Signal<Width>::Signal(const array<Node *, Width> & nodeArr, const string & name)
    : SignalBase(name),
      BusLine   ()
{
    printf("Created Signal([*]): \"%s\" %lu, %lX, w=%u\n", name.c_str(),
           this->Id, (size_t) this, Width);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        printf("sigLineArr.at(i)=%lX\n", (size_t) nodeArr.at(i));
        fflush(stdout);
        
        this->BusLine.at(i) = nodeArr.at(i);
//        printf("Loop1 Created Signal([*]): \"%s\" %lu, %lX\n", name.c_str(),
//               this->Id, (size_t) this);
        
        this->BusLine.at(i)->AddContainer(*this);
//        printf("Loop2 Created Signal([*]): \"%s\" %lu, %lX\n", name.c_str(),
//               this->Id, (size_t) this);
    }
    
    printf("Exit Created Signal([*]): \"%s\" %lu, %lX\n", name.c_str(),
           this->Id, (size_t) this);
}

template <unsigned int Width>
Signal<Width>::Signal(const Signal<Width> & sig, const string & name)
    : SignalBase(name),
      BusLine   ()
{
    printf("Created Signal(&): \"%s\" %lu->%lu, %lX->%lX\n", name.c_str(),
           sig.Id, this->Id, (size_t) &sig, (size_t) this);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->BusLine.at(i) = sig.BusLine.at(i);
        this->BusLine.at(i)->AddContainer(*this);
    }
}

template <unsigned int Width>
Signal<Width>::~Signal()
{
    printf("Destroyed Signal: \"%s\" Id = %lu, this = %lX\n",
           this->Name.c_str(), this->Id, (size_t) this);
    
    for(unsigned int i = 0 ; i < Width ; i++){
        this->BusLine.at(i)->DelContainer(*this);
    }
    
//    delete &this->BusLine;
}

template <unsigned int Width>
Signal<Width> & Signal<Width>::operator = (const Signal<Width> & sig)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->BusLine.at(i) = sig.BusLine.at(i);
        
//        this->BusLine.at(i)->DelContainer(*this); // Why Del ???
        this->BusLine.at(i)->AddContainer(*this);
    }
    
    return *this;
}

template <unsigned int Width>
template <unsigned int SWidth>
Signal<SWidth> Signal<Width>::SubVector(unsigned int loIndex) const
{
    if(loIndex + SWidth > Width){
        throw GeneralException(__FILE__, __LINE__);
    }
    
    Signal<SWidth> result(*this, loIndex);
    
    return result;
}

//template <unsigned int Width>
//Signal<1> Signal<Width>::SubVector(unsigned int index) const
//{
//    if(index >= Width){
//        throw GeneralException(__FILE__, __LINE__);
//    }
//    
//    Signal<1> result(*this, index, index);
//    
//    return result;
//}

template <unsigned int Width>
template <unsigned int Width1>
inline Signal<Width1> Signal<Width>::operator()(unsigned int hiIndex, unsigned int loIndex) const
{
    return this->SubVector<Width1>(hiIndex, loIndex);
}

template <unsigned int Width>
inline Signal<1> Signal<Width>::operator[](unsigned int index) const
{
    return this->SubVector<1>(index);
}

//template <unsigned int Width>
//Node & Signal<Width>::operator [] (unsigned int index)
//{
//    return *this->BusLine.at(index);
//}
//
//template <unsigned int Width>
//const Node & Signal<Width>::operator [] (unsigned int index) const
//{
//    return *this->BusLine.at(index);
//}

//template <unsigned int Width>
//Node & Signal<Width>::At(unsigned int index)
//{
//    return *this->BusLine.at(index);
//}
//
//template <unsigned int Width>
//const Node & Signal<Width>::At(unsigned int index) const
//{
//    return *this->BusLine.at(index);
//}

//template <unsigned int Width>
//inline void Signal<Width>::ResolveValue(unsigned int index)
//{
//    Logic runningResolution = Logic::Z;
//    printf("%s:%d Width = %u, index = %u\n", __FILE__, __LINE__, Width, index);
//    
//    Logic currentState = this->BusLine.at(index)->Value;
//    
//    printf("%s:%d\n", __FILE__, __LINE__);
//    
//    for(unsigned int i = 0 ; i < this->BusLine.at(index)->SourceValueList.size() ; i++){
//        runningResolution = runningResolution * *this->BusLine.at(index)->SourceValueList.at(i);
//    }
//    
//    printf("%s:%d\n", __FILE__, __LINE__);
//    
//    this->BusLine.at(index)->Value = runningResolution;
//    
//    printf("%s:%d\n", __FILE__, __LINE__);
//    
//    if(runningResolution != currentState){
//        throw int();
//        for(unsigned int i = 0 ; i < this->ListenerList.size() ; i++){
//            if(this->ListenerList.at(i).OnChange != nullptr){
////                printf("%s:%d i = %u\n", __FILE__, __LINE__, i);
//                (this->ListenerList.at(i).ParentModule->*this->ListenerList.at(i).OnChange)();
//            }
//        }
//    }
//}

//template <unsigned int Width>
//inline void Signal<Width>::ResolveValue(unsigned int index)
//{
//    this->BusLine.at(index)->ResolveValue();
//}
//
//template <unsigned int Width>
//void Signal<Width>::ResolveValue(void)
//{
//    for(unsigned int i = 0 ; i < Width ; i++){
//        this->BusLine.at(i)->ResolveValue();
//    }
//}

//template <unsigned int Width>
//inline void Signal<Width>::UpdateCap(unsigned int index)
//{
//    this->BusLine.at(index)->UpdateCap();
//}
//
//template <unsigned int Width>
//void Signal<Width>::UpdateCap(void)
//{
//    for(unsigned int i = 0 ; i < Width ; i++){
//        this->BusLine.at(i)->UpdateCap();
//    }
//}

template <unsigned int Width>
LogicVec<Width> Signal<Width>::GetValue(void) const
{
    LogicVec<Width> result;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.operator[](i) = this->BusLine.at(i)->Value;
    }
    
    return result;
}

template <unsigned int Width>
inline const array<Node *, Width> & Signal<Width>::GetSigLineVec(void) const
{
    return this->BusLine;
}

//template <unsigned int Width>
//void Signal<Width>::AddSource(const Source<Width> & src)
//{
//    for(unsigned int i = 0 ; i < Width ; i++){
//        this->BusLine.at(i)->AddSourceValue(src.GetValue().operator[](i));
//    }
//}
//
//template <unsigned int Width>
//void Signal<Width>::AddLoad(const Load<Width> & ld)
//{
//    for(unsigned int i = 0 ; i < Width ; i++){
//        this->BusLine.at(i)->AddLoadValue(ld.GetCapacitance().operator[](i));
//    }
//}
//
//template <unsigned int Width>
//void Signal<Width>::AddLineListener(const LineListener & listener, unsigned int index)
//{
//    this->BusLine.at(index)->AddListener(listener);
//}

template <unsigned int Width>
void Signal<Width>::AddLineListener(const LineListener & listener)
{
    for(unsigned int i = 0 ; i < Width ; i++){
        this->BusLine.at(i)->AddListener(listener);
    }
}

template <unsigned int Width>
unsigned int Signal<Width>::GetWidth(void) const
{
    return Width;
}

template <unsigned int Width>
string Signal<Width>::ToString(void) const
{
    string result(Width, '?');
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result.at(Width - i - 1) = this->BusLine.at(i)->Value.ToString();
    }
    
    return result;
}

template <unsigned int Width>
Logic * Signal<Width>::ToLogicArray(unsigned int & width) const
{
    Logic * result_a = new Logic[Width];
    width = Width;
    
    for(unsigned int i = 0 ; i < Width ; i++){
        result_a[i] = this->BusLine.at(i)->Value;
    }
    
    return result_a;
}

}   // namespace Flambeau

#endif /* SIGNAL_H_ */
