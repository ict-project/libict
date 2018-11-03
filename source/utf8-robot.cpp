//! @file
//! @brief UTF8 robot module - Source file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2018
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
static const uint8_t bit_map8[][2]={
    {0b10000000,0b11000000},// 0->
    {0b11000000,0b11100000},// 1-> 2
    {0b11100000,0b11110000},// 2-> 3
    {0b11110000,0b11111000},// 3-> 4
    {0b11111000,0b11111100},// 4-> 5
    {0b11111100,0b11111110},// 5-> 6
};
static const uint32_t bit_map32[]={
    0b00000000000000000000000001111111,// 0-> 7
    0b00000000000000000000011111111111,// 1-> 11
    0b00000000000000001111111111111111,// 2-> 16
    0b00000000000111111111111111111111,// 3-> 21
    0b00000011111111111111111111111111,// 4-> 26
    0b01111111111111111111111111111111,// 5-> 31
};
//===========================================
template <typename T>
class Tester{
private:
    T mask;
public:
    Tester():mask(0x0){
        for (uint_fast8_t k=0;k<sizeof(T);k++){
            mask<<=8;
            mask|=0x80;
        }
    }
    bool operator ()(const T & input) const {
        return(mask&input);
    }
};
//===========================================
const wchar_t Robot::empty_char(-1);
const std::string Robot::empty_string;
Robot::status_t Robot::getSize8(const uint8_t & input){
    for (uint_fast8_t k=1;k<6;k++)
        if (bit_map8[k][0]==(input&(bit_map8[k][1]))) 
            return((status_t)(status_empty+k));
    return(status_empty);    
}
Robot::status_t Robot::getSize32(const uint32_t & input){
    for (uint_fast8_t k=0;k<6;k++)
        if (!(input&(~(bit_map32[k]))))
            return((status_t)(status_empty+k));
    return(status_empty);
}
Robot & Robot::operator ()(const char & input){
    const static Tester<char> tester;
    if (!status) {//Jeśli status jest ustawiony na status_ready
        s.clear();//Wyczyść
        status=status_empty;//Ustaw status.
    }
    s+=input;//Dodaj bajt
    {
        const char & front(s.front());
        if (s.size()==1){//To jest dopiero pierwszy bajt
            if (tester(front)){//To nie jest ASCII
                status=getSize8(front);//Ustal status
            } else {//To jest ASCII
                c=front;//Ustaw znak
                status=status_ready;//Ustaw status
            }
        } else if (status<=s.size()) {//Jest wystarczająca liczba bajtów
            c=front&(0b01111111>>status);//Przepisz zerowy znak
            for (uint_fast8_t k=1;k<status;k++){//Przepisz pozostałe znaki
                c<<=6;//Przesuń o 6 bitów.
                c|=s[k]&0b00111111;//Wstaw 6 bitów
            }
            status=status_ready;//Ustaw status
        }
    }
    return(*this);
}
Robot & Robot::operator ()(const wchar_t & input){
    s.clear();//Wyczyść
    c=input&(bit_map32[5]);//Ustaw znak
    status=getSize32(input);//Ustal status (tymczasowo)
    if (status_empty<status){//Jeśli to znak wielobajtowy
        uint32_t tmp=input;
        s.resize(status);//Ustaw rozmiar
        for (uint_fast8_t k=status-1;0<k;k--){
            uint8_t b=bit_map8[0][0];//Ustaw najwyższe bity
            uint8_t t=tmp;
            t&=(~bit_map8[0][1]);
            b|=t;//Ustaw niższe bity
            tmp>>=6;
            s[k]=b;
        }
        {
            uint_fast8_t k=status-1;
            uint8_t b=bit_map8[k][0];//Ustaw nawjyższe bity
            uint8_t t=tmp;
            t&=(~bit_map8[k][1]);
            b|=t;//Ustaw niższe bity
            s[0]=b;
        }
    } else {//Jeśli to znak jedno bajtowy
        s.resize(1);//Ustaw rozmiar
        s.front()=input;//Wstaw znak
    }
    status=status_ready;//Ustaw status.    
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
  std::cout<<" Test ict::utf8::Robot  (from std::string - robot.getString())"<<std::endl;
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
REGISTER_TEST(utf8_robot,tc2){
  std::size_t k=0;
  std::cout<<" Test ict::utf8::Robot (from std::string - robot.getChar())"<<std::endl;
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
REGISTER_TEST(utf8_robot,tc3){
  std::size_t k=0;
  std::cout<<" Test ict::utf8::Robot (from std::wstring - robot.getString())"<<std::endl;
  for (const auto & s : ict::test::test_wstring){
    std::wstring input(s);
    std::string output(ict::test::test_string[k++]);
    std::string out;
    ict::utf8::Robot robot;
    for (const auto & c : input){
        if (robot(c).isReady()){
            out+=robot.getString();
        }
    }
    if (output!=out){
      std::cout<<" Błąd!!!"<<std::endl;
      std::wcout<<" input="<<input<<std::endl;
      std::cout<<L" output(oczekiwany)="<<output<<std::endl;
      std::cout<<L" output(otrzymany)="<<out<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(utf8_robot,tc4){
  std::size_t k=0;
  std::cout<<" Test ict::utf8::Robot (from std::wstring - robot.getChar())"<<std::endl;
  for (const auto & s : ict::test::test_wstring){
    std::wstring input(s);
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
      std::wcout<<L" input="<<input<<std::endl;
      std::wcout<<L" output(oczekiwany)="<<output<<std::endl;
      std::wcout<<L" output(otrzymany)="<<out<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(utf8_robot,tc5){
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