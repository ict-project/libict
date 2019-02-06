//! @file
//! @brief Data proxy module - Header file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2019
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2012-2019, ICT-Project Mariusz Ornowski (ict-project.pl)
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
#ifndef _DATA_PROXY_HEADER
#define _DATA_PROXY_HEADER
//============================================
#include "data-interface.hpp"
//============================================
namespace ict { namespace data { namespace proxy {
//===========================================
//! Wzorzec klasy z fabrykami proxy.
template<class P> 
struct proxy_factory {
    static interface * factory(interface * iface){return(new P(iface));}
    static const interface * factory(const interface * iface){return(new P(iface));}
};
//! Domyślne funkcje proxy.
class proxy_default: public interface{
protected:
    interface * iface;
    const interface * ciface;
public:
    proxy_default(interface * iface_in):iface(iface_in),ciface(nullptr){}
    proxy_default(const interface * iface_in):iface(nullptr),ciface(iface_in){}
    //! Patrz ict::data::interface::data_parse()
    virtual int data_parse(ict::buffer::interface & buffer);
    //! Patrz ict::data::interface::data_serialize()
    virtual int data_serialize(ict::buffer::interface & buffer) const;
    //! Patrz ict::data::interface::data_validate()
    virtual int data_validate(std::vector<std::string> & error) const;
    //! Patrz ict::data::interface::data_clear()
    virtual void data_clear(const std::string & tag="");
    //! Patrz ict::data::interface::data_pushFront()
    virtual bool data_pushFront(const std::string & tag="");
    //! Patrz ict::data::interface::data_pushBack()
    virtual bool data_pushBack(const std::string & tag="");
    //! Patrz ict::data::interface::data_getType()
    virtual data_t data_getType() const ;
    //! Patrz ict::data::interface::data_getSize()
    virtual std::size_t data_getSize() const;
    //! Patrz ict::data::interface::data_getTag()
    virtual std::string data_getTag(const std::size_t & index) const;
    //! Patrz ict::data::interface::data_getValue()
    virtual interface & data_getValue(const std::size_t & index);
    virtual const interface & data_getValue(const std::size_t & index) const;
};
//===========================================
} } }
//===========================================
#endif