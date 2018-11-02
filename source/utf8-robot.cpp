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
#include "utf8-robot.hpp"
#include <cstdint>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace utf8 {
//===========================================
template <typename T>
class Tester{
private:
    T mask;
public:
    Tester():mask(0x0){
        for (int k=0;k<sizeof(T);k++){
            mask<<=8;
            mask|=0x80;
        }
    }
    bool operator ()(const T & input) const {
        return(mask&input);
    }
};
//===========================================
const wchar_t Robot::empty(-1);
Robot & Robot::operator ()(const char & input){
    const static Tester<char> tester;
    if (ready) {
        s.clear();
        ready=false;
    }
    s+=input;
    {
        char mask(0xFE);
        char value(0xFC);
        const char & front(s.front());
        if (tester(front)){//To nie jest ASCII
            for (int k=6;1<k;k--){//Ilość bajtów
                if ((front&mask)==value){//Znaleziona liczba bajtów
                    if (k<=s.size()){// Liczba znaków jest wystarczająca.
                        c=front&(~mask);//Przepisz zerowy znak
                        for (int l=1;l<k;l++){//Przepisz pozostałe znaki
                            c<<=6;//Przesuń o 6 bitów.
                            c|=s[l]&0x3F;//Wstaw 6 bitów
                        }
                        ready=true;
                    }
                    return(*this);
                }
                mask<<=1;
                value<<=1;
            }
        } else {//To jest ASCII
            c=front;
            ready=true;
        }
    }
    return(*this);
}
template <typename T>
bool hasUtf8_template(const std::string & input,std::size_t & last){
    const static Tester<T> tester;
    std::size_t max=input.size();
    max-=(max%sizeof(T));
    for (;last<max;last+=sizeof(T)) {
        const T * w=(T *)(input.c_str()+last);
        if (tester(*w)) return(true);
    }
    return(false);
}
bool Robot::hasUtf8(const std::string & input){
    std::size_t l=0;
    if (hasUtf8_template<std::int64_t>(input,l)) return(true);
    if (hasUtf8_template<std::int8_t>(input,l)) return(true);
    return(false);
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(utf8_robot,tc1){
  std::size_t k=0;
  std::cout<<" Test ict::utf8::Robot"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::wstring output(ict::test::test_wstring[k++]);
    std::wstring out;
    ict::utf8::Robot robot;
    for (const auto & c : input){
        if (robot(c).isReady()){
            out+=robot.getChar();
        }
    }
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
REGISTER_TEST(utf8_robot,tc2){
  std::size_t k=0;
  std::cout<<" Test ict::utf8::Robot"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::string out;
    ict::utf8::Robot robot;
    for (const auto & c : input){
        if (robot(c).isReady()){
            out+=robot.getString();
        }
    }
    if (input!=out){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" output(otrzymany)="<<out<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(utf8_robot,tc3){
  std::size_t k=0;
  std::cout<<" Test ict::utf8::Robot::hasUtf8"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    if (!ict::utf8::Robot::hasUtf8(input)){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" output(otrzymany)="<<0<<std::endl;
      return(-1);
    }
  }
  {
    std::string input("Test ASCII - 0123456789");
    if (ict::utf8::Robot::hasUtf8(input)){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" output(otrzymany)="<<1<<std::endl;
      return(-1);
    }
  }
  return(0);
}
#endif
//===========================================