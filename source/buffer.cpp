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
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace buffer {
//============================================
const std::size_t max_size(0xffffffffffffffff);
void basic::byteIn(const byte_t & byte){
    q.push(byte);
}
void basic::byteOut(byte_t & byte){
    byte=q.front();
    q.pop();
}
basic::basic():max(max_size){}
basic::basic(const std::size_t & maxSize):max((maxSize<max_size)?maxSize:max_size){}
std::size_t basic::getMaxSize() const{return(max);}
std::size_t basic::getFreeSpace() const{return(max-q.size());}
std::size_t basic::getSize() const{return(q.size());}
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
