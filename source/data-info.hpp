//! @file
//! @brief Data info module - Header file.
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
#ifndef _DATA_INFO_HEADER
#define _DATA_INFO_HEADER
//============================================
#include "data-simple.hpp"
#include "data-vector.hpp"
#include "data-object.hpp"
//============================================
namespace ict { namespace data {
//===========================================
class info_item:public object_object_t{
public:
    item_t<bool_t> info_bool;
    item_t<number_s_char_t> info_s_char;
    item_t<number_ss_int_t> info_ss_int;
    item_t<number_s_int_t> info_s_int;
    item_t<number_sl_int_t> info_sl_int;
    item_t<number_sll_int_t> info_sll_int;
    item_t<number_u_char_t> info_u_char;
    item_t<number_us_int_t> info_us_int;
    item_t<number_u_int_t> info_u_int;
    item_t<number_ul_int_t> info_ul_int;
    item_t<number_ull_int_t> info_ull_int;
    item_t<number_float_t> info_float;
    item_t<number_double_t> info_double;
    item_t<number_l_double_t> info_l_double;
    item_t<string_string_t> info_string;
    item_t<number_u_int_t> type;
private:
    void data_registerItems()const{
        ict_data_registerItem(info_bool);
        ict_data_registerItem(info_s_char);
        ict_data_registerItem(info_ss_int);
        ict_data_registerItem(info_s_int);
        ict_data_registerItem(info_sl_int);
        ict_data_registerItem(info_sll_int);
        ict_data_registerItem(info_u_char);
        ict_data_registerItem(info_us_int);
        ict_data_registerItem(info_u_int);
        ict_data_registerItem(info_ul_int);
        ict_data_registerItem(info_ull_int);
        ict_data_registerItem(info_float);
        ict_data_registerItem(info_double);
        ict_data_registerItem(info_l_double);
        ict_data_registerItem(info_string);
        ict_data_registerItem(type);
    }
};
class info_item_array:public array_vector_t<info_item>{};
class info_child;
class info_child_array:public array_vector_t<info_child>{};
class info:public object_object_t{
public:
    item_t<info_item_array> info_params;
    item_t<info_child_array> info_children;
private:
    void data_registerItems()const{
        ict_data_registerItem(info_params);
        ict_data_registerItem(info_children);
    }
};
class info_child:public object_object_t{
public:
    item_t<string_string_t> name;
    item_t<info> value;
private:
    void data_registerItems()const{
      ict_data_registerItem(name);
      ict_data_registerItem(value);
    }
};
//===========================================
} }
//===========================================
#endif