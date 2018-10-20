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
    typedef std::map<std::string,object_object_t::item_offset_t> name_offset_t;
    typedef std::map<object_object_t::item_offset_t,std::string> offset_name_t;
    //! Mapa nazwa i ofset.
    name_offset_t name_offset;
    //! Mapa ofset i nazwa.
    offset_name_t offset_name;    
};
//============================================
#define setInfo_m(p,t) \
void interface::data_setInfo(info & output,number_u_int_type type,const p##t##_type & value) { \
    if (!output.info_params.size()) output.data_pushBack("info_params"); \
    output.info_params().emplace_back(); \
    {\
      auto & back(output.info_params().back());\
      back.data_pushBack("type");\
      back.type()=type;\
      back.data_pushBack("info_" #t);\
      back.info_##t()=value;\
    }\
}
setInfo_m(,bool)
setInfo_m(number_,s_char)
setInfo_m(number_,ss_int)
setInfo_m(number_,s_int)
setInfo_m(number_,sl_int)
setInfo_m(number_,sll_int)
setInfo_m(number_,u_char)
setInfo_m(number_,us_int)
setInfo_m(number_,u_int)
setInfo_m(number_,ul_int)
setInfo_m(number_,ull_int)
setInfo_m(number_,float)
setInfo_m(number_,double)
setInfo_m(number_,l_double)
setInfo_m(string_,string)
#undef setInfo_m
void interface::data_getInfoMain(info & output) const {
  data_setInfo(output,info_types::data_type,type_name(data_getType()));
  data_setInfo(output,info_types::data_type,data_getType());
  data_setInfo(output,info_types::class_name,typeid(*this).name());  
}
std::string interface::data_getTag(const std::size_t & index) const{
  return(std::to_string(index));
}
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
static object_object_t::item_ptr_t offset2pointer(const interface * self,const object_object_t::item_offset_t & offset){
  char* pointer=((char*)self)+offset;
  return((object_object_t::item_t<interface>*)pointer);
}
static object_object_t::item_offset_t pointer2offset(const interface * self,const void * pointer){
  object_object_t::item_offset_t offset=((char*)pointer)-((char*)self);
  return(offset);
}
//============================================
void object_object_t::registerItems()const{
  if (!get_map_info_test(typeid(*this))){
    get_object_mutex().read.unlock();//Na chwilę przerwij doczyt
    {//Rozpocznij zapis
      std::unique_lock<ict::mutex::read_write::write_t> lock (get_object_mutex().write);
      data_registerItems();
    }
    get_object_mutex().read.lock();//Przywróć doczyt
  }
}
void object_object_t::data_registerItem(const std::string & name,const void * item)const{
  object_struct_t & object_struct(get_map_info_new(typeid(*this)));
  object_object_t::item_offset_t offset(pointer2offset(this,item));
  object_struct.name_offset[name]=offset;
  object_struct.offset_name[offset]=name;
}
void object_object_t::data_clear(const std::string & tag){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  if (tag.size()){
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (object_struct.name_offset.count(tag)){
      object_object_t::item_offset_t item_offset=object_struct.name_offset.at(tag);
      item_ptr_t item=offset2pointer(this,item_offset);
      if (item) {
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
    } else {
      throw std::invalid_argument("Missing item [3]!");
    }
  } else {  
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    for (object_struct_t::offset_name_t::const_iterator it=object_struct.offset_name.cbegin();it!=object_struct.offset_name.cend();++it){
      item_ptr_t item=offset2pointer(this,it->first);
      if (item) item->clear();
    }
    list_vector.clear();
  }
}
bool object_object_t::data_pushFront(const std::string & tag){
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
bool object_object_t::data_pushBack(const std::string & tag){
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
std::string object_object_t::data_getTag(const std::size_t & index) const{
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
interface & object_object_t::data_getValue(const std::size_t & index){
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
const interface & object_object_t::data_getValue(const std::size_t & index) const{
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
void object_object_t::data_pushFront(item_ptr_t item){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (item) {
      object_object_t::item_offset_t item_offset=pointer2offset(this,item);
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
void object_object_t::data_pushBack(item_ptr_t item){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (item) {
      object_object_t::item_offset_t item_offset=pointer2offset(this,item);
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
void object_object_t::data_clear(item_ptr_t item){
  std::unique_lock<ict::mutex::read_write::read_t> lock (get_object_mutex().read);
  registerItems();
  {
    object_struct_t & object_struct(get_map_info_at(typeid(*this)));
    if (item) {
      object_object_t::item_offset_t item_offset=pointer2offset(this,item);
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
void test_info_data(ict::data::info & i){
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
const std::vector<ict::data::data_t> types_order={
  ict::data::data_object_object,//1,0,
    ict::data::data_array_vector,//2,0,info_children
      ict::data::data_object_object,//3,0,info_children.0
        ict::data::data_object_object,//4,0,info_children.0.value
          ict::data::data_array_vector,//5,0,info_children.0.value.info_params
            ict::data::data_object_object,//6,0,info_children.0.value.info_params.0
              ict::data::data_number_us_int,//7,0,info_children.0.value.info_params.0.type
            ict::data::data_object_object,//6,1,info_children.0.value.info_params.0
              ict::data::data_number_us_int,//7,0,info_children.0.value.info_params.0.info_u_int
            ict::data::data_object_object,//6,2,info_children.0.value.info_params.0
          ict::data::data_array_vector,//5,1,info_children.0.value.info_params
        ict::data::data_object_object,//4,1,info_children.0.value
      ict::data::data_object_object,//3,1,info_children.0
        ict::data::data_string_string,//4,0,info_children.0.name
      ict::data::data_object_object,//3,2,info_children.0
    ict::data::data_array_vector,//2,1,info_children
  ict::data::data_object_object,//1,1,
    ict::data::data_array_vector,//2,0,info_params
      ict::data::data_object_object,//3,0,info_params.0
        ict::data::data_number_us_int,//4,0,info_params.0.type
      ict::data::data_object_object,//3,1,info_params.0
        ict::data::data_number_double,//4,0,info_params.0.info_double
      ict::data::data_object_object,//3,2,info_params.0
    ict::data::data_array_vector,//2,1,info_params
      ict::data::data_object_object,//3,0,info_params.1
        ict::data::data_number_us_int,//4,0,info_params.1.type
      ict::data::data_object_object,//3,1,info_params.1
        ict::data::data_bool,//4,0,info_params.1.info_bool
      ict::data::data_object_object,//3,2,info_params.1
    ict::data::data_array_vector,//2,2,info_params
      ict::data::data_object_object,//3,0,info_params.2
        ict::data::data_number_us_int,//4,0,info_params.2.type
      ict::data::data_object_object,//3,1,info_params.2
        ict::data::data_string_string,//4,0,info_params.2.info_string
      ict::data::data_object_object,//3,2,info_params.2
    ict::data::data_array_vector,//2,3,info_params
  ict::data::data_object_object//1,2,
};
REGISTER_TEST(data_interface,tc1){
  try{
    ict::data::info i;
    test_info_data(i);
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc2){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    for (auto it=i.data_begin();it!=i.data_end();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      if (k<types_order.size()){
        std::size_t l=k;
        if (it->data_getType()!=types_order.at(l)){
          std::cerr<<"ERROR: it->data_getType()!=types_order.at(l) l="<<l<<" ("<<type_name(it->data_getType())<<"!="<<type_name(types_order.at(l))<<")!"<<std::endl;
          return(1);
        }
      }
      k++;
    }
    if (k!=types_order.size()){
      std::cerr<<"ERROR: k!=types_order.size() ("<<k<<"!="<<types_order.size()<<")!"<<std::endl;
      return(1);
    }
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc3){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    for (auto it=i.data_rbegin();it!=i.data_rend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      if (k<types_order.size()){
        std::size_t l=types_order.size()-k-1;
        if (it->data_getType()!=types_order.at(l)){
          std::cerr<<"ERROR: it->data_getType()!=types_order.at(l) l="<<l<<" ("<<type_name(it->data_getType())<<"!="<<type_name(types_order.at(l))<<")!"<<std::endl;
          return(1);
        }
      }
      k++;
    }
    if (k!=types_order.size()){
      std::cerr<<"ERROR: k!=types_order.size() ("<<k<<"!="<<types_order.size()<<")!"<<std::endl;
      return(1);
    }
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc4){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    for (auto it=i.data_cbegin();it!=i.data_cend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      k++;
    }
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc5){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    for (auto it=i.data_crbegin();it!=i.data_crend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      k++;
    }
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc6){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    i.data_clear();
    for (auto it=i.data_cbegin();it!=i.data_cend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      k++;
    }
    if (k!=1){
      std::cerr<<"ERROR: k!=1 ("<<k<<"!="<<1<<")!"<<std::endl;
      return(1);
    }
   }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc7){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    i.data_clear("info_children");
    for (auto it=i.data_cbegin();it!=i.data_cend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      k++;
    }
    if (k!=21){
      std::cerr<<"ERROR: k!=21 ("<<k<<"!="<<21<<")!"<<std::endl;
      return(1);
    }
   }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
REGISTER_TEST(data_interface,tc8){
  try{
    std::size_t k=0;
    ict::data::info i;
    test_info_data(i);
    i.data_clear(&i.info_children);
    for (auto it=i.data_cbegin();it!=i.data_cend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      k++;
    }
    if (k!=21){
      std::cerr<<"ERROR: k!=21 ("<<k<<"!="<<21<<")!"<<std::endl;
      return(1);
    }
   }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
struct test_data_info: public ict::data::info{
  void test(ict::data::info & info){
    data_getInfoMain(info);
  }
};
REGISTER_TEST(data_interface,tc9){
  try{
    std::size_t k=0;
    test_data_info i;
    i.test(i);
    for (auto it=i.data_cbegin();it!=i.data_cend();++it){
      std::cout<<type_name(it->data_getType())<<","<<it.level()<<","<<it.step()<<","<<it.getTags().string()<<std::endl;
      k++;
    }
    if (k!=21){
      std::cerr<<"ERROR: k!=21 ("<<k<<"!="<<21<<")!"<<std::endl;
      return(1);
    }
   }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
#endif
//===========================================
