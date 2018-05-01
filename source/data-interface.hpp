//! @file
//! @brief Data interface module - Header file.
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
#ifndef _DATA_INTERFACE_HEADER
#define _DATA_INTERFACE_HEADER
//============================================
#include "buffer.hpp"
#include <string>
#include <vector>
#include <stack>
//============================================
namespace ict { namespace data {
//===========================================
enum data_t{
    data_null,
    data_bool,
    data_number_s_char,
    data_number_ss_int,
    data_number_s_int,
    data_number_sl_int,
    data_number_sll_int,
    data_number_u_char,
    data_number_us_int,
    data_number_u_int,
    data_number_ul_int,
    data_number_ull_int,
    data_number_float,
    data_number_double,
    data_number_l_double,
    data_string_string,
    data_string_wstring,
    data_string_bytes,
    data_string_stream,
    data_object,
    data_array
};
//===========================================
template <class T> class comparable {
private:
    virtual int compare(const T & rhs) const =0;
public:
    bool operator==(const T& rhs) const { 
        return(compare(rhs)==0); 
    }
    bool operator!=(const T& rhs) const { 
        return(compare(rhs)!=0); 
    }
    bool operator<(const T& rhs) const { 
        return(compare(rhs)<0); 
    }
    bool operator>(const T& rhs) const { 
        return(compare(rhs)>0); 
    }
    bool operator<=(const T& rhs) const { 
        return(compare(rhs)<=0); 
    }
    bool operator>=(const T& rhs) const { 
        return(compare(rhs)>=0); 
    }
};
//===========================================
class info;
class tags : public std::vector<std::string>,public comparable<tags>{
    int compare(const tags & rhs) const {
        if (size()<rhs.size()) return(-1);
        if (size()>rhs.size()) return(1);
        for (std::size_t k=0;k<size();k++){
            if (at(k)<rhs.at(k)) return(-1);
            if (at(k)>rhs.at(k)) return(1);            
        }
        return(0);
    }
    std::string string() const {
        std::string out;
        for (std::size_t k=0;k<size();k++) {
            if (!k) out+=".";
            out+=at(k);
        }
        return(out);
    }
};
class interface{
private:
    template<class I> class iterator_template: public comparable<iterator_template<I>>{
    private:
        template<class T> class stack_item_t: public comparable<stack_item_t<T>> {
        private:
            int compare(const stack_item_t& rhs) const {
                if (ptr<rhs.ptr) return(-1);
                if (ptr>rhs.ptr) return(1);
                if (step<rhs.step) return(-1);
                if (step>rhs.step) return(1);
                return(0);
            }
        public:
            std::size_t max;
            std::size_t step;
            T ptr;
        };
    public:
        typedef I pointer;
        typedef iterator_template<I> self_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        iterator_template(pointer ptr,bool reverse=false,bool end=false):reverse_value(reverse){
            if (ptr){ 
                if (!end) push(ptr);
            } else {
                throw std::invalid_argument("Null!");
            }
        }
        pointer ptr(){
            if (stack.empty()) throw std::range_error("No value [1]!");
            return(stack.top().ptr);
        }
        std::size_t step() const {
            if (stack.empty()) throw std::range_error("No value [2]!");
            return(stack.top().step>>0x1);
        }
        bool phase() const {
            if (stack.empty()) throw std::range_error("No value [3]!");
            return(stack.top().step&0x1);
        }
        const tags & getTags() const {
            return(tag_vector);
        }
        self_type operator++() {
            self_type i=*this; 
            go(); 
            return(i); 
        }
        self_type operator++(int) {
            go();
            return(*this); 
        }
    private:
        bool reverse_value;
        std::stack<stack_item_t<I>> stack;
        tags tag_vector; 
        void push(pointer ptr,const std::string & tag=""){
            stack_item_t<I> item;
            item.max=(ptr->data_getSize()<<0x1)+1;
            item.step=0;
            item.ptr=ptr;
            stack.push(item);
            if (tag.size()) tag_vector.push_back(tag);
        }
        void pop(){
            if (!tag_vector.empty()) tag_vector.pop_back();
            stack.pop();
        }
        int compare(const self_type & rhs) const {
            if (stack<rhs.stack) return(-1);
            if (stack>rhs.stack) return(1);
            return(0);
        }
        void go(){
            while(!stack.empty()){
                stack_item_t<I> & item(stack.top());
                item.step++;
                if (item.step<item.max){//Przekroczenie zakresu.
                    pop();//Zdejmij element
                } else {//W zakresie
                    if (item.step&0x1){//Wejdź wyżej
                        std::size_t s=item.step>>0x1;
                        std::size_t m=item.max>>0x1;
                        std::size_t i=reverse_value?m-s:s;
                        interface & next(item.ptr->data_getValue(i));
                        push(&next,item.ptr->data_getTag(i));
                        return;
                    } else {//Pozostań w tym miejscu
                        return;
                    }
                }
            }
        }
    };
public:
    class data_iterator:public iterator_template<interface*>{
    public:
        data_iterator(pointer ptr,bool reverse=false,bool end=false):
            iterator_template<interface*>(ptr,reverse,end){}
    };
    class data_const_iterator:public iterator_template<const interface*>{
    public:
        data_const_iterator(pointer ptr,bool reverse=false,bool end=false):
            iterator_template<const interface*>(ptr,reverse,end){}
    };
    //====================
    data_iterator data_begin(){return data_iterator(this,false,false);}
    data_iterator data_end(){return data_iterator(this,false,true);}
    data_const_iterator data_cbegin() const {return data_const_iterator(this,false,false);};
    data_const_iterator data_cend() const {return data_const_iterator(this,false,true);};
    data_iterator data_rbegin(){return data_iterator(this,true,false);}
    data_iterator data_rend(){return data_iterator(this,true,true);}
    data_const_iterator data_crbegin() const {return data_const_iterator(this,true,false);};
    data_const_iterator data_crend() const {return data_const_iterator(this,true,true);};
    //====================
    virtual int data_parse(ict::buffer::interface & buffer){return(0);}
    virtual int data_serialize(ict::buffer::interface & buffer){return(0);}
    virtual int data_validate(std::string & error){return(0);}
    virtual data_t data_getType() const {return(data_null);}
    virtual std::size_t data_getSize() const {return(0);}
    virtual std::string data_getTag(const std::size_t & index) const {return("");}
    virtual interface & data_getValue(const std::size_t & index) {return(*this);}
    virtual void data_getInfo(info & output){}
};
class null_t: public interface{
};
template<typename T> class basic: public interface{
public:
    T value;
public:
    int data_parse(ict::buffer::interface & buffer){
        if (buffer.testIn(value)){
            buffer<<value;
            return(0);
        }
        return(1);
    }
    int data_serialize(ict::buffer::interface & buffer){
        if (buffer.testOut(value)){
            buffer>>value;
            return(0);
        }
        return(1);
    }
    T & operator()(){
        return(value);
    }
};
class bool_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_bool);}
};
class number_s_char_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_s_char);}
};
class number_ss_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_ss_int);}
};
class number_s_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_s_int);}
};
class number_sl_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_sl_int);}
};
class number_sll_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_sll_int);}
};
class number_u_char_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_u_char);}
};
class number_us_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_us_int);}
};
class number_u_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_us_int);}
};
class number_ul_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_ul_int);}
};
class number_ull_int_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_ull_int);}
};
class number_float_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_float);}
};
class number_double_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_double);}
};
class number_l_double_t:public basic<bool>{
public:
    data_t data_getType() const {return(data_number_l_double);}
};
template<class T> class string: public interface{
public:
    T value;
public:
    int data_parse(ict::buffer::interface & buffer){
        if (buffer.testIn(value)){
            buffer<<value;
            return(0);
        }
        return(1);
    }
    int data_serialize(ict::buffer::interface & buffer){
        value.resize(buffer.getSize());
        if (buffer.testOut(value)){
            buffer>>value;
            return(0);
        }
        return(1);
    }
    T & operator()(){
        return(value);
    }
};
class string_string_t:public string<std::string>{
public:
    data_t data_getType() const {return(data_string_string);}
};
class string_wstring_t:public string<std::wstring>{
public:
    data_t data_getType() const {return(data_string_wstring);}
};
class string_bytes_t:public string<ict::buffer::byte_vector_t>{
public:
    data_t data_getType() const {return(data_string_bytes);}
};
class string_stream_t:public interface{
public:
    data_t data_getType() const {return(data_string_stream);}
};
class string_object_t:public interface{
private:
    //TODO
public:
    data_t data_getType() const {return(data_object);}
};
template<class T> class string_array_t:public std::vector<T>,public interface{
private:
    //TODO
public:
    data_t data_getType() const {return(data_array);}
};
class info:public interface{
    //TODO
};
//===========================================
} }
//===========================================
#endif