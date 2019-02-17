//! @file
//! @brief Data binary proxy module - Source file.
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
#include "data-proxy-binary.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#include "data-info.hpp"
#include <unistd.h>
#endif
//============================================
namespace ict { namespace data { namespace proxy {
//============================================
//! Testuje rozmiar typu liczbowego
bool test_number_size(ict::buffer::interface & buffer,const ict::data::data_t & type_in,bool in){
    static const bool_type bool_null=0;
    static const number_s_char_type number_s_char=0;
    static const number_ss_int_type number_ss_int=0;
    static const number_s_int_type number_s_int=0;
    static const number_sl_int_type number_sl_int=0;
    static const number_sll_int_type number_sll_int=0;
    static const number_u_char_type number_u_char=0;
    static const number_us_int_type number_us_int=0;
    static const number_u_int_type number_u_int=0;
    static const number_ul_int_type number_ul_int=0;
    static const number_ull_int_type number_ull_int=0;
    static const number_float_type number_float=0;
    static const number_double_type number_double=0;
    static const number_l_double_type number_l_double=0;
    switch (type_in){
        case data_null:return(in?buffer.testIn(bool_null):buffer.testOut(bool_null));
        case data_bool:return(in?buffer.testIn(bool_null):buffer.testOut(bool_null));
        #define test_number_size_switch(name) \
            case data_##name:\
                return(in?buffer.testIn(name):buffer.testOut(name))
        test_number_size_switch(number_s_char);
        test_number_size_switch(number_ss_int);
        test_number_size_switch(number_s_int);
        test_number_size_switch(number_sl_int);
        #if LONG_MAX<LLONG_MAX
        test_number_size_switch(number_sll_int);
        #endif
        test_number_size_switch(number_u_char);
        test_number_size_switch(number_us_int);
        test_number_size_switch(number_u_int);
        test_number_size_switch(number_ul_int);
        #if ULONG_MAX<ULLONG_MAX
        test_number_size_switch(number_ull_int);
        #endif
        test_number_size_switch(number_float);
        test_number_size_switch(number_double);
        test_number_size_switch(number_l_double);
        #undef test_number_size_switch
        default:break;
    }
    return(false);
}
const std::size_t binary::max_chunk_size(113);
const unsigned char binary::the_next('>');
const unsigned char binary::the_end('<');
const ict::data::interface::data_iterator::proxy_factory::creator<binary> binary::factory;
const ict::data::interface::data_const_iterator::proxy_factory::creator<binary> binary::cfactory;
int binary::parse_string(ict::buffer::interface & buffer,ict::data::interface & iface_in){
    if (!parse_buffer) {
        parse_buffer.reset(new ict::buffer::basic);
        parse_chunk.reset(new std::string);
    }
    if (buffer.testOut(*parse_chunk)){
        buffer>>(*parse_chunk);//
    } else {
        return(1);//Czekaj
    }
    (*parse_buffer)<<(*parse_chunk);
    iface_in.data_parse(*parse_buffer);
    parse_buffer.reset(nullptr);
    parse_chunk.reset(nullptr);
    return(0);//Koniec
}
int binary::serialize_string(ict::buffer::interface & buffer,const ict::data::interface & iface_in) const{
    if (!serialize_buffer){
        serialize_buffer.reset(new ict::buffer::basic);
        serialize_chunk.reset(new std::string);
        iface_in.data_serialize(*serialize_buffer);
    }
    (*serialize_buffer)>>(*serialize_chunk);
    if (buffer.testIn(*serialize_chunk)){
        buffer<<(*serialize_chunk);
    } else {
        return(1);//Czekaj
    }
    serialize_buffer.reset(nullptr);
    serialize_chunk.reset(nullptr);
    return(0);//Koniec
}
int binary::parse_stream(ict::buffer::interface & buffer,ict::data::interface & iface_in){
    if (!parse_buffer) {
        parse_buffer.reset(new ict::buffer::basic);
        parse_chunk.reset(new std::string);
        parse_rv[0]=1;//Ustaw na 'czekaj'
        parse_rv[1]=1;//Ustaw na 'czekaj'
    }
    while (0<parse_rv[0]){
        if (buffer.testIn(*parse_chunk)){//Jeśli parse_chunk jest cały
            buffer>>(*parse_chunk);// buffer -> parse_chunk
            if (parse_chunk->size()){//Jeśli jest coś w parse_chunk
                parse_buffer->dataIn(*parse_chunk);//parse_chunk -> parse_buffer
                parse_chunk->clear(); //Wyczyść parse_chunk
            } else {//Jeśli koniec stream
                parse_rv[0]=0;//Koniec
            }
        } else {//Jeśli nie ma całego
            break;//Przerwij pętlę.
        }
    }
    if (0<parse_rv[1]){//Jeśli jest można coś wstawić. 
        parse_rv[1]=iface_in.data_parse(*parse_buffer);// parse_buffer -> iface_in
    }
    if ((0<=parse_rv[0])&&(0<=parse_rv[0])){//Jeśli nie ma błędu
        if (0<parse_rv[0]) return(parse_rv[0]);//Czekaj
        if (0<parse_rv[1]) return(parse_rv[1]);//Czekaj
    }
    parse_buffer.reset(nullptr);
    parse_chunk.reset(nullptr);
    if (parse_rv[0]<0) return(parse_rv[0]);//Błąd.
    if (parse_rv[1]<0) return(parse_rv[1]);//Błąd.
    return(0);//Koniec
}
int binary::serialize_stream(ict::buffer::interface & buffer,const ict::data::interface & iface_in) const{
    if (!serialize_buffer) {
        serialize_buffer.reset(new ict::buffer::basic);
        serialize_chunk.reset(new std::string);
        serialize_rv[0]=1;//Ustaw na 'czekaj'
        serialize_rv[1]=1;//Ustaw na 'czekaj'
    }
    if (0<serialize_rv[0]) {//Jeśli coś czeka z iface_in
        serialize_rv[0]=iface_in.data_serialize(*serialize_buffer);//iface_in -> serialize_buffer
    }
    while (0<serialize_rv[1]){
        serialize_buffer->dataOut(*serialize_chunk,max_chunk_size);// serialize_buffer -> serialize_chunk
        if (serialize_chunk->size()){//Jeśli serialize_chunk cos zawiera
            if (buffer.testIn(*serialize_chunk)){//Jeśli serialize_chunk się zmieści
                buffer<<(*serialize_chunk);// serialize_chunk -> buffer
                serialize_chunk->clear(); //Wyczyść serialize_chunk
            }
        } if (serialize_rv[0]<=0){ //Jeśli koniec
            serialize_rv[1]=0;
            if (buffer.testIn(*serialize_chunk)){//Jeśli serialize_chunk się zmieści
                buffer<<(*serialize_chunk);// serialize_chunk -> buffer
            }
        }
    }
    if ((0<=parse_rv[0])&&(0<=parse_rv[0])){//Jeśli nie ma błędu
        if (0<parse_rv[0]) return(parse_rv[0]);//Czekaj
        if (0<parse_rv[1]) return(parse_rv[1]);//Czekaj
    }
    serialize_buffer.reset(nullptr);
    serialize_chunk.reset(nullptr);
    if (parse_rv[0]<0) return(parse_rv[0]);//Błąd.
    if (parse_rv[1]<0) return(parse_rv[1]);//Błąd.
    return(0);//Koniec
}
int binary::parse_simple(ict::buffer::interface & buffer,ict::data::interface & iface_in,const ict::data::data_t & type_in){
    switch (type_in) {
        case data_string_string:
        case data_string_wstring:
        case data_string_bytes:return(parse_string(buffer,iface_in));
        case data_string_stream: return(parse_stream(buffer,iface_in));
        default: break;
    }
    if (test_number_size(buffer,type_in,false))//Jeśli jest w buforze.
        return(iface_in.data_parse(buffer));//Pobierz z bufora i zakończ.
    return(1);//Czekaj
}
int binary::serialize_simple(ict::buffer::interface & buffer,const ict::data::interface & iface_in,const ict::data::data_t & type_in) const{
    switch (type_in) {
        case data_string_string:
        case data_string_wstring:
        case data_string_bytes:return(serialize_string(buffer,iface_in));
        case data_string_stream:return(serialize_stream(buffer,iface_in));
        default: break;
    }
    if (test_number_size(buffer,type_in,true))//Jeśli jest w buforze.
        return(iface_in.data_serialize(buffer));//Zapisz do bufora i zakończ.
    return(1);//Czekaj
}
int binary::data_parse(ict::buffer::interface & buffer){
    int rv=0;
    if (!iface) return(0);//Koniec (brak interfejsu)
    if (!parse_it){//Jeśli brak iteratora
        iface->data_clear();//Resetuj obiekt przed parsowaniem.
        parse_it.reset(new ict::data::interface::data_iterator);//Utwórz iterator
        (*parse_it)=iface->data_begin();//Ustaw iterator na początek.
    }
    for (;(*parse_it)!=iface->data_end();++(*parse_it)){//Iteracja po elementach
        ict::data::interface & i=*(parse_it->ptr());//Pobierz interfejs.
        ict::data::data_t t=i.data_getType();//Pobierz typ.
        if (ict::data::simple_type(t)){//Jeśli typ prosty.
            rv=parse_simple(buffer,i,t);//Parsuj typ prosty.
            if (rv) if (rv<0) {//Jeśli błąd.
                break;//Przerwij iterację.
            } else {
                return(rv);//Czekaj
            }
        } else {
            unsigned char tmp;
            if (buffer.testOut(tmp)){//Jeśli jest.
                buffer>>tmp;//Pobierz.
                if (tmp==the_end){//Jeśli to jest ostatni.
                    break;//Koniec iteracji.
                } else { //Jeśli kolejny element.
                    if (ict::data::get_json_type(t)==ict::data::json_object){//Jeśli potrzebny jest tag.
                        if (!parse_tag){//jeśli nie istnieje tag
                            parse_tag.reset(new std::string);//Utwórz tag.
                        }
                        if (buffer.testOut(*parse_tag)) {//Jeśli jest cały tag
                            buffer>>(*parse_tag);//Pobierz tag
                        } else {
                            return(1);//Czekaj                            
                        }
                        i.data_pushBack(*parse_tag);//Dodaj nowy element.
                        parse_tag.reset(nullptr);//Usuń tag.
                    } else {//Jeśli tag nie potrzebny
                        i.data_pushBack();//Dodaj nowy element.
                    }
                }
            } else {
                return(1);//Czekaj
            }
        }
    }
    parse_it.reset(nullptr);//Usuń iterator.
    return(rv);//Koniec
}
int binary::data_serialize(ict::buffer::interface & buffer) const {
    int rv=0;
    if (!ciface) return(0);//Koniec (brak interfejsu)
    if (!serialize_it){//Jeśli brak iteratora
        serialize_it.reset(new ict::data::interface::data_const_iterator);//Utwórz iterator
        (*serialize_it)=ciface->data_cbegin();//Ustaw iterator na początek.
    }
    for (;(*serialize_it)!=ciface->data_cend();++(*serialize_it)){//Iteracja po elementach
        const ict::data::interface & i=*(serialize_it->ptr());//Pobierz interfejs.
        ict::data::data_t t=i.data_getType();//Pobierz typ.
        if (ict::data::simple_type(t)){//Jeśli typ prosty.
            rv=serialize_simple(buffer,i,t);//Serializuj typ prosty.
            if (rv) if (rv<0) {//Jeśli błąd.
                break;//Przerwij iterację.
            } else {
                return(rv);//Czekaj
            }
        } else {//Jeśli typ złożony.
            if (serialize_it->last()){//Jeśli ostatni.
                if (buffer.testIn(the_end)){//Jeśli się zmieści.
                    buffer<<the_end;//Wstaw.
                } else {
                    return(1);//Czekaj
                }
            } else {
                if (!serialize_tag){//Jeśli nie istnieje tag do serializowania
                    if (buffer.testIn(the_next)){//Jeśli się zmieści.
                        buffer<<the_next;//Wstaw.
                    } else {
                        return(1);//Czekaj
                    }
                    if (ict::data::get_json_type(t)==ict::data::json_object){//Jeśli potrzebny jest tag
                        serialize_tag.reset(new std::string);//Utwórz tag.
                        serialize_tag->assign(i.data_getTag(serialize_it->step()));//Ustaw tag.
                    }
                }
                if (serialize_tag){//Jeśli istnieje tag
                    if (buffer.testIn(*serialize_tag)){//Jeśli się zmieści.
                        buffer<<(*serialize_tag);//Wstaw.
                    } else {
                        return(1);//Czekaj
                    }
                    serialize_tag.reset(nullptr);//Usuń tag.
                }
            }
        } 
    }
    serialize_it.reset(nullptr);//Usuń iterator.
    return(rv);//Koniec
}
//===========================================
} } }
//===========================================
#ifdef ENABLE_TESTING
void test_proxy_data_in(ict::data::info & i){
    {
      double value=7.009;
      ict::data::interface::data_setInfo(i,5,value);
    }
    {
      bool value=false;
      ict::data::interface::data_setInfo(i,1,value);
    }
    {
      std::string value="7.009";
      ict::data::interface::data_setInfo(i,6,value);
    }
    {
      unsigned int value=975;
      ict_data_pushFront(i,info_children);
      i.info_children().emplace_back();
      ict_data_pushFront(i.info_children()[0],name);
      i.info_children[0][0].name[0]()="name";
      ict_data_pushFront(i.info_children()[0],value);
      ict::data::interface::data_setInfo(i.info_children()[0].value(),23,value);
    }
}
void test_proxy_data_in(ict::data::string_string_t & s){
    s()="test";
}
void test_proxy_data_in(ict::data::number_double_t & n){
    n()=123.45;
}
int test_proxy_data_out(ict::data::info & i){
    return(0);
}
int test_proxy_data_out(ict::data::string_string_t & s){
    if (s()=="test") return(0);
    std::cerr<<"ERROR: 'test'!='"<<s()<<"' !"<<std::endl;
    return(1);
}
int test_proxy_data_out(ict::data::number_double_t & n){
    if (n()==123.45) return(0);
    std::cerr<<"ERROR: 123.45!="<<n()<<" !"<<std::endl;
    return(1);
}
template <class T> 
void test_proxy_data_in(T & t){
}
template <class T> 
int test_proxy_data_out(T & t){
    return(0);
}
template <class T> 
int test_proxy(){
  try{
    int out;
    T input;
    T output;
    ict::buffer::basic buffer;
    ict::data::interface::data_const_iterator i_in=input.data_cbegin();
    ict::data::interface::data_iterator i_out=output.data_begin();
    test_proxy_data_in(input);
    i_in.proxy_insert({},ict::data::proxy::binary::cfactory.ptr());
    i_out.proxy_insert({},ict::data::proxy::binary::factory.ptr());
    std::cout<<"Serialize..."<<std::endl;
    for(out=1;0<out;){
        out=i_in.ptr()->data_serialize(buffer);
        std::cout<<out<<std::endl;
        usleep(100000);
    }
    if (out<0) {
        std::cerr<<"ERROR: Serialize error!"<<std::endl;
        return(out);
    }
    std::cout<<"Parse..."<<std::endl;
    for(out=1;0<out;){
        out=i_out.ptr()->data_parse(buffer);
        std::cout<<out<<std::endl;
        usleep(100000);
    }
    if (out<0) {
        std::cerr<<"ERROR: Parse error!"<<std::endl;
        return(out);
    }
    out=test_proxy_data_out(output);
    return(out);
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_proxy_binary,tc1){
    return(test_proxy<ict::data::bool_t>());
}
REGISTER_TEST(data_proxy_binary,tc2){
    return(test_proxy<ict::data::number_s_char_t>());
}
REGISTER_TEST(data_proxy_binary,tc3){
    return(test_proxy<ict::data::number_ss_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc4){
    return(test_proxy<ict::data::number_s_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc5){
    return(test_proxy<ict::data::number_sl_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc6){
    return(test_proxy<ict::data::number_sll_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc7){
    return(test_proxy<ict::data::number_u_char_t>());
}
REGISTER_TEST(data_proxy_binary,tc8){
    return(test_proxy<ict::data::number_us_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc9){
    return(test_proxy<ict::data::number_u_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc11){
    return(test_proxy<ict::data::number_ul_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc12){
    return(test_proxy<ict::data::number_ull_int_t>());
}
REGISTER_TEST(data_proxy_binary,tc13){
    return(test_proxy<ict::data::number_float_t>());
}
REGISTER_TEST(data_proxy_binary,tc14){
    return(test_proxy<ict::data::number_double_t>());
}
REGISTER_TEST(data_proxy_binary,tc15){
    return(test_proxy<ict::data::number_l_double_t>());
}
REGISTER_TEST(data_proxy_binary,tc16){
    return(test_proxy<ict::data::string_string_t>());
}
REGISTER_TEST(data_proxy_binary,tc17){
    return(test_proxy<ict::data::string_wstring_t>());
}
REGISTER_TEST(data_proxy_binary,tc18){
    return(test_proxy<ict::data::string_bytes_t>());
}
REGISTER_TEST(data_proxy_binary,tc19){
    return(test_proxy<ict::data::info>());
}
#endif
//===========================================