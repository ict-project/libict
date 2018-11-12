//! @file
//! @brief Data forward_list module - Header file.
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
#ifndef _DATA_FORWARD_LIST_HEADER
#define _DATA_FORWARD_LIST_HEADER
//============================================
#include <forward_list>
#include "data-interface.hpp"
//============================================
namespace ict { namespace data {
//===========================================
template<typename T,typename A=std::allocator<T>> 
class array_forward_list_t:public std::forward_list<T,A>,public complex_interface {
private:
    typedef std::forward_list<T,A> forward_list_t;
public:
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        forward_list_t::clear();
    }
    //! Patrz: interface::data_pushFront()
    bool data_pushFront(const std::string & tag=""){
        forward_list_t::emplace_front();
        return(true);
    } 
    //! Patrz: interface::data_pushBack()
    bool data_pushBack(const std::string & tag=""){
        forward_list_t::emplace_after(forward_list_t::end());
        return(true);
    }
    //! Patrz: interface::data_getType()
    data_t data_getType() const {
        return(data_array_forward_list);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(std::distance(forward_list_t::cbegin(),forward_list_t::cend()));
    }
    //! Patrz: interface::data_getValue()
    interface & data_getValue(const std::size_t & index) {
        std::size_t i=0;
        for (typename forward_list_t::iterator it=forward_list_t::begin();it!=forward_list_t::end();++it) {
            if (i==index) return(*it);
            i++;
        }
        throw std::range_error("No value!");
    }
    const interface & data_getValue(const std::size_t & index) const {
        std::size_t i=0;
        for (typename forward_list_t::const_iterator it=forward_list_t::cbegin();it!=forward_list_t::cend();++it) {
            if (i==index) return(*it);
            i++;
        }
        throw std::range_error("No value!");
    }
};
//===========================================
template<typename T,typename A=std::allocator<T>>
using forward_list = array_forward_list_t<T,A>;
//===========================================
} }
//===========================================
#endif