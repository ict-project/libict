//! @file
//! @brief UTF8 module - Source file.
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
#include "utf8.hpp"
#include "utf8/utf8.h"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace utf8 {
//===========================================
void transfer(std::string & input,std::wstring & output){
  if (input.size()){
    std::string::iterator buffer_begin=input.begin();
    std::string::iterator buffer_end=input.end();
    for (;buffer_begin!=buffer_end;){
      std::string::iterator buffer_invalid=::utf8::find_invalid(buffer_begin,buffer_end);
      switch (sizeof(wchar_t)){
        case 2:
          ::utf8::utf8to16(buffer_begin,buffer_invalid,std::back_inserter(output));
          break;
        case 4:
          ::utf8::utf8to32(buffer_begin,buffer_invalid,std::back_inserter(output));
          break;
        default:break;
      }
      if (buffer_invalid==buffer_end) {//Przetworzono całość.
        input.clear();
        return;//Koniec.
      } else {//Przetworzono część.
        if (::utf8::distance(buffer_invalid,buffer_end)>4){ // Jeśli zostało dużo, to przesuń się na koniec plus jeden.
          buffer_begin=buffer_invalid;
          ++buffer_begin;
        } else {// Jeśli zostało mało, to zostaw na później.
          input.erase(buffer_begin,buffer_invalid);
          return;//Koniec.
        }
      }
    }
  }
}
std::wstring get(const std::string & input_in){
  std::string input(input_in);
  std::wstring output;
  transfer(input,output);
  return(output);
}
std::wstring get(const char * input_in){
  std::string input(input_in);
  std::wstring output;
  transfer(input,output);
  return(output);
}
void transfer(std::wstring & input,std::string & output){
  if (input.size()){
    switch (sizeof(wchar_t)){
      case 2:
        ::utf8::utf16to8(input.begin(),input.end(),std::back_inserter(output));
        break;
      case 4:
        ::utf8::utf32to8(input.begin(),input.end(),std::back_inserter(output));
        break;
      default:break;
    }
    input.clear();
  }
}
std::string get(const std::wstring & input_in){
  std::wstring input(input_in);
  std::string output;
  transfer(input,output);
  return(output);
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(utf8,tc1){
  std::size_t k=0;
  std::cout<<" Test funcji ict::utf8::get(string)"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::wstring output(ict::test::test_wstring[k++]);
    std::wstring out=ict::utf8::get(input);
    if (output!=out){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::wcout<<L" output(oczekiwany)="<<output<<std::endl;
      std::wcout<<L" output(otrzymany)="<<out<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(utf8,tc2){
  std::size_t k=0;
  std::cout<<" Test funcji ict::utf8::get(wstring)"<<std::endl;
  for (const auto & s : ict::test::test_wstring){
    std::wstring input(s);
    std::string output(ict::test::test_string[k++]);
    std::string out=ict::utf8::get(input);
    if (output!=out){
      std::cout<<" Błąd!!!"<<std::endl;
      std::wcout<<L" input="<<input<<std::endl;
      std::cout<<" output(oczekiwany)="<<output<<std::endl;
      std::cout<<" output(otrzymany)="<<out<<std::endl;
      return(-1);
    }
  }
  return(0);
}
#endif
//===========================================