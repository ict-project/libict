//! @file
//! @brief Object register module - Source file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2017
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2012-2017, ICT-Project Mariusz Ornowski (ict-project.pl)
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
#include "register.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace reg {
//===========================================

//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
class TestClass: public ict::reg::Base {
public:
  TestClass(){
    ict::reg::get<TestClass>().add(this);
  }
  ~TestClass(){
    ict::reg::get<TestClass>().del(this);
  }
  void destroyThis(){}
};
REGISTER_TEST(reg,tc1){
  TestClass * ptr=nullptr;
  std::size_t s=0;
  s=ict::reg::get<TestClass>().size();
  if (s!=0){
    std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
    std::cout<<" s(otrzymany)="<<s<<std::endl;
    std::cout<<" s(oczekiwany)="<<0<<std::endl;
    return(-1);
  }
  if (ict::reg::get<TestClass>().test(ptr)){
    std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
    std::cout<<" Obecny null"<<std::endl;
  }
  {
    TestClass t1,t2,t3;
    ptr=&t1;
    s=ict::reg::get<TestClass>().size();
    if (s!=3){
      std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
      std::cout<<" s(otrzymany)="<<s<<std::endl;
      std::cout<<" s(oczekiwany)="<<3<<std::endl;
      return(-1);
    }
    if (!ict::reg::get<TestClass>().test(&t1)){
      std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
      std::cout<<" Brak t1"<<std::endl;
    }
    if (!ict::reg::get<TestClass>().test(&t2)){
      std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
      std::cout<<" Brak t2"<<std::endl;
    }
    if (!ict::reg::get<TestClass>().test(&t3)){
      std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
      std::cout<<" Brak t3"<<std::endl;
    }
  }
  s=ict::reg::get<TestClass>().size();
  if (s!=0){
    std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
    std::cout<<" s(otrzymany)="<<s<<std::endl;
    std::cout<<" s(oczekiwany)="<<0<<std::endl;
    return(-1);
  }
  if (ict::reg::get<TestClass>().test(ptr)){
    std::cout<<" Błąd("<<__LINE__<<")!!!"<<std::endl;
    std::cout<<" Obecny t1"<<std::endl;
  }
  return(0);
}
#endif
//===========================================
