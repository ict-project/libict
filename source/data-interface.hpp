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
#include <map>
#include <stack>
#include <typeinfo>
#include <type_traits>
//============================================
namespace ict { namespace data {
//===========================================
//! Typy danych
enum data_t{
    data_null=0x0001,// null
    data_bool=0x0002,// bool
    data_number_s_char=0x1000+sizeof(signed char),// signed char
    data_number_ss_int=0x1000+sizeof(signed short int),// signed short int
    data_number_s_int=0x1000+sizeof(signed int),// signed int
    data_number_sl_int=0x1000+sizeof(signed long int),// signed long int
    data_number_sll_int=0x1000+sizeof(signed long long int),// signed long long int
    data_number_u_char=0x2000+sizeof(unsigned char),// unsigned char
    data_number_us_int=0x2000+sizeof(unsigned short int),// unsigned short int
    data_number_u_int=0x2000+sizeof(unsigned int),// unsigned int
    data_number_ul_int=0x2000+sizeof(unsigned long int),// unsigned long int
    data_number_ull_int=0x2000+sizeof(unsigned long long int),// unsigned long long int
    data_number_float=0x4000+sizeof(float),// float
    data_number_double=0x4000+sizeof(double),// double
    data_number_l_double=0x4000+sizeof(long double),// long double
    data_string_string=0x8000+sizeof(char),// std::string
    data_string_wstring=0x8000+sizeof(wchar_t),// std::wstring
    data_string_bytes=0x8000,// std::vector<>
    data_string_stream=0x0004,
    data_object_object=0x0008,
    data_array_vector=0x0010
};
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
//! Wzorzec obietów porównywalnych
template <class T> class comparable {
private:
    //!
    //! @brief Funkcja porównująca dane (do nadpisania).
    //! 
    //! @param rhs Obiekt do porównania z tym obiektem.
    //! @return int Wartości:
    //!  @li -1 - Ten obiekt jest mniejszy od rhs.
    //!  @li 0 - Obiekty są równe.
    //!  @li 1 - Ten biekt jest więlszy od rhs.
    //! 
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
class info;
//===========================================
//! Porównwalna wersja std::vector<std::string>
class tags : public std::vector<std::string>,public comparable<tags>{
private:
    int compare(const tags & rhs) const {
        if (size()<rhs.size()) return(-1);
        if (size()>rhs.size()) return(1);
        for (std::size_t k=0;k<size();k++){
            if (at(k)<rhs.at(k)) return(-1);
            if (at(k)>rhs.at(k)) return(1);            
        }
        return(0);
    }
public:
    //! 
    //! @brief Zwraca string połączonych za pomocą kropki ('.') wartosći std::string.
    //! 
    //! @return std::string Połączony ciąg znaków.
    //! 
    std::string string() const {
        std::string out;
        for (std::size_t k=0;k<size();k++) {
            if (k) out+=".";
            out+=at(k);
        }
        return(out);
    }
};
//! Podstawowy interfejs danych.
struct info_types{
    const static number_u_int_type data_type=0x1;
    const static number_u_int_type class_name=0x2;
};
class interface{
private:
    //! Wzorzec iteratora jednokierunkowego.
    template<class I> class iterator_template: public comparable<iterator_template<I>>{
    private:
        //! Wzorzec elementu stosu iteratora.
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
            //! Krok w ramach obiektu.
            std::size_t step;
            //! Wskaźnik do obiektu.
            T ptr;
        };
    public:
        typedef I pointer;
        typedef iterator_template<I> self_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        //! 
        //! @brief Construct a new iterator template object
        //! 
        //! @param ptr Wskaźnik obiektu powiązanego z iteratorem.
        //! @param reverse Kierunek działania iteratora.
        //! 
        iterator_template(pointer ptr,bool reverse=false):reverse_value(reverse){
            if (ptr) {
                push(ptr);
            } else {
                throw std::invalid_argument("Null!");
            }
        }
        //! 
        //! @brief Construct a new iterator template object (end)
        //! 
        //! @param reverse Kierunek działania iteratora.
        //! 
        iterator_template(bool reverse=false):reverse_value(reverse){
        }
        //! 
        //! @brief Zwraca wskaźnik aktualnego obiektu.
        //! 
        //! @return pointer Wskaźnik aktualnego obiektu.
        //! 
        pointer ptr() const {
            if (stack.empty()) throw std::range_error("No value [1]!");
            return(stack.top().ptr);
        }
        pointer operator->() const {
            return(ptr());
        }
        //! 
        //! @brief Zwraca rozmiar stosu iteratora. 
        //! 
        //! @return std::size_t Rozmiar stosu iteratora.
        //! 
        std::size_t level() const {
            return(stack.size());
        }
        //! 
        //! @brief Zwraca aktualny krok w ramach aktualnego obiektu. 
        //! 
        //! @return std::size_t Aktualny krok w ramach aktualnego obiektu.
        //! 
        std::size_t step() const {
            if (stack.empty()) throw std::range_error("No value [2]!");
            return(stack.top().step>>0x1);
        }
        //! 
        //! @brief Zwraca liczbę elementów w ramach aktualnego obiektu.
        //! 
        //! @return std::size_t Liczba elementów.
        //! 
        std::size_t size() const {
            if (stack.empty()) throw std::range_error("No value [3]!");
            return(stack.top().ptr->data_getSize());  
        }
        //! 
        //! @brief Sprawdza, czy to pierwszy element w ramach aktualnego obiektu.
        //! 
        //! @return true Pierwszy element
        //! @return false Inny elememnt.
        //! 
        bool first() const {
            return(step()<=0);  
        }
        //! 
        //! @brief Sprawdza, czy to ostatni element w ramach aktualnego obiektu.
        //! 
        //! @return true Ostatni element.
        //! @return false Inny element.
        //! 
        bool last() const {
            return(size()<=step());  
        }
        //! 
        //! @brief Sprawdza, czy to jest środkowy element w ramach aktualnego obiektu.
        //! 
        //! @return true Środkowy element.
        //! @return false Inny element.
        //! 
        bool middle() const {
            if (first()||last()) return(false);
            return(true);  
        }
        //! 
        //! @brief Get the Tags object
        //! 
        //! @return const tags& 
        //! 
        const tags & getTags() const {
            return(tag_vector);
        }
        //! 
        //! @brief Krok iteratora.
        //! 
        //! @return self_type 
        //! 
        self_type operator++() {
            self_type i=*this; 
            go(); 
            return(i); 
        }
        //! 
        //! @brief Krok iteratora.
        //! 
        //! @return self_type 
        //! 
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
                data_t type=item.ptr->data_getType();
                if (complex_type(type)){//Typy złożone
                    std::size_t max=item.ptr->data_getSize();
                    item.step++;//Inkrementacja kroku
                    max<<=0x1;
                    max++;
                    if (item.step<max){//W zakresie
                        if (item.step&0x1){//Wejdź wyżej
                            std::size_t s=item.step>>0x1;
                            std::size_t m=item.ptr->data_getSize();
                            std::size_t i=reverse_value?m-s-1:s;
                            push(&(item.ptr->data_getValue(i)),item.ptr->data_getTag(i));
                            return;
                        } else {//Pozostań w tym miejscu
                            return;
                        }
                    } else {//Przekroczenie zakresu.
                        pop();//Zdejmij element
                    }
                } else {//Typy proste
                    pop();//Zdejmij element
                }
            }
        }
    };
public:
    typedef interface value_t;
    //! Iterator jednokierunkowy.
    class data_iterator:public iterator_template<interface*>{
    public:
        //! 
        //! @brief Construct a new data iterator object
        //! 
        //! @param ptr Wskaźnik obiektu powiązanego z iteratorem.
        //! @param reverse Kierunek działania iteratora.
        //! 
        data_iterator(pointer ptr,bool reverse=false):
            iterator_template<interface*>(ptr,reverse){}
        //! 
        //! @brief Construct a new data iterator object (end)
        //! 
        //! @param reverse Kierunek działania iteratora.
        //! 
        data_iterator(bool reverse=false):
            iterator_template<interface*>(reverse){}
    };
    //! Iterator jednokierunkowy (const).
    class data_const_iterator:public iterator_template<const interface*>{
    public:
        //! 
        //! @brief Construct a new data iterator object
        //! 
        //! @param ptr Wskaźnik obiektu powiązanego z iteratorem.
        //! @param reverse Kierunek działania iteratora.
        //! 
        data_const_iterator(pointer ptr,bool reverse=false):
            iterator_template<const interface*>(ptr,reverse){}
        //! 
        //! @brief Construct a new data iterator object
        //! 
        //! @param reverse Kierunek działania iteratora.
        //! 
        data_const_iterator(bool reverse=false):
            iterator_template<const interface*>(reverse){}
    };
    //====================
    //! 
    //! @brief Zwraca iterator wskazujący początek obieku (iteracja naprzód).
    //! 
    //! @return Iterator wskazujący początek obieku.
    //! 
    data_iterator data_begin() {
        return data_iterator(this,false);
    }
    //! 
    //! @brief Zwraca iterator kończący iterację.
    //! 
    //! @return Iterator kończący.
    //! 
    const data_iterator & data_end() const {
        const static data_iterator i(false);
        return (i);
    }
    //! 
    //! @brief Zwraca iterator wskazujący początek obieku (iteracja naprzód - const).
    //! 
    //! @return Iterator wskazujący początek obieku.
    //! 
    data_const_iterator data_cbegin() const {
        return data_const_iterator(this,false);
    };
    //! 
    //! @brief Zwraca iterator kończący iterację (const).
    //! 
    //! @return Iterator kończący.
    //! 
    const data_const_iterator & data_cend() const {
        const static data_const_iterator i(false);
        return (i);
    };
    //! 
    //! @brief Zwraca iterator wskazujący koniec obieku (iteracja wstecz).
    //! 
    //! @return Iterator wskazujący koniec obieku.
    //! 
    data_iterator data_rbegin(){
        return data_iterator(this,true);
    }
    //! 
    //! @brief Zwraca iterator kończący iterację.
    //! 
    //! @return Iterator kończący.
    //! 
    const data_iterator & data_rend() const {
        const static data_iterator i(true);
        return(i);
    }
    //! 
    //! @brief Zwraca iterator wskazujący koniec obieku (iteracja wstecz - const).
    //! 
    //! @return Iterator wskazujący koniec obieku.
    //! 
    data_const_iterator data_crbegin() const {
        return data_const_iterator(this,true);
    };
    //! 
    //! @brief Zwraca iterator kończący iterację (const).
    //! 
    //! @return Iterator kończący.
    //! 
    const data_const_iterator & data_crend() const {
        const static data_const_iterator i(true);
        return(i);
    };
protected:
    //! 
    //! @brief Zapisuje w klasie info dane.
    //! 
    //! @param output Klasa info.
    //! @param type Typ informacji.
    //! @param value Wartość do zapisania.
    //! 
    static void data_setInfo(info & output,number_u_int_type type,const bool_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_s_char_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_ss_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_s_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_sl_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_sll_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_u_char_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_us_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_u_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_ul_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_ull_int_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_float_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_double_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const number_l_double_type & value);
    static void data_setInfo(info & output,number_u_int_type type,const string_string_type & value);
    //! 
    //! @brief Tworzy podstawową informację o obiekcie.
    //! 
    //! @param output Informacja o obiekcie.
    //! 
    void data_getInfoMain(info & output) const;
    //! 
    //! @brief Tworzy informację o obiekcie - funkcja do nadpisania.
    //! 
    //! @param output Informacja o obiekcie.
    //! @return Sukces lub porażka. 
    //! 
    virtual bool data_getInfo(info & output) const {}
public:
    //====================
    //! 
    //! @brief Funkcja parsująca bufor binarny do obiektu.
    //! 
    //! @param buffer Bufor zawierający dane do parsowania.
    //! @return Wynik parsowania - wartości:
    //!  @li -1 - Wystąpił błąd podczas parsowania.
    //!  @li 0 - Parsowanie zakończone.
    //!  @li 1 - Parsowanie nie jest zakończone ze względu na niekompletne dane w buforze.
    //! 
    virtual int data_parse(ict::buffer::interface & buffer) {return(0);}
    //! 
    //! @brief Funkcja serializująca obiekt do bufora binarnego.
    //! 
    //! @param buffer Bufor do zapisania danych.
    //! @return Wynik seralizowania - wartości:
    //!  @li -1 - Wystąpił błąd podczas serializowania.
    //!  @li 0 - Serializowanie zakończone.
    //!  @li 1 - Serializowanie nie jest zakończone ze względu na brak miejsca w buforze.
    //! 
    virtual int data_serialize(ict::buffer::interface & buffer) const {return(0);}
    //! 
    //! @brief Funkcja walidująca obiekt.
    //! 
    //! @param error String do zapisywania opisu ewentualnych błędów.
    //! @return Wynik parsowania - wartości:
    //!  @li -1 - Wystąpił błąd podczas walidacji.
    //!  @li 0 - Walidowanie zakończone - brak błędów.
    //!  @li 1 - Walidowanie zakończone - wsytąpiły błędy (opis w error).
    //!     
    virtual int data_validate(std::string & error) const {return(0);}
    //! 
    //! @brief Resetowanie obiektu. Ewentualnie jednego element obiektu.
    //! @param tag Wskazanie tagu elementu do resetowania (gdy pusty cały obiekt jest resetowany).
    //! 
    virtual void data_clear(const std::string & tag=""){}
    //! 
    //! @brief Dodaje element na początku obiektu.
    //! 
    //! @param tag Wskazanie tagu elementu do wstawienia.
    //! @return Sukces lub porażka. 
    //! 
    virtual bool data_pushFront(const std::string & tag=""){return(false);}
    //! 
    //! @brief Dodaje element na końcu obiektu.
    //! 
    //! @param tag Wskazanie tagu elementu do wstawienia.
    //! @return Sukces lub porażka. 
    //! 
    virtual bool data_pushBack(const std::string & tag=""){return(false);}
    //! 
    //! @brief Zwraca typ obiektu.
    //! 
    //! @return Typ obiektu. 
    //! 
    virtual data_t data_getType() const {return(data_null);}
    //! 
    //! @brief Zwraca rozmiar (w zależności od kontekstu).
    //! 
    //! @return Rozmiar.
    //! 
    virtual std::size_t data_getSize() const {return(0);}
    //! 
    //! @brief Zwraca tag elementu o indeksie index.
    //! 
    //! @param index Indeks elementu.
    //! @return Tag elementu.
    //! 
    virtual std::string data_getTag(const std::size_t & index) const;
    //! 
    //! @brief Zwraca interfejs elementu o indeksie index.
    //! 
    //! @param index 
    //! @return interface& 
    //! 
    virtual interface & data_getValue(const std::size_t & index) {return(*this);}
    virtual const interface & data_getValue(const std::size_t & index) const {return(*this);}
    //! 
    //! @brief Zwraca interfejs pierwszego elementu.
    //! 
    //! @return interface& 
    //!
    virtual interface & data_getFront() {return(data_getValue(0));}
    virtual const interface & data_getFront() const {return(data_getValue(0));}
    //! 
    //! @brief Zwraca interfejs ostatniego elementu.
    //! 
    //! @return interface& 
    //!
    virtual interface & data_getBack() {return(data_getValue(data_getSize()-1));}
    virtual const interface & data_getBack() const {return(data_getValue(data_getSize()-1));}
};
class simple_interface: public interface{
};
class complex_interface: public interface{
};
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
template<class T> class array_t:public std::vector<T>,public complex_interface{
private:
    typedef std::vector<T> vector_t;
public:
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        vector_t::clear();
    }
    //! Patrz: interface::data_pushFront()
    bool data_pushFront(const std::string & tag=""){
        vector_t::emplace(vector_t::begin());
        return(true);
    } 
    //! Patrz: interface::data_pushBack()
    bool data_pushBack(const std::string & tag=""){
        vector_t::emplace_back();
        return(true);
    }
    //! Patrz: interface::data_getType()
    data_t data_getType() const {
        return(data_array_vector);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(vector_t::size());
    }
    //! Patrz: interface::data_getValue()
    interface & data_getValue(const std::size_t & index) {
        return(vector_t::operator[](index));
    }
    const interface & data_getValue(const std::size_t & index) const {
        return(vector_t::at(index));
    }
    //! Patrz: interface::data_getFront()
    virtual interface & data_getFront() {
        return(vector_t::front());
    }
    //! Patrz: interface::data_getBack()
    virtual interface & data_getBack() {
        return(vector_t::back());
    }
};
class object_t:public complex_interface{
public:
    class item_interface_t {
        friend class object_t;
    private:
        virtual void emplace_back()=0;
        virtual void clear()=0;
        virtual interface & get_interface(const std::size_t & index)=0;
    public:
        
