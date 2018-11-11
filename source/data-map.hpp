//! @file
//! @brief Data map module - Header file.
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
#ifndef _DATA_MAP_HEADER
#define _DATA_MAP_HEADER
//============================================
#include <map>
#include <vector>
#include <memory>
#include "data-interface.hpp"
//============================================
std::map<int,int> t;
namespace ict { namespace data {
//===========================================
#define CONSTRUCTORS(name)\
name(){}\
explicit name(const Compare& comp,const Alloc& alloc=Alloc()):\
    parent_t(comp,alloc){}\
template<class Input>name(Input first,Input last,const Compare& comp=Compare(),const Alloc& alloc=Alloc()):\
    parent_t(first,last,comp,alloc){}\
name(const name& other):\
    parent_t(other){}\
name(const name& other,const Alloc& alloc):\
    parent_t(other,alloc){}\
name(name&& other):\
    parent_t(other){}\
name(name&& other,const Alloc& alloc):\
    parent_t(other,alloc){}\
name(std::initializer_list<Tp> init,const Compare& comp=Compare(),const Alloc& alloc=Alloc()):\
    parent_t(init,comp,alloc){}\
name(const std::map<Key,Tp,Compare,Alloc>& other):\
    parent_t(other){}\
name(const std::map<Key,Tp,Compare,Alloc>& other,const Alloc& alloc):\
    parent_t(other,alloc){}\
name(std::map<Key,Tp,Compare,Alloc>&& other):\
    parent_t(other){}\
name(std::map<Key,Tp,Compare,Alloc>&& other,const Alloc& alloc):\
    parent_t(other,alloc){}
//===========================================
template <
    typename Key, 
    typename Tp, 
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key,Tp>> 
>
class map_template: public std::map<Key,Tp,Compare,Alloc>{
public:
    typedef std::map<Key,Tp,Compare,Alloc> parent_t;
    typedef typename parent_t::key_type key_type;
    typedef typename parent_t::mapped_type mapped_type;
    typedef typename parent_t::value_type value_type;
    typedef typename parent_t::key_compare key_compare;
    typedef typename parent_t::allocator_type allocator_type;
    typedef typename parent_t::pointer pointer;
    typedef typename parent_t::const_pointer const_pointer;
    typedef typename parent_t::reference reference;
    typedef typename parent_t::const_reference const_reference;
    typedef typename parent_t::iterator iterator;
    typedef typename parent_t::const_iterator const_iterator;
    typedef typename parent_t::size_type size_type;
    typedef typename parent_t::difference_type difference_type;
    typedef typename parent_t::reverse_iterator reverse_iterator;
    typedef typename parent_t::const_reverse_iterator const_reverse_iterator;
protected:
    typedef typename std::vector<iterator> list_t;
    list_t list;
    bool insert_front(const key_type & key){
        std::size_t s=parent_t::size();
        mapped_type & out(parent_t::operator[](key));
        if (s<parent_t::size()) {
            list.emplace(list.begin());
            list.front()=parent_t::lower_bound(key);
            return(true);
        }
        return(false);
    }
    bool insert_back(const key_type & key){
        std::size_t s=parent_t::size();
        mapped_type & out(parent_t::operator[](key));
        if (s<parent_t::size()) {
            list.emplace_back();
            list.back()=parent_t::lower_bound(key);
            return(true);
        }
        return(false);
    }
public:
    CONSTRUCTORS(map_template)
    mapped_type & operator[](const key_type & key){
        std::size_t s=parent_t::size();
        mapped_type & out(parent_t::operator[](key));
        if (s<parent_t::size()) {
            list.emplace_back();
            list.back()=parent_t::lower_bound(key);
        }
        return(out);
    }
    mapped_type & operator[](key_type & key){
        std::size_t s=parent_t::size();
        mapped_type & out(parent_t::operator[](key));
        if (s<parent_t::size()) {
            list.emplace_back();
            list.back()=parent_t::lower_bound(key);            
        }
        return(out);
    }
    void clear() {
        list.clear();
        parent_t::clear();
    }
    std::pair<iterator,bool> insert(const value_type& value){
        std::pair<iterator,bool> out(parent_t::insert(value));
        if (out.second) {
            list.emplace_back();
            list.back()=parent_t::lower_bound(out.first); 
        }
        return(out);
    }
    template<class P> std::pair<iterator,bool> insert(P&& value){
        std::pair<iterator,bool> out(parent_t::insert(value));
        if (out.second) {
            list.emplace_back();
            list.back()=parent_t::lower_bound(out.first); 
        }
        return(out);
    }
    iterator insert(const_iterator hint, const value_type& value){
        iterator out(parent_t::insert(hint,value));
        if (out.second) {
            list.emplace_back();
            list.back()=parent_t::lower_bound(out); 
        }
        return(out);
    }
    template<class P> iterator insert(const_iterator hint, P&& value){
        std::size_t s=parent_t::size();
        iterator out(parent_t::insert(hint,value));
        if (s<parent_t::size()) {
            list.emplace_back();
            list.back()=out;
        }
        return(out);
    }
    void insert(std::initializer_list<value_type> ilist){
        for (typename std::initializer_list<value_type>::const_iterator it=ilist.cbegin();it!=ilist.cend();++it)
           insert(*it);
    }
    template<class... Args> std::pair<iterator,bool> emplace(Args&&... args){
        std::pair<iterator,bool> out(parent_t::emplace(args...));
        if (out.second) {
            list.emplace_back();
            list.back()=out.first;
        }
        return(out);
    }
    template <class... Args> iterator emplace_hint(const_iterator hint,Args&&... args){
        std::size_t s=parent_t::size();
        iterator out(parent_t::emplace_hint(hint,args...));
        if (s<parent_t::size()) {
            list.emplace_back();
            list.back()=out;
        }
        return(out);
    }
    iterator erase(const_iterator pos){
        std::size_t s=parent_t::size();
        iterator out(parent_t::erase(pos));
        if (parent_t::size()<s) 
            for (typename list_t::iterator it=list.begin();it!=list.end();++it) 
                if (it->first==out->first) {
                    list.erase(it);
                    break;
                }
        return(out);
    }
    iterator erase(iterator pos){
        std::size_t s=parent_t::size();
        iterator out(parent_t::erase(pos));
        if (parent_t::size()<s) 
            for (typename list_t::iterator it=list.begin();it!=list.end();++it) 
                if (it->fisrt==out->first) {
                    list.erase(it);
                    break;
                }
        return(out);
    }
    iterator erase(const_iterator first, const_iterator last){
        for (const_iterator i=first;i!=last;++i)
            for (typename list_t::iterator it=list.begin();it!=list.end();++it) 
                if (it->first==i->first) {
                    list.erase(it);
                    break;
                }
        return(parent_t::erase(first,last));
    }
    size_type erase(const key_type& key){
        size_type out(parent_t::erase(key));
        if (out)
           for (typename list_t::iterator it=list.begin();it!=list.end();++it) 
                if ((*it)->first==key) {
                    list.erase(it);
                    break;
                }
        return(out);
    }
    void swap(map_template & other){
        list.swap(other.list);
        swap(other);
    }
};
template <
    typename Key, 
    typename Tp, 
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key,Tp>> 
>
class array_map_t: public map_template<Key,Tp,Compare,Alloc>,public complex_interface {
private:
    typedef map_template<Key,Tp,Compare,Alloc> parent_t;
    typedef typename std::unique_ptr<Key> ptr_t;
    ptr_t begin;
    ptr_t end;
public:
    CONSTRUCTORS(array_map_t)
    void clear() {
        parent_t::clear();
        begin.reset(nullptr);
        end.reset(nullptr);   
    }
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        clear();
    }
    //! Patrz: interface::data_pushFront()
    bool data_pushFront(const std::string & tag=""){
        if (begin){
            parent_t::insert_front(*begin);
            begin.reset(nullptr);
        } else {
            begin.reset(new Key);
        }
        return(true);
    } 
    //! Patrz: interface::data_pushBack()
    bool data_pushBack(const std::string & tag=""){
        if (end){
            parent_t::insert_back(*end);
            end.reset(nullptr);
        } else {
            end.reset(new Key);
        }
        return(true);
    }
    //! Patrz: interface::data_getType()
    data_t data_getType() const {
        return(data_array_map);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        std::size_t s=parent_t::list.size()<<1;
        if (begin) s++;
        if (end) s++;
        return(s);
    }
    //! Patrz: interface::data_getValue()
    interface & data_getValue(const std::size_t & index) {
        std::size_t i=index;
        if (begin){
            if (i) {
                i--;
            } else { 
                return(*begin);
            }
        }
        {
            std::size_t j=i>>1;
            if (j<parent_t::list.size()){
                if (i&0x1){
                    return(parent_t::list[j]->second);
                } else {
                    throw std::range_error("It must be const value to get a key!");
                }
            } else if (j==parent_t::list.size()){
                if (end) return(*end);
            }
        }
        throw std::range_error("No value!");
    }
    const interface & data_getValue(const std::size_t & index) const {
        std::size_t i=index;
        if (begin){
            if (i) {
                i--;
            } else { 
                return(*begin);
            }
        }
        {
            std::size_t j=i>>1;
            if (j<parent_t::list.size()){
                if (i&0x1){
                    return(parent_t::list[j]->second);
                } else {
                    return(parent_t::list[j]->first);
                }
            } else if (j==parent_t::list.size()){
                if (end) return(*end);
            }
        }
        throw std::range_error("No value!");
    }
};
template <
    typename Tp, 
    typename Compare = std::less<std::string>,
    typename Alloc = std::allocator<std::pair<const std::string,Tp>> 
>
class object_map_t: public map_template<std::string,Tp,Compare,Alloc>,public complex_interface {
public:
    using Key=std::string;
private:
    typedef map_template<std::string,Tp,Compare,Alloc> parent_t;
public:
    CONSTRUCTORS(object_map_t)
    //! Patrz: interface::data_clear()
    void data_clear(const std::string & tag=""){
        if (tag.size()){
            parent_t::erase(tag);
        } else {
            parent_t::clear();
        }
    }
    //! Patrz: interface::data_pushFront()
    bool data_pushFront(const std::string & tag=""){
        return(parent_t::insert_front(tag));
    }
    //! Patrz: interface::data_pushBack()
    bool data_pushBack(const std::string & tag=""){
        return(parent_t::insert_back(tag));
    }
    //! Patrz: interface::data_getType()
    data_t data_getType() const {
        return(data_object_map);
    }
    //! Patrz: interface::data_getSize()
    std::size_t data_getSize() const {
        return(parent_t::size());
    }
    //! Patrz: interface::data_getValue()
    interface & data_getValue(const std::size_t & index) {
        if (index<parent_t::list.size()){
            return(parent_t::list[index]->second);
        }
        throw std::range_error("No value!");
    }
    const interface & data_getValue(const std::size_t & index) const {
        if (index<parent_t::list.size()){
            return(parent_t::list[index]->second);
        }
        throw std::range_error("No value!");    
    }
};
//===========================================
template <
    typename Key, 
    typename Tp, 
    typename Compare = std::less<Key>,
    typename Alloc = std::allocator<std::pair<const Key,Tp>> 
>
using map = array_map_t<Key,Tp,Compare,Alloc>;
//===========================================
#undef CONSTRUCTORS
//===========================================
} }
//===========================================
#endif