//! @file
//! @brief Buffer module - Source file.
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
#include "buffer.hpp"
#include <stdexcept>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace buffer {
//============================================
namespace _internal {
//============================================
template<typename T> bool testPlus(const T & input,const vector & output){
    return(output.getFreeSpace()<sizeof(T)); 
}
template<typename T> bool testMinus(const vector & input,const T & output){
    return(input.getSize()<sizeof(T));
}
template<class T> bool testPlusArray(const T & input,const vector & output){
    return(output.getFreeSpace()<(sizeof(input.back())*input.size())); 
}
template<class T> bool testMinusArray(const vector & input,const T & output){
    return(input.getSize()<(sizeof(output.back())*output.max_size()));
}
template<typename T> void plus(const T & input,vector & output){
    const byte_t* ptr=(byte_t*)(&input);
    if (testPlus(input,output)) throw std::range_error("Input to large for ict::buffer::vector [1]!");
    for (std::size_t k=0;k<sizeof(T);k++) output.push_back(ptr[k]); 
}
template<typename T> void minus(vector & input,T & output){
    byte_t* ptr=(byte_t*)(&output);
    if (testMinus(input,output)) throw std::range_error("Output to small for ict::buffer::vector [1]!");
    for (std::size_t k=0;k<sizeof(T);k++) ptr[k]=input[k];
    input.erase(input.begin(),input.begin()+sizeof(T));
}
template<class T> void plusArray(const T & input,vector & output){
    if (testPlusArray(input,output)) throw std::range_error("Input to large for ict::buffer::vector [2]!");
    for (std::size_t k=0;k<input.size();k++) plus(input[k],output);
}
template<class T> void minusArray(vector & input,T & output){
    if (testMinusArray(input,output)) throw std::range_error("Output to small for ict::buffer::vector [2]!");
    for (;input.size();input.pop_back()) minus(input,output.back());
}
//============================================
}
//============================================
vector::vector():max(max_size()){
}
vector::vector(std::size_t maxSize):max((maxSize<max_size())?maxSize:max_size()){
}
std::size_t vector::getMaxSize() const {
    return(max);
}
std::size_t vector::getFreeSpace() const {
    return(max-size());
}
std::size_t vector::getSize() const {
    return(size());
}

bool vector::testPlus(const signed char & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const signed short int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const signed int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const signed long int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const signed long long int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const unsigned char & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const unsigned short int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const unsigned int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const unsigned long int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const unsigned long long int & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const float & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const double & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const long double & input){return(!_internal::testPlus(input,*this));}
bool vector::testPlus(const bool & input){
    unsigned char in;
    return(!_internal::testPlus(in,*this));
}
bool vector::testPlus(const std::string & input){return(!_internal::testPlusArray(input,*this));}
bool vector::testPlus(const std::wstring & input){return(!_internal::testPlusArray(input,*this));}
bool vector::testPlus(const byte_vector_t & input){return(!_internal::testPlusArray(input,*this));}

vector & vector::operator +=(const signed char & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const signed short int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const signed int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const signed long int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const signed long long int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const unsigned char & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const unsigned int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const unsigned long int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const unsigned long long int & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const float & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const double & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const long double & input){_internal::plus(input,*this);return(*this);}
vector & vector::operator +=(const bool & input){
    unsigned char in;
    in=input;
    _internal::plus(input,*this);
    return(*this);
}
vector & vector::operator +=(const std::string & input){_internal::plusArray(input,*this);return(*this);}
vector & vector::operator +=(const std::wstring & input){_internal::plusArray(input,*this);return(*this);}
vector & vector::operator +=(const byte_vector_t & input){_internal::plusArray(input,*this);return(*this);}

