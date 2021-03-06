//! @file
//! @brief Data tuple module - Header file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2018
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2012-2018, ICT-Project Mariusz Ornowski (ict-project.pl)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the ICT-Project Mariusz Ornowski nor the names
of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************/
#ifndef _DATA_TUPLE_HEADER
#define _DATA_TUPLE_HEADER
//============================================
#include <tuple>
#include "data-interface.hpp"
//============================================
namespace ict { namespace data {
//===========================================
template< class... Types > 
class array_tuple_t:public std::tuple<Types...>,public complex_interface {
public:
    typedef std::tuple<Types...> tuple_t;
private:
    template <std::size_t N,class T1>
    void data_clear_priv(const std::string & tag=""){
        std::get<N>(*this).data_clear(tag);
     }
    template <std::size_t N, class T1, class T2, class ... Tn >
    void data_clear_priv(const std::string & tag=""){
        std::get<N>(*this).data_clear(tag);
        data_clear_priv<N+1,T2,Tn ...>(tag);
    }
    template <std::size_t N,class T1>
    interface & data_getValue_priv(const std::size_t & index){
        if (N==index) return(std::get<N>(*this));
        throw std::range_error("No value!"); 
    }
    template <std::size_t N, class T1, class T2, class ... Tn >
    interface & data_getValue_priv(const std::size_t & index){
        if (N==index) return(std::get<N>(*this));
        return(data_getValue_priv<N+1,T2,Tn ...>(index));
    }
    template <std::size_t N,class T1>
    const interface & data_getValue_priv(const std::size_t & index) const{
        if (N==index) return(std::get<N>(*this));
        throw std::range_error("No value!"); 
    }
    template <std::size_t N, class T1, class T2, class ... Tn >
    const interface & data_getValue_priv(const std::size_t & index) const {
        if (N==index) return(std::get<N>(*this));
        return(data_getValue_priv<N+1,T2,Tn ...>(index));
    }
public:
    array_tuple_t(){}
    array_tuple_t(const Types&... args ):
        tuple_t(args ...){}
    template <class... UTypes> array_tuple_t(UTypes&&... args):
        tuple_t(args ...){}
    template <class... UTypes> array_tuple_t(const array_tuple_t<UTypes...>& other):
        tuple_t(other){}
    template <class... UTypes> array_tuple_t(array_tuple_t<UTypes...>&& other):
        tuple_t(other){}
    template <class U1,class U2> array_tuple_t(const std::pair<U1,U2>& p):
        tuple_t(p){}
    template <class U1,class U2> array_tuple_t(std::pair<U1,U2>&& p):
        tuple_t(p){}
    array_tuple_t(const array_tuple_t& other):
        tuple_t(other){}
    array_tuple_t(array_tuple_t&& other):
        tuple_t(other){}
    template <class Alloc> array_tuple_t(std::allocator_arg_t x,const Alloc& a):
        tuple_t(x,a){}
    template <class Alloc> array_tuple_t(std::allocator_arg_t x,const Alloc& a,const Types&... args):
        tuple_t(x,a,args ...){}
    template <class Alloc,class... UTypes> array_tuple_t(std::allocator_arg_t x,const Alloc& a,UTypes&&... args):
        tuple_t(x,a,args ...){}
    template <class Alloc,class... UTypes> array_tuple_t(std::allocator_arg_t x,const Alloc& a,const array_tuple_t<UTypes...>& other):
        tuple_t(x,a,other){}
    template <class Alloc,class... UTypes> array_tuple_t(std::allocator_arg_t x,const Alloc& a,array_tuple_t<UTypes...>&& other):
        tuple_t(x,a,other){}
    template <class Alloc,class U1,class U2> array_tuple_t(std::allocator_arg_t x,const Alloc& a,const std::pair<U1, U2>& p):
        tuple_t(x,a,p){}
    template <class Alloc,class U1,class U2> array_tuple_t(std::allocator_arg_t x,const Alloc& a,std::pair<U1, U2>&& p):
        tuple_t(x,a,p){}
    template <class Alloc> array_tuple_t(std::allocator_arg_t x,const Alloc& a,const array_tuple_t& other):
        tuple_t(x,a,other){}
    template <class Alloc> array_tuple_t(std::allocator_arg_t x,const Alloc& a,array_tuple_t&& other):
        tuple_t(x,a,other){}
    template <class... UTypes> array_tuple_t(const std::tuple<UTypes...>& other):
        tuple_t(other){}
    template <class... UTypes> array_tuple_t(std::tuple<UTypes...>&& other):
        tuple_t(other){}
   template <class Alloc,class... UTypes> array_tuple_t(std::allocator_arg_t x,const Alloc& a,const std::tuple<UTypes...>& other):
        tuple_t(x,a,other){}
    template <class Alloc,class... UTypes> array_tuple_t(std::allocator_arg_t x,const Alloc& a,std::tuple<UTypes...>&& other):
        tuple_t(x,a,other){}
    template <class Alloc> array_tuple_t(std::allocator_arg_t x,const Alloc& a,const tuple_t& other):
        tuple_t(x,a,other){}
    template <class Alloc> array_tuple_t(std::allocator_arg_t x,const Alloc& a,tuple_t&& other):
        tuple_t(x,a,other){}

    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        data_clear_priv<0,Types...>(tag);
    }
    //! Patrz: interface::data_pushFront()
    bool data_pushFront(const std::string & tag=""){
        return(true);
    } 
    //! Patrz: interface::data_pushBack()
    bool data_pushBack(const std::string & tag=""){
        return(true);
    }
    //! Patrz: interface::data_getType()
    data_t data_getType() const {
        return(data_array_tuple);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(std::tuple_size<tuple_t>::value);
    }
    //! Patrz: interface::data_getValue()
    interface & data_getValue(const std::size_t & index) {
        return(data_getValue_priv<0,Types...>(index));
    }
    const interface & data_getValue(const std::size_t & index) const {
        return(data_getValue_priv<0,Types...>(index));
    }
};
//===========================================
template< class... Types >
using tuple = array_tuple_t<Types...>;
//===========================================
} }
//===========================================
#endif