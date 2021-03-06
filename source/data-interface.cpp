//! @file
//! @brief Data interface module - Source file.
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
//============================================
#include "data-interface.hpp"
#include "data-info.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace data {
//============================================
#define setInfo_m(p,t) \
void interface::data_setInfo(info & output,number_u_int_type type,const p##t##_type & value) { \
    if (!output.info_params.size()) output.data_pushBack("info_params"); \
    output.info_params().emplace_back(); \
    {\
      auto & back(output.info_params().back());\
      back.data_pushBack("type");\
      back.type()=type;\
      back.data_pushBack("info_" #t);\
      back.info_##t()=value;\
    }\
}
setInfo_m(,bool)
setInfo_m(number_,s_char)
setInfo_m(number_,ss_int)
setInfo_m(number_,s_int)
setInfo_m(number_,sl_int)
setInfo_m(number_,sll_int)
setInfo_m(number_,u_char)
setInfo_m(number_,us_int)
setInfo_m(number_,u_int)
setInfo_m(number_,ul_int)
setInfo_m(number_,ull_int)
setInfo_m(number_,float)
setInfo_m(number_,double)
setInfo_m(number_,l_double)
setInfo_m(string_,string)
#undef setInfo_m
void interface::data_getInfoMain(info & output) const {
  data_setInfo(output,info_types::data_type,type_name(data_getType()));
  data_setInfo(output,info_types::data_type,data_getType());
  data_setInfo(output,info_types::class_name,typeid(*this).name());  
}
std::string interface::data_getTag(const std::size_t & index) const{
  return(std::to_string(index));
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING

#endif
//===========================================