bool vector::testMinus(const signed char & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const signed short int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const signed int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const signed long int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const signed long long int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const unsigned char & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const unsigned short int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const unsigned int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const unsigned long int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const unsigned long long int & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const float & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const double & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const long double & output){return(!_internal::testMinus(*this,output));}
bool vector::testMinus(const bool & output){
    unsigned char out;
    return(!_internal::testMinus(*this,out));
}
bool vector::testMinus(const std::string & output){return(!_internal::testMinusArray(*this,output));}
bool vector::testMinus(const std::wstring & output){return(!_internal::testMinusArray(*this,output));}
bool vector::testMinus(const byte_vector_t & output){return(!_internal::testMinusArray(*this,output));}

vector & vector::operator -=(signed char & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(signed short int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(signed int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(signed long int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(signed long long int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(unsigned char & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(unsigned int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(unsigned long int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(unsigned long long int & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(float & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(double & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(long double & output){_internal::minus(*this,output);return(*this);}
vector & vector::operator -=(bool & output){
    unsigned char out;
    _internal::minus(*this,out);
    output=out;
    return(*this);
}
vector & vector::operator -=(std::string & output){_internal::minusArray(*this,output);return(*this);}
vector & vector::operator -=(std::wstring & output){_internal::minusArray(*this,output);return(*this);}
vector & vector::operator -=(byte_vector_t & output){_internal::minusArray(*this,output);return(*this);}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
/*

REGISTER_TEST(jobject,tc1){
  try{
    std::string b;
    MutableTest mutableTest;
    ArrayTest arrayTest;
    arrayTest.emplace_back();
    arrayTest.emplace_back();
    arrayTest[0].int1()=3;
    arrayTest[0].int2()=65;
    arrayTest[0].int3()=656;
    arrayTest[0].int4()=0x5555;
    arrayTest[0].int5()=55353;
    arrayTest[0].float1()=2.4;
    arrayTest[0].float2()=66.001;
    arrayTest[0].float3()=56.4e3;
    arrayTest[0].bool_v()=true;
    arrayTest[0].null_v();
    arrayTest[0].string_v()="a kuku!";
    arrayTest[1].int1()=34;
    arrayTest[1].int2()=655;
    arrayTest[1].int3()=6576;
    arrayTest[1].int4()=0x55455;
    arrayTest[1].int5()=553453;
    arrayTest[1].float1()=2.47;
    arrayTest[1].float2()=66.0071;
    arrayTest[1].float3()=56.4e-30;
    arrayTest[1].bool_v()=false;
    arrayTest[1].null_v();
    arrayTest[1].string_v()="a kuku2!";

    while(arrayTest[0].int1.serializeJson(b)==1){}
    std::cerr<<"arrayTest[0].int1.serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].float2.serializeJson(b)==1){}
    std::cerr<<"arrayTest[0].float2.serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].bool_v.serializeJson(b)==1){}
    std::cerr<<"arrayTest[0].bool_v.serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].null_v.serializeJson(b)==1){}
    std::cerr<<"arrayTest[0].null_v.serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].string_v.serializeJson(b)==1){}
    std::cerr<<"arrayTest[0].string_v.serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].serializeJson(b)==1){}
    std::cerr<<"arrayTest[0].serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest.serializeJson(b)==1){}
    std::cerr<<"arrayTest.serializeJson(b)>"<<b<<"<"<<std::endl;b.clear();

    while(arrayTest[0].int1.infoJson(b)==1){}
    std::cerr<<"arrayTest[0].int1.infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].float2.infoJson(b)==1){}
    std::cerr<<"arrayTest[0].float2.infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].bool_v.infoJson(b)==1){}
    std::cerr<<"arrayTest[0].bool_v.infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].null_v.infoJson(b)==1){}
    std::cerr<<"arrayTest[0].null_v.infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].string_v.infoJson(b)==1){}
    std::cerr<<"arrayTest[0].string_v.infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest[0].infoJson(b)==1){}
    std::cerr<<"arrayTest[0].infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
    while(arrayTest.infoJson(b)==1){}
    std::cerr<<"arrayTest.infoJson(b)>"<<b<<"<"<<std::endl;b.clear();
  }catch(const std::exception e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}*/

#endif
//===========================================
