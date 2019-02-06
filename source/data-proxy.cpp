//! @file
//! @brief Data proxy module - Source file.
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
//============================================
#include "data-proxy.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace data { namespace proxy {
//============================================
int proxy_default::data_parse(ict::buffer::interface & buffer) {
    if (iface) return(iface->data_parse(buffer));
    return(0);
}
int proxy_default::data_serialize(ict::buffer::interface & buffer) const {
    if (iface) return(iface->data_serialize(buffer));
    if (ciface) return(ciface->data_serialize(buffer));
    return(0);
}
int proxy_default::data_validate(std::vector<std::string> & error) const {
    if (iface) return(iface->data_validate(error));
    if (ciface) return(ciface->data_validate(error));
    return(0);
}
void proxy_default::data_clear(const std::string & tag){
    if (iface) iface->data_clear(tag);
}
bool proxy_default::data_pushFront(const std::string & tag){
    if (iface) return(iface->data_pushFront(tag));
    return(false);
}
bool proxy_default::data_pushBack(const std::string & tag){
    if (iface) return(iface->data_pushBack(tag));
    return(false);
}
data_t proxy_default::data_getType() const {
    if (iface) return(iface->data_getType());
    if (ciface) return(ciface->data_getType());
    return(data_null);
}
std::size_t proxy_default::data_getSize() const {
    if (iface) return(iface->data_getSize());
    if (ciface) return(ciface->data_getSize());
    return(0);
}
std::string proxy_default::data_getTag(const std::size_t & index) const{
    if (iface) return(iface->data_getTag(index));
    if (ciface) return(ciface->data_getTag(index));
    return("");
}
interface & proxy_default::data_getValue(const std::size_t & index) {
    if (iface) return(iface->data_getValue(index));
    return(*this);
}
const interface & proxy_default::data_getValue(const std::size_t & index) const {
    if (iface) return(iface->data_getValue(index));
    if (ciface) return(ciface->data_getValue(index));
    return(*this);
}
//===========================================
} } }
//===========================================
#ifdef ENABLE_TESTING

#endif
//===========================================
