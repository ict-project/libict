//! @file
//! @brief Data types module - Source file.
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
#include "data-types.hpp"
#include <map>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace data {
//============================================
bool complex_type(data_t type){
  return((type&ctype__mask)==ctype_complex);
}
bool simple_type(data_t type){
  return(!complex_type(type));
}
const std::string & type_name(data_t type){
  static const std::string unknown("unknown");
  static const std::map<data_t,std::string> map={
    #define map_item(name) {name,#name}
    map_item(data_null),
    map_item(data_bool),
    map_item(data_number_s_char),
    map_item(data_number_ss_int),
    map_item(data_number_s_int),
    map_item(data_number_sl_int),
    map_item(data_number_sll_int),
    map_item(data_number_u_char),
    map_item(data_number_us_int),
    map_item(data_number_u_int),
    map_item(data_number_ul_int),
    map_item(data_number_ull_int),
    map_item(data_number_float),
    map_item(data_number_double),
    map_item(data_number_l_double),
    map_item(data_string_string),
    map_item(data_string_wstring),
    map_item(data_string_bytes),
    map_item(data_string_stream),
    map_item(data_object_object),
    map_item(data_array_array),
    map_item(data_array_deque),
    map_item(data_array_forward_list),
    map_item(data_array_list),
    map_item(data_array_map),
    map_item(data_array_multimap),
    map_item(data_array_queue),
    map_item(data_array_priority_queue),
    map_item(data_array_set),
    map_item(data_array_multiset),
    map_item(data_array_stack),
    map_item(data_array_unordered_map),
    map_item(data_array_unordered_multimap),
    map_item(data_array_unordered_set),
    map_item(data_array_unordered_multiset),
    map_item(data_array_vector),
    map_item(data_array_tuple),
    map_item(data_array_pair),
    #undef map_item
  };
  if (map.count(type)) return(map.at(type));
  return(unknown);
}
json_t get_json_type(const data_t & type){
  return((json_t)(type&json__mask));
}
ctype_t get_ctype_type(const data_t & type){
  return((ctype_t)(type&ctype__mask));
}
complex_t get_complex_type(const data_t & type){
  return((complex_t)(type&complex__mask));
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING

#endif
//===========================================
