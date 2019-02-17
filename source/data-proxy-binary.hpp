//! @file
//! @brief Data binary proxy module - Header file.
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
#ifndef _DATA_BINARY_PROXY_HEADER
#define _DATA_BINARY_PROXY_HEADER
//============================================
#include "data-interface.hpp"
#include "data-proxy.hpp"
#include "factory.hpp"
//============================================
namespace ict { namespace data { namespace proxy {
//===========================================
//! Binarne proxy.
class binary: public proxy_default{
private:
    //! Maksymalny rozmiar kawałka danych - serialize_stream()
    const static std::size_t max_chunk_size;
    //! Znak przed elementenm typu złożonego - data_parse() i data_serialize()
    const static unsigned char the_next;
    //! Znak po elementamach typu złożonego - data_parse() i data_serialize()
    const static unsigned char the_end;
    //! Iterator podczas parsowania - data_parse()
    mutable std::unique_ptr<ict::data::interface::data_iterator> parse_it;
    //! Iterator podczas serializowania - data_serialize()
    mutable std::unique_ptr<ict::data::interface::data_const_iterator> serialize_it;
    //! Tag do parsowania - data_parse()
    mutable std::unique_ptr<std::string> parse_tag;
    //! Tag do serializowania - data_serialize()
    mutable std::unique_ptr<std::string> serialize_tag;
    //! Bufor podczas parsowania i serializowania - parse_stream() i serialize_stream()
    mutable std::unique_ptr<ict::buffer::basic> parse_buffer,serialize_buffer;
    //! Wartość podczas parsowania i serializowania - parse_stream() i serialize_stream()
    mutable std::unique_ptr<std::string> parse_chunk,serialize_chunk;
    //! Zwracana wartość podczas parsowania i serializowania - parse_stream() i serialize_stream()
    mutable int parse_rv[2],serialize_rv[2];


    int parse_stream(ict::buffer::interface & buffer,ict::data::interface & iface_in);
    int serialize_stream(ict::buffer::interface & buffer,const ict::data::interface & iface_in) const;
    int parse_string(ict::buffer::interface & buffer,ict::data::interface & iface_in);
    int serialize_string(ict::buffer::interface & buffer,const ict::data::interface & iface_in) const;
    int parse_simple(ict::buffer::interface & buffer,ict::data::interface & iface_in,const ict::data::data_t & type_in);
    int serialize_simple(ict::buffer::interface & buffer,const ict::data::interface & iface_in,const ict::data::data_t & type_in) const;
public:
    binary(interface * iface_in):proxy_default(iface_in){}
    binary(const interface * iface_in):proxy_default(iface_in){}
    static const ict::data::interface::data_iterator::proxy_factory::creator<binary> factory;
    static const ict::data::interface::data_const_iterator::proxy_factory::creator<binary> cfactory;
    //! Patrz ict::data::interface::data_parse()
    int data_parse(ict::buffer::interface & buffer);
    //! Patrz ict::data::interface::data_serialize()
    int data_serialize(ict::buffer::interface & buffer) const;
    //! Patrz ict::data::interface::data_getType()
    virtual data_t data_getType() const {return(data_string_stream);}
};
//===========================================
} } }
//===========================================
#endif