//! @file
//! @brief Data pair module - Header file.
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
#ifndef _DATA_PAIR_HEADER
#define _DATA_PAIR_HEADER
//============================================
#include <utility>
#include "data-interface.hpp"
//============================================
namespace ict { namespace data {
//===========================================
template<class T1,class T2> 
class array_pair_t:public std::pair<T1,T2>,public complex_interface {
public:
    typedef std::pair<T1,T2> pair_t;
public:
    array_pair_t(){}
    array_pair_t(const T1& x, const T2& y):
        pair_t(x,y){}
    template<class U1,class U2> array_pair_t(U1&& x,U2&& y):
        pair_t(x,y){}
    template<class U1,class U2> array_pair_t(const std::pair<U1,U2>& p):
        pair_t(p){}
    template<class U1,class U2> array_pair_t(std::pair<U1,U2>&& p):
        pair_t(p){}
    template< class... Args1,class... Args2> array_pair_t(
        std::piecewise_construct_t x,
        std::tuple<Args1...> first_args,
        std::tuple<Args2...> second_args
    ):pair_t(x,first_args,second_args){}
    array_pair_t(const pair_t& p):
        pair_t(p){}
    array_pair_t(pair_t&& p):
        pair_t(p){}
    array_pair_t(const array_pair_t& p):
        pair_t(p){}
    array_pair_t(array_pair_t&& p):
        pair_t(p){}

    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        pair_t::first.data_clear();
        pair_t::second.data_clear();
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
        return(data_array_pair);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(2);
    }
    //! Patrz: interface::data_getValue()
    interface & data_getValue(const std::size_t & index) {
        switch (index){
            case 0:return(pair_t::first);
            case 1:return(pair_t::second);
            default:break;
        }
        throw std::range_error("No value!");
    }
    const interface & data_getValue(const std::size_t & index) const {
        switch (index){
            case 0:return(pair_t::first);
            case 1:return(pair_t::second);
            default:break;
        }
        throw std::range_error("No value!");
    }
};
//===========================================
template<class T1,class T2>
using pair = array_pair_t<T1,T2>;
//===========================================
} }
//===========================================
#endif