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
const unsigned char binary::the_first('>');
const unsigned char binary::the_middle('|');
const unsigned char binary::the_last('<');

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
    switch (type_in){
        case data_string_string:
        case data_string_wstring:
        case data_string_bytes://Typy string.
        case data_string_stream://Typ stream.
            return(parse_stream(buffer,iface_in));
        default:break;
    }
    //Pozostałe typy.
    if (test_number_size(buffer,type_in,false))//Jeśli jest w buforze.
        return(iface_in.data_parse(buffer));//Pobierz z bufora i zakończ.
    return(1);//Czekaj
}
int binary::serialize_simple(ict::buffer::interface & buffer,const ict::data::interface & iface_in,const ict::data::data_t & type_in) const{
    switch (type_in){
        case data_string_string:
        case data_string_wstring:
        case data_string_bytes://Typy string.
        case data_string_stream://Typ stream.
            return(serialize_stream(buffer,iface_in));
        default:break;
    }
    //Pozostałe typy.
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
                if (tmp==the_last){//Jeśli to jest ostatni.
                    break;//Koniec iteracji.
                } else if ((tmp!=the_first)&&(tmp!=the_middle)) { //Jeśli inna wartość.
                    rv=-1;//Błąd parsowania
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
            if (serialize_it->first()){//Jeśli pierwszy.
                if (buffer.testIn(the_first)){//Jeśli się zmieści.
                    buffer<<the_first;//Wstaw.
                } else {
                    return(1);//Czekaj
                }
            } else if (serialize_it->last()){//Jeśli ostatni.
                if (buffer.testIn(the_last)){//Jeśli się zmieści.
                    buffer<<the_last;//Wstaw.
                } else {
                    return(1);//Czekaj
                }
            } else {//Jeśli nie pierwszy i ostatni
                if (!serialize_tag){//Jeśli nie istnieje tag do serializowania
                    if (buffer.testIn(the_middle)){//Jeśli się zmieści.
                        buffer<<the_middle;//Wstaw.
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

#endif
//===========================================