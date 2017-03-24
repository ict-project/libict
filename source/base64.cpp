//! @file
//! @brief Base64 module - Source file.
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
#include "base64.hpp"
#include <map>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace base64 {
static char bit2char(uint8_t input,bool url=false){
  const static std::map<uint8_t,char> map1({
    {0,'A'},{22,'W'},{44,'s'},
    {1,'B'},{23,'X'},{45,'t'},
    {2,'C'},{24,'Y'},{46,'u'},
    {3,'D'},{25,'Z'},{47,'v'},
    {4,'E'},{26,'a'},{48,'w'},
    {5,'F'},{27,'b'},{49,'x'},
    {6,'G'},{28,'c'},{50,'y'},
    {7,'H'},{29,'d'},{51,'z'},
    {8,'I'},{30,'e'},{52,'0'},
    {9,'J'},{31,'f'},{53,'1'},
    {10,'K'},{32,'g'},{54,'2'},
    {11,'L'},{33,'h'},{55,'3'},
    {12,'M'},{34,'i'},{56,'4'},
    {13,'N'},{35,'j'},{57,'5'},
    {14,'O'},{36,'k'},{58,'6'},
    {15,'P'},{37,'l'},{59,'7'},
    {16,'Q'},{38,'m'},{60,'8'},
    {17,'R'},{39,'n'},{61,'9'},
    {18,'S'},{40,'o'},{62,'+'},
    {19,'T'},{41,'p'},{63,'/'},
    {20,'U'},{42,'q'},   
    {21,'V'},{43,'r'}
  });
  const static std::map<uint8_t,char> map2({
    {0,'A'},{22,'W'},{44,'s'},
    {1,'B'},{23,'X'},{45,'t'},
    {2,'C'},{24,'Y'},{46,'u'},
    {3,'D'},{25,'Z'},{47,'v'},
    {4,'E'},{26,'a'},{48,'w'},
    {5,'F'},{27,'b'},{49,'x'},
    {6,'G'},{28,'c'},{50,'y'},
    {7,'H'},{29,'d'},{51,'z'},
    {8,'I'},{30,'e'},{52,'0'},
    {9,'J'},{31,'f'},{53,'1'},
    {10,'K'},{32,'g'},{54,'2'},
    {11,'L'},{33,'h'},{55,'3'},
    {12,'M'},{34,'i'},{56,'4'},
    {13,'N'},{35,'j'},{57,'5'},
    {14,'O'},{36,'k'},{58,'6'},
    {15,'P'},{37,'l'},{59,'7'},
    {16,'Q'},{38,'m'},{60,'8'},
    {17,'R'},{39,'n'},{61,'9'},
    {18,'S'},{40,'o'},{62,'-'},
    {19,'T'},{41,'p'},{63,'_'},
    {20,'U'},{42,'q'},   
    {21,'V'},{43,'r'}
  });
  input&=0x3F;
  return(url?map2.at(input):map1.at(input));
}
static uint8_t char2bit(char input){
  const static std::map<char,uint8_t> map({
    {'A',0},{'W',22},{'s',44},
    {'B',1},{'X',23},{'t',45},
    {'C',2},{'Y',24},{'u',46},
    {'D',3},{'Z',25},{'v',47},
    {'E',4},{'a',26},{'w',48},
    {'F',5},{'b',27},{'x',49},
    {'G',6},{'c',28},{'y',50},
    {'H',7},{'d',29},{'z',51},
    {'I',8},{'e',30},{'0',52},
    {'J',9},{'f',31},{'1',53},
    {'K',10},{'g',32},{'2',54},
    {'L',11},{'h',33},{'3',55},
    {'M',12},{'i',34},{'4',56},
    {'N',13},{'j',35,},{'5',57},
    {'O',14},{'k',36},{'6',58},
    {'P',15},{'l',37},{'7',59},
    {'Q',16},{'m',38},{'8',60},
    {'R',17},{'n',39},{'9',61},
    {'S',18},{'o',40},{'+',62},
    {'T',19},{'p',41},{'/',63},
    {'U',20},{'q',42},{'-',62},   
    {'V',21},{'r',43},{'_',63},
    {' ',64},{'\n',64},{'\r',64},{'\t',64},{'\v',64}//Znaki ignorowane
  });
  if (map.count(input)) return(map.at(input));
  return(0xFF);
}
void encode(const std::string & input,std::string & output,bool url,bool padding){
  uint8_t phase=0;
  uint8_t three[3];
  uint8_t c_out;
  for (const auto & c_in: input){
    three[phase]=c_in;
    if((++phase)>=3) {
      c_out=three[0]>>2;
      output+=bit2char(c_out,url);
      c_out=three[0]<<4;
      c_out|=three[1]>>4;
      output+=bit2char(c_out,url);
      c_out=three[1]<<2;
      c_out|=three[2]>>6;
      output+=bit2char(c_out,url);
      c_out=three[2];
      output+=bit2char(c_out,url);
      phase=0;
    }
  }
  switch (phase){
    case 1:
      c_out=three[0]>>2;
      output+=bit2char(c_out,url);
      c_out=three[0]<<4;
      output+=bit2char(c_out,url); 
      if (padding) output.append(2,'=');
      break;
    case 2:
      c_out=three[0]>>2;
      output+=bit2char(c_out,url);
      c_out=three[0]<<4;
      c_out|=three[1]>>4;
      output+=bit2char(c_out,url);
      c_out=three[1]<<2;
      output+=bit2char(c_out,url);
      if (padding) output.append(1,'=');
      break;
    default:break;
  }
}
std::string encode(const std::string & input,bool url,bool padding){
  std::string output;
  encode(input,output,url,padding);
  return(output);
}
void decode(const std::string & input,std::string & output){
  uint8_t phase=0;
  uint8_t four[4];
  uint8_t c_out;
  for (const auto & c_in: input){
    if ((four[phase]=char2bit(c_in))<64){
      if((++phase)>=4) {
        c_out=four[0]<<2;
        c_out|=four[1]>>4;
        output+=c_out;
        c_out=four[1]<<4;
        c_out|=four[2]>>2;
        output+=c_out;
        c_out=four[2]<<6;
        c_out|=four[3];
        output+=c_out;
        phase=0;
      }
    } else if (four[phase]>64){
      break;
    }
  }
  switch (phase){
    case 1:
      c_out=four[0]<<2;
      if (c_out) output+=c_out;
      break;
    case 2:
      c_out=four[0]<<2;
      c_out|=four[1]>>4;
      output+=c_out;
      c_out=four[1]<<4;
      if (c_out) output+=c_out;
      break;
    case 3:
      c_out=four[0]<<2;
      c_out|=four[1]>>4;
      output+=c_out;
      c_out=four[1]<<4;
      c_out|=four[2]>>2;
      output+=c_out;
      c_out=four[2]<<6;
      if (c_out) output+=c_out;
      break;
    default:break;
  }
}
std::string decode(const std::string & input){
  std::string output;
  decode(input,output);
  return(output);
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(base64,tc1){
  std::string input(ict::test::test_string[3]);
  std::string output("RG_Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg==");
  std::string out;
  std::cout<<" Test funcji ict::base64::encode(input,url=true,padding=true)"<<std::endl;
  out=ict::base64::encode(input,true,true);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc2){
  std::string input(ict::test::test_string[3]);
  std::string output("RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg==");
  std::string out;
  std::cout<<" Test funcji ict::base64::encode(input,url=false,padding=true)"<<std::endl;
  out=ict::base64::encode(input,false,true);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc3){
  std::string input(ict::test::test_string[3]);
  std::string output("RG_Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg");
  std::string out;
  std::cout<<" Test funcji ict::base64::encode(input,url=true,padding=false)"<<std::endl;
  out=ict::base64::encode(input,true,false);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc4){
  std::string input(ict::test::test_string[3]);
  std::string output("RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg");
  std::string out;
  std::cout<<" Test funcji ict::base64::encode(input,url=false,padding=false)"<<std::endl;
  out=ict::base64::encode(input,false,false);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc5){
  std::string input("RG_Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg==");
  std::string output(ict::test::test_string[3]);
  std::string out;
  std::cout<<" Test funcji ict::base64::decode()"<<std::endl;
  out=ict::base64::decode(input);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc6){
  std::string input("RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg==");
  std::string output(ict::test::test_string[3]);
  std::string out;
  std::cout<<" Test funcji ict::base64::decode()"<<std::endl;
  out=ict::base64::decode(input);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc7){
  std::string input("RG_Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg");
  std::string output(ict::test::test_string[3]);
  std::string out;
  std::cout<<" Test funcji ict::base64::decode()"<<std::endl;
  out=ict::base64::decode(input);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc8){
  std::string input("RG/Fm8SHIGLFgmF6ZcWEc3R3LCDFvHLEhSBtw7NqIHDEmWsgbHXFum55Y2ggZmlnLg");
  std::string output(ict::test::test_string[3]);
  std::string out;
  std::cout<<" Test funcji ict::base64::decode()"<<std::endl;
  out=ict::base64::decode(input);
  if (out!=output){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" input="<<input<<std::endl;
    std::cout<<" output(oczekiwany)="<<output<<std::endl;
    std::cout<<" output(otrzymany)="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(base64,tc9){
  std::cout<<" Test funcji ict::base64::encode(input,url=true,padding=true) oraz ict::base64::decode()"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::string enc(ict::base64::encode(input,true,true));
    std::string dec(ict::base64::decode(enc));
    if (input!=dec){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" encode()="<<enc<<std::endl;
      std::cout<<" decode()="<<dec<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(base64,tc10){
  std::cout<<" Test funcji ict::base64::encode(input,url=false,padding=true) oraz ict::base64::decode()"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::string enc(ict::base64::encode(input,false,true));
    std::string dec(ict::base64::decode(enc));
    if (input!=dec){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" encode()="<<enc<<std::endl;
      std::cout<<" decode()="<<dec<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(base64,tc11){
  std::cout<<" Test funcji ict::base64::encode(input,url=true,padding=false) oraz ict::base64::decode()"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::string enc(ict::base64::encode(input,true,false));
    std::string dec(ict::base64::decode(enc));
    if (input!=dec){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" encode()="<<enc<<std::endl;
      std::cout<<" decode()="<<dec<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(base64,tc12){
  std::cout<<" Test funcji ict::base64::encode(input,url=false,padding=false) oraz ict::base64::decode()"<<std::endl;
  for (const auto & s : ict::test::test_string){
    std::string input(s);
    std::string enc(ict::base64::encode(input,false,false));
    std::string dec(ict::base64::decode(enc));
    if (input!=dec){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input="<<input<<std::endl;
      std::cout<<" encode()="<<enc<<std::endl;
      std::cout<<" decode()="<<dec<<std::endl;
      return(-1);
    }
  }
  return(0);
}
#endif
//===========================================