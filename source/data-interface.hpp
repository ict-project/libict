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
#include <map>
#include <stack>
#include <functional>
#include <memory>
#include "buffer.hpp"
#include "data-types.hpp"
//============================================
namespace ict { namespace data {
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
class info;
class interface;
//===========================================
//! Porównwalna wersja std::vector<std::string>
class tags : public std::vector<std::string>,public comparable<tags>{
private:
    int compare(const tags & rhs) const {
        if (size()<rhs.size()) return(-1);
        if (size()>rhs.size()) return(1);
        for (std::size_t k=0;k<size();k++){
            if (at(k).size()&&rhs.at(k).size()) {
                if (at(k)<rhs.at(k)) return(-1);
                if (at(k)>rhs.at(k)) return(1);      
            }
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
    public:
        typedef I pointer;
        typedef iterator_template<I> self_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        typedef std::function<I(I)> streamer_factory;
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
        mutable std::shared_ptr<typename std::remove_pointer<I>::type> streamer;
        mutable std::map<tags,streamer_factory> streamer_map;
        bool reverse_value;
        std::stack<stack_item_t<I>> stack;
        tags tag_vector;
    public:
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
        //! @brief Dodaje nową fabrykę streamerów
        //! 
        //! @param tag Tag, przy którym streamer jest tworzony.
        //! @param factory Fabryka streamerów.
        //! 
        void streamer_insert(const tags & tag,streamer_factory factory){
            streamer_erase(tag);
            streamer_map[tag]=factory;
        }
        //! 
        //! @brief Usuwa fabrykę streamerów.
        //! 
        //! @param tag  Tag, przy którym streamer jest tworzony.
        //! 
        void streamer_erase(const tags & tag){
            if (streamer_map.count(tag)) streamer_map.erase(tag);
        }
        //! 
        //! @brief Czyści fabryki streamerów.
        //! 
        void streamer_clear(){
            streamer_map.clear();
        }
        //! 
        //! @brief Zwraca wskaźnik aktualnego obiektu.
        //! 
        //! @return pointer Wskaźnik aktualnego obiektu.
        //! 
        pointer ptr() const {
            if (stack.empty()) throw std::range_error("No value [1]!");
            if (!streamer) //Jeśli streamer pusty
                if (streamer_map.size()) //Jeśli są fabryki streamerów
                    if (streamer_map.count(tag_vector)){ //Jeśli jest własciwa fabryka streamerów
                        streamer.reset(streamer_map[tag_vector](stack.top().ptr));
                    }
            if (streamer) return(streamer.get());
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
            bool streamer_present(streamer);
            if (!streamer_present) //Jeśli streamer pusty
                if (streamer_map.size()) //Jeśli są fabryki streamerów
                    if (streamer_map.count(tag_vector)){ //Jeśli jest własciwa fabryka streamerów
                        streamer_present=true;
                    }
            while(!stack.empty()){
                stack_item_t<I> & item(stack.top());
                data_t type=item.ptr->data_getType();
                if (streamer_present) {//Jeśli jest streamer
                    streamer.reset();//Usuń streamer
                    pop();//Zdejmij element
                    streamer_present=false;
                } else if (complex_type(type)){//Typy złożone
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
} }
//===========================================
#endif