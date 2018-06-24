//! @file
//! @brief Data interface module - Source file.
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
//============================================
#include "data-interface.hpp"
#include <typeindex>
#include <mutex>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#include "random.hpp"
#endif
//============================================
namespace ict { namespace data {
//============================================
struct object_struct_t {
    typedef std::map<std::string,object_t::item_offset_t> name_offset_t;
    typedef std::map<object_t::item_offset_t,std::string> offset_name_t;
    //! Mapa nazwa i ofset.
    name_offset_t name_offset;
    //! Mapa ofset i nazwa.
    offset_name_t offset_name;    
};
//============================================
typedef std::map<std::type_index,object_struct_t> map_info_t;
static object_struct_t & get_object_struct(const std::type_info & type){
  static map_info_t map_info;
  return(map_info[std::type_index(type)]);
}
static std::mutex & get_object_mutex(){
  static std::mutex mutex;
  return(mutex);
}
static object_t::item_ptr_t offset2pointer(interface * self,const object_t::item_offset_t & offset){
  char* pointer=((char*)self)+offset;
  return((object_t::item_t<interface>*)pointer);
}
static object_t::item_offset_t pointer2offset(interface * self,object_t::item_ptr_t pointer){
  object_t::item_offset_t offset=((char*)pointer)-((char*)self);
  return(offset);
}
//============================================
object_t::data_init::data_init(interface * self,const std::type_info & type, const object_t::item_map_t & map){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(type));
  for (object_t::item_map_t::const_iterator it=map.begin();it!=map.end();++it){
    object_t::item_offset_t offset(pointer2offset(self,it->second));
    object_struct.name_offset[it->first]=offset;
    object_struct.offset_name[offset]=it->first;
  }
}
void object_t::data_clear(){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  for (object_struct_t::offset_name_t::const_iterator it=object_struct.offset_name.cbegin();it!=object_struct.offset_name.cend();++it){
    item_ptr_t item=offset2pointer(this,it->first);
    if (item) {
      item->value.clear();
      item->value.shrink_to_fit();
    }
  }
  list_vector.clear();
}
bool object_t::data_pushFront(const std::string & tag){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (object_struct.name_offset.count(tag)){
    item_ptr_t item=offset2pointer(this,object_struct.name_offset.at(tag));
    if (item) {
      item->value.emplace_back();
      item->value.shrink_to_fit();
      list_vector.emplace(list_vector.begin());
      list_vector.front().offset=object_struct.name_offset.at(tag);
      list_vector.front().index=item->value.size()-1;
    }
    return(true);
  }
  return(false);
}
bool object_t::data_pushBack(const std::string & tag){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (object_struct.name_offset.count(tag)){
    item_ptr_t item=offset2pointer(this,object_struct.name_offset.at(tag));
    if (item) {
      item->value.emplace_back();
      item->value.shrink_to_fit();
      list_vector.emplace_back();
      list_vector.back().offset=object_struct.name_offset.at(tag);
      list_vector.back().index=item->value.size()-1;
    }
    return(true);
  }
  return(false);
}
std::string object_t::data_getTag(const std::size_t & index) const{
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (index<list_vector.size()){
    return(object_struct.offset_name.at(list_vector.at(index).offset));
  }
  throw std::invalid_argument("Index out of range [1]!");
  return("");
}
interface & object_t::data_getValue(const std::size_t & index){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (index<list_vector.size()){
    return(offset2pointer(this,list_vector.at(index).offset)->value[list_vector.at(index).index]);
  }
  throw std::invalid_argument("Index out of range [2]!");
  return(*this);
}
void object_t::data_pushFront(item_ptr_t item){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (item) {
    object_t::item_offset_t item_offset=pointer2offset(this,item);
    if (object_struct.offset_name.count(item_offset)){
      item->value.emplace_back();
      item->value.shrink_to_fit();
      list_vector.emplace(list_vector.begin());
      list_vector.front().offset=item_offset;
      list_vector.front().index=item->value.size()-1;
      return;
    }
    throw std::invalid_argument("Missing item [1]!");
  } else {
    throw std::invalid_argument("Missing item [2]!");
  }
}
void object_t::data_pushBack(item_ptr_t item){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (item) {
    object_t::item_offset_t item_offset=pointer2offset(this,item);
    if (object_struct.offset_name.count(item_offset)){
      item->value.emplace_back();
      item->value.shrink_to_fit();
      list_vector.emplace_back();
      list_vector.back().offset=item_offset;
      list_vector.back().index=item->value.size()-1;
      return;
    }
    throw std::invalid_argument("Missing item [1]!");
  } else {
    throw std::invalid_argument("Missing item [2]!");
  }
}
void object_t::data_clear(item_ptr_t item){
  std::unique_lock<std::mutex> lock (get_object_mutex());
  object_struct_t & object_struct(get_object_struct(typeid(*this)));
  if (item) {
    object_t::item_offset_t item_offset=pointer2offset(this,item);
    if (object_struct.offset_name.count(item_offset)){
      item->value.clear();
      item->value.shrink_to_fit();
      list_vector_t new_vector(list_vector);
      list_vector.clear();
      for (const item_list_t & i : new_vector) if (i.offset!=item_offset) {
        list_vector.emplace_back(i);
      }
      return;
    }
    throw std::invalid_argument("Missing item [1]!");
  } else {
    throw std::invalid_argument("Missing item [2]!");
  }
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
/*
REGISTER_TEST(data_interface,tc1){
  try{
    ict::buffer::basic b; 
    signed char in1,out1;
    signed short int in2,out2;
    signed int in3,out3;
    signed long int in4,out4;
    signed long long int in5,out5;
    unsigned char in6,out6;
    unsigned short int in7,out7;
    unsigned int in8,out8;
    unsigned long int in9,out9;
    unsigned long long int in10,out10;
    float in11,out11;
    double in12,out12;
    long double in13,out13;
    bool in14,out14;
    std::string in15,out15;
    std::wstring in16,out16;
    ict::buffer::byte_vector_t in17,out17;
    random(in1);
    random(in2);
    random(in3);
    random(in4);
    random(in5);
    random(in6);
    random(in7);
    random(in8);
    random(in9);
    random(in10);
    random(in11);
    random(in12);
    random(in13);
    random(in14);
    in15=ict::random::randomString(100);
    test(in1,out1,b);
    test(in2,out2,b);
    test(in3,out3,b);
    test(in4,out4,b);
    test(in5,out5,b);
    test(in6,out6,b);
    test(in7,out7,b);
    test(in8,out8,b);
    test(in9,out9,b);
    test(in10,out10,b);
    test(in11,out11,b);
    test(in12,out12,b);
    test(in13,out13,b);
    test(in14,out14,b);
    out15.resize(in15.size());
    test(in15,out15,b);
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
*/
#endif
//===========================================
