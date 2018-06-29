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
#include "mutex.hpp"
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
static map_info_t & get_map_info(){
  static map_info_t map_info;
  return(map_info);
}
static bool get_map_info_test(const std::type_info & type){
  return(get_map_info().count(std::type_index(type)));
}
static object_struct_t & get_map_info_new(const std::type_info & type){
  return(get_map_info()[std::type_index(type)]);
}
static object_struct_t & get_map_info_at(const std::type_info & type){
  return(get_map_info().at(std::type_index(type)));
}
static ict::mutex::read_write & get_object_mutex(){
  static ict::mutex::read_write mutex(true);
  return(mutex);
}
static object_t::item_ptr_t offset2pointer(const interface * self,const object_t::item_offset_t & offset){
  char* pointer=((char*)self)+offset;
  return((object_t::item_t<interface>*)pointer);
}
static object_t::item_offset_t pointer2offset(const interface * self,const void * pointer){
  object_t::item_offset_t offset=((char*)pointer)-((char*)self);
  return(offset);
}
//============================================
void object_t::registerItems()const{
  if (!get_map_info_test(typeid(*this))){
    get_object_mutex().read.unlock();//Na chwilę przerwij doczyt
    {//Rozpocznij zapis
      std::unique_lock<ict::mutex::read_write::write_t> lock (get_object_mutex().write);
      data_registerItems();
    }
    get_object_mutex().read.lock();//Przywróć doczyt
  }
}
void object_t::data_registerItem(const std::string & name,const void * item)const{
  object_struct_t & object_struct(get_map_info_new(typeid(*this)));
  object_t::item_offset_t offset(pointer2offset(this,item));
  object_struct.name_offset[name]=offset;
  object_struct.offset_name[offset]=name;
}
void object_t::data_clear(){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {  
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (!object_struct.offset_name.size()) 
    for (object_struct_t::offset_name_t::const_iterator it=object_struct.offset_name.cbegin();it!=object_struct.offset_name.cend();++it){
      item_ptr_t item=offset2pointer(this,it->first);
      if (item) item->clear();
    }
    list_vector.clear();
  }
}
bool object_t::data_pushFront(const std::string & tag){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (object_struct.name_offset.count(tag)){
      item_ptr_t item=offset2pointer(this,object_struct.name_offset.at(tag));
      if (item) {
        item->emplace_back();
        list_vector.emplace(list_vector.begin());
        list_vector.front().offset=object_struct.name_offset.at(tag);
        list_vector.front().index=item->size()-1;
      }
      return(true);
    }
  }
  return(false);
}
bool object_t::data_pushBack(const std::string & tag){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (object_struct.name_offset.count(tag)){
      item_ptr_t item=offset2pointer(this,object_struct.name_offset.at(tag));
      if (item) {
        item->emplace_back();
        list_vector.emplace_back();
        list_vector.back().offset=object_struct.name_offset.at(tag);
        list_vector.back().index=item->size()-1;
      }
      return(true);
    }
  }
  return(false);
}
std::string object_t::data_getTag(const std::size_t & index) const{
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (index<list_vector.size()){
      return(object_struct.offset_name.at(list_vector.at(index).offset));
    }
  }
  throw std::invalid_argument("Index out of range [1]!");
  return("");
}
interface & object_t::data_getValue(const std::size_t & index){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (index<list_vector.size()){
      return(offset2pointer(this,list_vector.at(index).offset)->get_interface(list_vector.at(index).index));
    }
  }
  throw std::invalid_argument("Index out of range [2]!");
  return(*this);
}
void object_t::data_pushFront(item_ptr_t item){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (item) {
      object_t::item_offset_t item_offset=pointer2offset(this,item);
      if (object_struct.offset_name.count(item_offset)){
        item->emplace_back();
        list_vector.emplace(list_vector.begin());
        list_vector.front().offset=item_offset;
        list_vector.front().index=item->size()-1;
        return;
      }
      throw std::invalid_argument("Missing item [1]!");
    } else {
      throw std::invalid_argument("Missing item [2]!");
    }
  }
}
void object_t::data_pushBack(item_ptr_t item){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (item) {
      object_t::item_offset_t item_offset=pointer2offset(this,item);
      if (object_struct.offset_name.count(item_offset)){
        item->emplace_back();
        list_vector.emplace_back();
        list_vector.back().offset=item_offset;
        list_vector.back().index=item->size()-1;
        return;
      }
      throw std::invalid_argument("Missing item [1]!");
    } else {
      throw std::invalid_argument("Missing item [2]!");
    }
  }
}
void object_t::data_clear(item_ptr_t item){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (item) {
      object_t::item_offset_t item_offset=pointer2offset(this,item);
      if (object_struct.offset_name.count(item_offset)){
        list_vector_t new_vector(list_vector);
        list_vector.clear();
        item->clear();
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
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(data_interface,tc1){
  try{
    ict::data::info i;

    ict_data_pushFront(i,info_double);
    i.info_double().emplace_back();
    ict_data_pushFront(i.info_double()[0],type);
    ict_data_pushFront(i.info_double()[0],value);
    i.info_double[0][0].type[0]()=5;
    i.info_double[0][0].value[0]()=7.009;

    ict_data_pushFront(i,info_string);
    i.info_string().emplace_back();
    ict_data_pushFront(i.info_string()[0],type);
    ict_data_pushFront(i.info_string()[0],value);
    i.info_string[0][0].type[0]()=6;
    i.info_string[0][0].value[0]()="7.009";

    ict_data_pushFront(i,info_children);
    i.info_children().emplace_back();
    ict_data_pushFront(i.info_children()[0],name);
    ict_data_pushFront(i.info_children()[0],value);
    ict_data_pushFront(i.info_children()[0].value(),info_u_int);
    i.info_children[0][0].name[0]()="name";








  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
#endif
//===========================================
