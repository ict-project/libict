//! @file
//! @brief Data object module - Header file.
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
#ifndef _DATA_OBJECT_HEADER
#define _DATA_OBJECT_HEADER
//============================================
#include "data-interface.hpp"
//============================================
namespace ict { namespace data {
//===========================================
class object_object_t:public complex_interface{
public:
    class item_interface_t {
        friend class object_object_t;
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
} }
//===========================================
#endif