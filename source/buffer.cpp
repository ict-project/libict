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
#include "random.hpp"
#endif
//============================================
namespace ict { namespace buffer {
//============================================
void size_type::clear(){
    is_ready=true;
    bytes.clear();
    bytes.emplace_back();
    bytes.back()=0x0;
    size=0x0;
}
size_type & size_type::operator()(const std::size_t & input){
    std::size_t s(input);
    bytes.clear();
    do {
        bytes.emplace_back();
        bytes.back()=(s&mask)|(~mask);
        s>>=0x7;
    } while (s);
    bytes.back()=bytes.back()&mask;
    size=input;
    is_ready=true;
    return(*this);
}
size_type & size_type::operator()(const byte_t & input){
    if (is_ready){
        is_ready=false;
        bytes.clear();
        size=0x0;
    }
    bytes.emplace_back();
    bytes.back()=input;
    if (!(input&(~mask))) is_ready=true;
    if (bytes.size()>sizeof(size)) is_ready=true;
    if (is_ready) for (byte_vector_t::const_reverse_iterator it=bytes.crbegin();it!=bytes.crend();++it){
      size<<=0x7;
      size|=((*it)&mask);
    }
    return(*this);
}
//===========================================
bool byte_interface::testInput(const size_type & input) const{
  return(getFreeSpace()<input.getBytes().size());
}
bool byte_interface::testOutput(const size_type & output) const{
  try{
    getArraySize();
  }catch(...){
    return(true);
  }
  return(false);
}
void byte_interface::dataIn(const size_type & input,bool test){
  const byte_vector_t & buffer(input.getBytes());
  if (test) if (testInput(input)) throw std::range_error("Input to large for ict::buffer::interface [3]!");
  for (const byte_t b : buffer) {
    dataIn(b);
  }
}
void byte_interface::dataOut(size_type & output,bool test){
  byte_t b;
  if (test) if (testOutput(output)) throw std::range_error("Output to small for ict::buffer::interface [3]!");
  do {
    dataOut(b,false);
  } while (!output(b).isReady());
}
//===========================================
const std::size_t max_size(0xffffffffffffffff);
void basic::byteIn(const byte_t & byte){
    q.push_back(byte);
}
void basic::byteOut(byte_t & byte){
    byte=q.front();
    q.pop_front();
}
size_type basic::getArraySize() const {
  size_type s;
  for (const byte_t & b : q){
    if (s(b).isReady()) return(s);
  }
  throw new std::exception;
}
basic::basic():max(max_size){}
basic::basic(const std::size_t & maxSize):max((maxSize<max_size)?maxSize:max_size){}
std::size_t basic::getMaxSize() const {return(max);}
std::size_t basic::getFreeSpace() const {return(max-q.size());}
std::size_t basic::getSize() const {return(q.size());}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
struct test_struct{
    ict::buffer::byte_vector_t bytes;
    std::size_t size;
};
template <typename I=int> void random(I & output){
  output=ict::random::randomInteger(0,(0x1<<15));
}
REGISTER_TEST(buffer,tc1){
  try{
    static const std::vector<test_struct> tv={
      {{0b0},                                                  0b0},
      {{0b1},                                                  0b1},
      {{0b1111111},                                      0b1111111},
      {{0b11111111,0b01111111},                   0b11111111111111},
      {{0b10000001,0b01000000},                   0b10000000000001},
      {{0b10000001,0b11000000,0b01100000}, 0b110000010000000000001},
    };
    for (const auto & t : tv){
      ict::buffer::size_type left,right;
      for (const auto & i : t.bytes) left(i);
      right(t.size);
      std::cout<<"left="<<left.getSize()<<" ("<<left.getBytes().size()<<")"<<std::endl;
      std::cout<<"right="<<right.getSize()<<" ("<<right.getBytes().size()<<")"<<std::endl;
      if (left.getSize()!=right.getSize()) throw std::range_error("left.getSize() różni się od right.getSize()!");
      if (left.getBytes().size()!=right.getBytes().size()) throw std::range_error("left.getBytes().size() różni się od right.getBytes().size()!");
      for (std::size_t k=0;k<left.getBytes().size();k++)
        {
          std::cout<<"k="<<k<<std::endl;
          std::cout<<"left="<<(int)left.getBytes().at(k)<<std::endl;
          std::cout<<"right="<<(int)right.getBytes().at(k)<<std::endl;
          if (left.getBytes().at(k)!=right.getBytes().at(k)) throw std::range_error("left.getBytes().at(k) różni się od right.getBytes().at(k)!");
        }
    }
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
template <typename I=int> void test(const I & input,I & output,ict::buffer::basic & buffer){
  buffer<<input;
  buffer>>output;
  std::cout<<"input="<<input<<std::endl;
  std::cout<<"output="<<output<<std::endl;
  if (input!=output) throw std::range_error("input różni się od output!");
}
REGISTER_TEST(buffer,tc2){
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
    std::string in16,out16;
    std::wstring in17,out17;
    std::wstring in18,out18;
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
    in15=ict::random::randomString(10);
    in16=ict::random::randomString(10000);
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
    test(in15,out15,b);
    test(in16,out16,b);
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
#endif
//===========================================
