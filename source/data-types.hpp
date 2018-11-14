//! @file
//! @brief Data types module - Header file.
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
#ifndef _DATA_TYPES_HEADER
#define _DATA_TYPES_HEADER
//============================================
#include <string>
#include "buffer.hpp"
//============================================
namespace ict { namespace data {
//===========================================
//! Typ danych JSON
enum json_t{
    json_null=0x1<<((2*8)-4),
    json_bool=0x2<<((2*8)-4),
    json_number=0x3<<((2*8)-4),
    json_string=0x4<<((2*8)-4),
    json_object=0x5<<((2*8)-4),
    json_array=0x6<<((2*8)-4),
    //=======================
    json__mask=0xf<<((2*8)-4),
};
//! Typ danych C
enum ctype_t{
    ctype_sint=0x1<<((2*8)-4-4),
    ctype_uint=0x2<<((2*8)-4-4),
    ctype_float=0x3<<((2*8)-4-4),
    ctype_string=0x4<<((2*8)-4-4),
    ctype_complex=0x5<<((2*8)-4-4),
    //=======================
    ctype__mask=0xf<<((2*8)-4-4),
};
//! Typ danych złożonych (np. kontenerów)
enum complex_t{
    complex_object=0x1,
    complex_array=0x2,
    complex_deque=0x3,
    complex_forward_list=0x4,
    complex_list=0x5,
    complex_map=0x6,
    complex_multimap=0x7,
    complex_queue=0x8,
    complex_priority_queue=0x9,
    complex_set=0xa,
    complex_multiset=0xb,
    complex_stack=0xc,
    complex_unordered_map=0xd,
    complex_unordered_multimap=0xe,
    complex_unordered_set=0xf,
    complex_unordered_multiset=0x10,
    complex_vector=0x11,
    complex_tuple=0x12,
    complex_pair=0x13,
    //=======================
    complex__mask=0xff,
};
//! Typy danych
enum data_t{
    data_null=json_null|0x0|0x0,// null
    data_bool=json_bool|0x0|0x0,// bool 
    data_number_s_char=json_number|ctype_sint|sizeof(signed char),// signed char
    data_number_ss_int=json_number|ctype_sint|sizeof(signed short int),// signed short int
    data_number_s_int=json_number|ctype_sint|sizeof(signed int),// signed int
    data_number_sl_int=json_number|ctype_sint|sizeof(signed long int),// signed long int
    data_number_sll_int=json_number|ctype_sint|sizeof(signed long long int),// signed long long int
    data_number_u_char=json_number|ctype_uint|sizeof(unsigned char),// unsigned char
    data_number_us_int=json_number|ctype_uint|sizeof(unsigned short int),// unsigned short int
    data_number_u_int=json_number|ctype_uint|sizeof(unsigned int),// unsigned int
    data_number_ul_int=json_number|ctype_uint|sizeof(unsigned long int),// unsigned long int
    data_number_ull_int=json_number|ctype_uint|sizeof(unsigned long long int),// unsigned long long int
    data_number_float=json_number|ctype_float|sizeof(float),// float
    data_number_double=json_number|ctype_float|sizeof(double),// double
    data_number_l_double=json_number|ctype_float|sizeof(long double),// long double
    data_string_string=json_string|ctype_string|sizeof(char),// std::string
    data_string_wstring=json_string|ctype_string|sizeof(wchar_t),// std::wstring
    data_string_bytes=json_string|ctype_string|0x0,// std::vector<>
    data_string_stream=json_string|ctype_string|0xff,
    data_object_object=json_object|ctype_complex|complex_object,
    data_array_array=json_array|ctype_complex|complex_array,
    data_array_deque=json_array|ctype_complex|complex_deque,
    data_array_forward_list=json_array|ctype_complex|complex_forward_list,
    data_array_list=json_array|ctype_complex|complex_list,
    data_array_map=json_array|ctype_complex|complex_map,
    data_object_map=json_object|ctype_complex|complex_map,
    data_array_multimap=json_array|ctype_complex|complex_multimap,
    data_array_queue=json_array|ctype_complex|complex_queue,
    data_array_priority_queue=json_array|ctype_complex|complex_priority_queue,
    data_array_set=json_array|ctype_complex|complex_set,
    data_array_multiset=json_array|ctype_complex|complex_multiset,
    data_array_stack=json_array|ctype_complex|complex_stack,
    data_array_unordered_map=json_array|ctype_complex|complex_unordered_map,
    data_array_unordered_multimap=json_array|ctype_complex|complex_unordered_multimap,
    data_array_unordered_set=json_array|ctype_complex|complex_unordered_set,
    data_array_unordered_multiset=json_array|ctype_complex|complex_unordered_multiset,
    data_array_vector=json_array|ctype_complex|complex_vector,
    data_array_tuple=json_array|ctype_complex|complex_tuple,
    data_array_pair=json_array|ctype_complex|complex_pair,
};
//===========================================
typedef bool bool_type;
typedef signed char number_s_char_type;
typedef signed short int number_ss_int_type;
typedef signed int number_s_int_type;
typedef signed long int number_sl_int_type;
typedef signed long long int number_sll_int_type;
typedef unsigned char number_u_char_type;
typedef unsigned short int number_us_int_type;
typedef unsigned int number_u_int_type;
typedef unsigned long int number_ul_int_type;
typedef unsigned long long int number_ull_int_type;
typedef float number_float_type;
typedef double number_double_type;
typedef long double number_l_double_type;
typedef std::string string_string_type;
typedef std::wstring string_wstring_type;
typedef ict::buffer::byte_vector_t string_bytes_type;
//===========================================
//! 
//! @brief Sprawdza czy typ jest złożony.
//! 
//! @param type Typ.
//! @return true - Typ złożony
//! @return false - Typ prosty
//! 
bool complex_type(data_t type);
//! 
//! @brief Sprawdza czy typ jest prosty.
//! 
//! @param type Typ.
//! @return true - Typ prosty
//! @return false - Typ złożony.
//! 
bool simple_type(data_t type);
//! 
//! @brief Zwaca nazwę typu.
//! 
//! @param type Typ.
//! @return Nazwa.
//! 
const std::string & type_name(data_t type);
//===========================================
} }
//===========================================
#endif