//! @file
//! @brief Data simple types module - Header file.
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
#ifndef _DATA_SIMPLE_HEADER
#define _DATA_SIMPLE_HEADER
//============================================
#include "buffer.hpp"
#include "data-types.hpp"
#include "data-interface.hpp"
//============================================
namespace ict { namespace data {
//===========================================
template<typename T> class basic: public simple_interface{
public:
    T value=0;
public:
    typedef T value_t;
    //! Patrz: interface::data_parse()
    int data_parse(ict::buffer::interface & buffer) {
        //if (buffer.testOut(value)){
            buffer>>value;
            return(0);
        //}
        //return(1);
    }
    //! Patrz: interface::data_serialize()
    int data_serialize(ict::buffer::interface & buffer) const {
        //if (buffer.testIn(value)){
            buffer<<value;
            return(0);
        //}
        //return(1);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(sizeof(T));
    }
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        value=0;
    }
    //! 
    //! @brief Udostępnia zmienną.
    //! 
    //! @return Zmienna typu liczba.
    //! 
    value_t & operator()(){
        return(value);
    }
};
class null_t:public basic<bool_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_null);}
};
class bool_t:public basic<bool_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_bool);}
};
class number_s_char_t:public basic<number_s_char_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_s_char);}
};
class number_ss_int_t:public basic<number_ss_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_ss_int);}
};
class number_s_int_t:public basic<number_s_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_s_int);}
};
class number_sl_int_t:public basic<number_sl_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_sl_int);}
};
class number_sll_int_t:public basic<number_sll_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_sll_int);}
};
class number_u_char_t:public basic<number_u_char_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_u_char);}
};
class number_us_int_t:public basic<number_us_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_us_int);}
};
class number_u_int_t:public basic<number_u_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_us_int);}
};
class number_ul_int_t:public basic<number_ul_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_ul_int);}
};
class number_ull_int_t:public basic<number_ull_int_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_ull_int);}
};
class number_float_t:public basic<number_float_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_float);}
};
class number_double_t:public basic<number_double_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_double);}
};
class number_l_double_t:public basic<number_l_double_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_number_l_double);}
};
template<class T> class string: public simple_interface{
public:
    T value;
public:
    typedef T value_t;
    //! Patrz: interface::data_parse()
    int data_parse(ict::buffer::interface & buffer) {
        //if (buffer.testOut(value)){
            buffer>>value;
            return(0);
        //}
        //return(1);
    }
    //! Patrz: interface::data_serialize()
    int data_serialize(ict::buffer::interface & buffer) const {
        //if (buffer.testIn(value)){
            buffer<<value;
            return(0);
        //}
        //return(1);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(value.size());
    }
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        value.clear();
    }
    //! 
    //! @brief Udostępnia zmienną.
    //! 
    //! @return Zmienna typu string.
    //! 
    value_t & operator()(){
        return(value);
    }
};
class string_string_t:public string<string_string_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_string_string);}
};
class string_wstring_t:public string<string_wstring_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_string_wstring);}
};
class string_bytes_t:public string<string_bytes_type>{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_string_bytes);}
};
class string_stream_t:public interface{
public:
    //! Patrz: interface::data_getType()
    data_t data_getType() const {return(data_string_stream);}
};
//===========================================
} }
//===========================================
#endif