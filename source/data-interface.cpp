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
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#include "random.hpp"
#endif
//============================================
namespace ict { namespace data {
//============================================
interface * string_object_t::data_getPropPointer(const data_offset_t & offset){
  char * pointer=((char*)this)+offset;
  return((interface*)pointer);
}
std::size_t string_object_t::data_getPropOffset(interface * pointer){
  std::size_t offset=((char*)pointer)-((char*)this);
  return(offset);
}
void string_object_t::data_registerProp(interface * element,const std::string & name){
  if (element){
    std::size_t offset=data_getPropOffset(element);
    data_name_offset[name]=offset;
    data_offset_name[offset]=name;
  }
}
void string_object_t::data_hideProp(const data_offset_t & offset){
  if (data_offset_name.count(offset)){
    data_offset_vector_t tmp;
    for (const std::size_t & item : data_visible_prop){
      if (item!=offset) tmp.push_back(item);
    }
    data_visible_prop=tmp;
  }
}
void string_object_t::data_hideProp(interface * element){
  if (element) data_hideProp(data_getPropOffset(element));
}
void string_object_t::data_hideProp(const std::string & name){
  if (data_name_offset.count(name)) data_hideProp(data_name_offset.at(name));
}
void string_object_t::data_showProp(const data_offset_t & offset){
  if (data_offset_name.count(offset)){
    if (!data_isPropPresent(offset)){
      data_visible_prop.push_back(offset);
    }
  }
}
void string_object_t::data_showProp(interface * element){
  if (element) data_showProp(data_getPropOffset(element));
}
void string_object_t::data_showProp(const std::string & name){
  if (data_name_offset.count(name)) data_showProp(data_name_offset.at(name));
}
void string_object_t::data_hideAllProp(){
  data_visible_prop.clear();
}
void string_object_t::data_showAllProp(){
  for (data_name_offset_t::const_iterator it=data_name_offset.cbegin();it!=data_name_offset.cend();++it){
    if (!data_isPropPresent(it->second)) data_visible_prop.push_back(it->second);
  }  
}
bool string_object_t::data_isPropPresent(const data_offset_t & offset){
  for (const data_offset_t & item : data_visible_prop){
    if (item==offset) return(true);
  }
  return(false);
}
bool string_object_t::data_isPropPresent(interface * element){
  if (element){
    return(data_isPropPresent(data_getPropOffset(element)));
  }
  return(false);
}
bool string_object_t::data_isPropPresent(const std::string & name){
  if (data_name_offset.count(name)){
    return(data_isPropPresent(data_name_offset.at(name)));
  }
  return(false);
}
std::string string_object_t::data_getTag(const std::size_t & index) const {
  if (index<data_visible_prop.size()){
    return(data_offset_name.at(data_visible_prop.at(index)));
  }
  throw std::invalid_argument("Index out of range [1]!");
  return("");
}
interface & string_object_t::data_getValue(const std::size_t & index){
  if (index<data_visible_prop.size()){
    return(*data_getPropPointer(data_visible_prop.at(index)));
  }
  throw std::invalid_argument("Index out of range [2]!");
  return(*this);
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