        //Capacity:
        //size Return size (public member function )
        virtual std::size_t size()const noexcept=0;
        //max_size Return maximum size (public member function )
        virtual std::size_t max_size()const noexcept=0;
        //capacity Return size of allocated storage capacity (public member function )
        virtual std::size_t capacity()const noexcept=0;
        //empty Test whether vector is empty (public member function )
        virtual bool empty()const noexcept=0;
    };
    template<class T> class item_template_t: public item_interface_t {
    protected:
        typedef std::vector<T> vector_t;
        vector_t value;

        void emplace_back(){
            value.emplace_back();
            value.shrink_to_fit();
        }
        void clear(){
            value.clear();
            value.shrink_to_fit();
        }
        virtual interface & get_interface(const std::size_t & index){
            return((interface &)(value[index]));
        }
    public:
        typedef typename vector_t::iterator iterator;
        typedef typename vector_t::const_iterator const_iterator;
        //Iterators:
        //begin Return iterator to beginning (public member function )
        iterator begin()noexcept{return(value.begin());}
        //end Return iterator to end (public member function )
        iterator end()noexcept{return(value.end());}
        //rbegin Return reverse iterator to reverse beginning (public member function )
        iterator rbegin()noexcept{return(value.rbegin());}
        //rend Return reverse iterator to reverse end (public member function )
        iterator rend()noexcept{return(value.rend());}
        //cbegin Return const_iterator to beginning (public member function )
        const_iterator cbegin()const noexcept{return(value.cbegin());}
        //cend Return const_iterator to end (public member function )
        const_iterator cend()const noexcept{return(value.cend());}
        //crbegin Return const_reverse_iterator to reverse beginning (public member function )
        const_iterator crbegin()const noexcept{return(value.crbegin());}
        //crend Return const_reverse_iterator to reverse end (public member function )
        const_iterator crend()const noexcept{return(value.crend());}

        //Capacity:
        //size Return size (public member function )
        std::size_t size()const noexcept{return(value.size());}
        //max_size Return maximum size (public member function )
        std::size_t max_size()const noexcept{return(value.max_size());}
        //capacity Return size of allocated storage capacity (public member function )
        std::size_t capacity()const noexcept{return(value.capacity());}
        //empty Test whether vector is empty (public member function )
        bool empty()const noexcept{return(value.empty());}

        //Element access:
        //operator[] Access element (public member function )
        T & operator[](const std::size_t & index){return(value[index]);}
        //at Access element (public member function )
        T & at(const std::size_t & index)const{return(value.at(index));}
        //front Access first element (public member function )
        T & front(){return(value.front());}
        //back Access last element (public member function )
        T & back(){return(value.back());}
        //data Access data (public member function )
    };
    template<class T,class Enable = void> class item_t {
    };
    template<class T>
    class item_t<T,typename std::enable_if<std::is_base_of<complex_interface,T>::value>::type>: public item_template_t<T> {
    public:
        //! 
        //! @brief Udostępnia element o podanym indeksie.
        //! 
        //! @param index Indeks elementu.
        //! @return Element.
        //! 
        T & operator()(const std::size_t & index=0){
            return(item_template_t<T>::value[index]);
        }
    };
    template<class T>
    class item_t<T,typename std::enable_if<!std::is_base_of<complex_interface,T>::value>::type>: public item_template_t<T> {
    public:
        //! 
        //! @brief Udostępnia element o podanym indeksie.
        //! 
        //! @param index Indeks elementu.
        //! @return Element.
        //! 
        typename T::value_t & operator()(const std::size_t & index=0){
            return(item_template_t<T>::value[index].value);
        }
    };
    typedef std::size_t item_offset_t;
    typedef std::size_t item_index_t;
    typedef item_interface_t* item_ptr_t;
private:
    struct item_list_t {
        item_offset_t offset;
        item_index_t index;
    };
    typedef std::vector<item_list_t> list_vector_t;
    list_vector_t list_vector;
    void registerItems()const;
protected:
    //! 
    //! @brief Rejestruje składniki obiektu. Funkcja do nadpisania.
    //! 
    virtual void data_registerItems()const{}
    //! 
    //! @brief Rejestruje wskazany składnik obiektu.
    //! 
    //! @param name Nazwa składnika.
    //! @param item Wskaźnik składnika.
    //! 
    void data_registerItem(const std::string & name,const void * item)const;
    #define ict_data_registerItem(item) data_registerItem(#item,&item)
public:
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag="");
    //! Patrz: interface::data_pushFront()
    bool data_pushFront(const std::string & tag="");
    //! Patrz: interface::data_pushBack()
    bool data_pushBack(const std::string & tag="");
    //! Patrz: ict::data:interface
    data_t data_getType() const {return(data_object_object);}
    //! Patrz: ict::data:interface
    std::size_t data_getSize() const {return(list_vector.size());}
    //! Patrz: ict::data:interface
    std::string data_getTag(const std::size_t & index) const;
    //! Patrz: ict::data:interface
    interface & data_getValue(const std::size_t & index);
    const interface & data_getValue(const std::size_t & index) const;
    //=================================
    //! 
    //! @brief Dodaje element na końcu wskazanego składnika obiektu. Podczas iteracji składnik jest iterowany na początku.
    //! 
    //! @param item Wskaźnik składnika obiektu.
    //! 
    void data_pushFront(item_ptr_t item);
    #define ict_data_pushFront(object,item) object.data_pushFront(&(object.item));
    //! 
    //! @brief Dodaje element na końcu wskazanego składnika obiektu. Podczas iteracji składnik jest iterowany na końcu.
    //! 
    //! @param item Wskaźnik składnika obiektu.
    //! 
    void data_pushBack(item_ptr_t item);
    #define ict_data_pushBack(object,item) object.data_pushBack(&(object.item));
    //! 
    //! @brief Czyści wskazany składnik obiektu.
    //! 
    //! @param item Wskaźnik składnika obiektu.
    //! 
    void data_clear(item_ptr_t item);
    #define ict_data_clear(object,item) object.data_clear(&(object.item));
};
//===========================================
template<class T> class info_pair:public object_t{
public:
    item_t<ict::data::number_u_int_t> type;
    item_t<T> value;
private:
    void data_registerItems()const{
      ict_data_registerItem(type);
      ict_data_registerItem(value);
    }
};
class info_child;
template<class T> class info_array:public array_t<info_pair<T>>{};
typedef  info_array<bool_t> info_bool_t;
typedef  info_array<number_s_char_t> info_s_char_t;
typedef  info_array<number_ss_int_t> info_ss_int_t;
typedef  info_array<number_s_int_t> info_s_int_t;
typedef  info_array<number_sl_int_t> info_sl_int_t;
typedef  info_array<number_sll_int_t> info_sll_int_t;
typedef  info_array<number_u_char_t> info_u_char_t;
typedef  info_array<number_us_int_t> info_us_int_t;
typedef  info_array<number_u_int_t> info_u_int_t;
typedef  info_array<number_ul_int_t> info_ul_int_t;
typedef  info_array<number_ull_int_t> info_ull_int_t;
typedef  info_array<number_float_t> info_float_t;
typedef  info_array<number_double_t> info_double_t;
typedef  info_array<number_l_double_t> info_l_double_t;
typedef  info_array<string_string_t> info_string_t;
typedef  array_t<info_child> info_children_t;
//! Zwraca informacje o obiekcie.
class info:public object_t{
public:
    item_t<info_bool_t> info_bool;
    item_t<info_s_char_t> info_s_char;
    item_t<info_ss_int_t> info_ss_int;
    item_t<info_s_int_t> info_s_int;
    item_t<info_sl_int_t> info_sl_int;
    item_t<info_sll_int_t> info_sll_int;
    item_t<info_u_char_t> info_u_char;
    item_t<info_us_int_t> info_us_int;
    item_t<info_u_int_t> info_u_int;
    item_t<info_ul_int_t> info_ul_int;
    item_t<info_ull_int_t> info_ull_int;
    item_t<info_float_t> info_float;
    item_t<info_double_t> info_double;
    item_t<info_l_double_t> info_l_double;
    item_t<info_string_t> info_string;
    item_t<info_children_t> info_children;
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
        ict_data_registerItem(info_children);
    }
};
class info_child:public object_t{
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