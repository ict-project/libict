//! @file
//! @brief Options parser module - Source file.
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
#include "options.hpp"
#include "utf8.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace options {
//===========================================
#define _L_(language,string) ((optionLang_##language==lang)?(string):"")
Parser::Parser():lang(optionLang_en){

}
void Parser::setLang(){
  static const std::string locale(setlocale(LC_ALL,NULL));
  static const langMap_string_t langMap={
    {"en",optionLang_en},
    {"pl",optionLang_pl}
  };
  for (langMap_string_t::const_iterator it=langMap.cbegin();it!=langMap.cend();++it){
    if (locale.find(it->first)==0) lang=it->second;
  }
}
void Parser::execConfig(bool config){
  for (configList_t::iterator it=configList.begin();it!=configList.end();++it)
    for (config_t & c : it->second) if (c) c(*this,config);
}
void Parser::internalError(const char * file,int line){
  errors<<"Internal error ("<<file<<":"<<line<<")"<<std::endl;
}
std::wstring Parser::tolower(const std::wstring & input){
  std::wstring output;
  const static std::locale loc;
  for (std::wstring::size_type i=0; i<input.length();++i)
    output+=std::tolower(input.at(i),loc);
  return(output);
}
void Parser::registerOption(
  optionId_t id,
  const shortOptionList_t & shortOpt,
  const longOptionList_t & longOpt
){
  for (const shortOption_t & s : shortOpt){
    if (shortMap.count(s)) throw std::invalid_argument("short option used more then once");
    shortMap[s]=id;
  }
  for (const longOption_t & l : longOpt){
    if (longMap.count(l)) throw std::invalid_argument("long option used more then once");
    longMap[l]=id;
  }
}
std::string Parser::getOptionDesc(optionId_t id){
  std::wstring output;
  bool first=true;
  for (shortMap_t::const_iterator it=shortMap.begin();it!=shortMap.end();++it) if(it->second==id){
    if (first) {
      first=false;
    } else {
      output+=L", ";
    }
    output+=L"-";
    output+=it->first;
  }
  for (longMap_t::const_iterator it=longMap.begin();it!=longMap.end();++it) if(it->second==id){
    if (first) {
      first=false;
    } else {
      output+=L", ";
    }
    output+=L"--";
    output+=it->first;
  }
  return(ict::utf8::get(output));
}
option_v_string_t Parser::parseValue_string(std::wstring & input,const dictionary_string_t * dictionary){
  if (dictionary) if (dictionary->count(input)) return(dictionary->at(input));
  return(ict::utf8::get(input));
}
option_v_wstring_t Parser::parseValue_wstring(std::wstring & input,const dictionary_wstring_t * dictionary){
  if (dictionary) if (dictionary->count(input)) return(dictionary->at(input));
  return(input);
}
option_v_Integer_t Parser::parseValue_Integer(std::wstring & input,const dictionary_Integer_t * dictionary){
  if (dictionary) if (dictionary->count(input)) return(dictionary->at(input));
  {
    option_v_Integer_t output=std::stoi(input);
    return(output);
  }
}
option_v_lInteger_t Parser::parseValue_lInteger(std::wstring & input,const dictionary_lInteger_t * dictionary){
  if (dictionary) if (dictionary->count(input)) return(dictionary->at(input));
  {
    option_v_lInteger_t output=std::stol(input);
    return(output);
  }
}
option_v_llInteger_t Parser::parseValue_llInteger(std::wstring & input,const dictionary_llInteger_t * dictionary){
  if (dictionary) if (dictionary->count(input)) return(dictionary->at(input));
  {
    option_v_llInteger_t output=std::stoll(input);
    return(output);
  }
}
option_v_Float_t Parser::parseValue_Float(std::wstring & input,const dictionary_Float_t * dictionary){
  if (dictionary)  if (dictionary->count(input)) return(dictionary->at(input));
  {
    option_v_Float_t output=std::stod(input);
    return(output);
  }
}
option_v_lFloat_t Parser::parseValue_lFloat(std::wstring & input,const dictionary_lFloat_t * dictionary){
  if (dictionary)  if (dictionary->count(input)) return(dictionary->at(input));
  {
    option_v_lFloat_t output=std::stold(input);
    return(output);
  }
}
option_v_bool_t Parser::parseValue_bool(std::wstring & input,const dictionary_bool_t * dictionary){
  if (dictionary)  if (dictionary->count(input)) return(dictionary->at(input));
  {
    option_v_bool_t output;
    std::wstring in(tolower(input));
    switch (lang){
      case optionLang_pl:{
        static const std::map<std::wstring,bool> map({
          {L"1",true},
          {L"0",false},
          {L"tak",true},
          {L"nie",false},
          {L"prawda",true},
          {L"fałsz",false},
          {L"falsz",false},
          {L"true",true},
          {L"false",false}
        });
        if (map.count(in)) {
          output=map.at(in);
        } else {
          throw std::invalid_argument("unable to parse bool value");
        }
      } break;
      default:{
        static const std::map<std::wstring,bool> map({
          {L"1",true},
          {L"0",false},
          {L"yes",true},
          {L"no",false},
          {L"true",true},
          {L"false",false}
        });
        if (map.count(in)) {
          output=map.at(in);
        } else {
          throw std::invalid_argument("unable to parse bool value");
        }
      } break;
    }
    return(output);
  }
}
int Parser::parseValue(std::wstring & input){
  optionId_t id=currentId;
  currentId=-1;
  if (!optionConfig.count(id)) {
    internalError(__FILE__,__LINE__);
    return(-3);
  }
  try {
    switch(optionConfig.at(id).type){
      case optionType_v_none:{
        option_v_none_t * t=optionConfig.at(id).target.v_none;
        if (t) (*t)++;
      } break;
      #define Options_parseValue(prefix,name) 
      #define _OPTIONS_(type,name) \
        case optionType_v_##name:{ \
          option_v_##name##_t * t=optionConfig.at(id).target.v_##name; \
          if (t) (*t)=parseValue_##name(input,optionConfig.at(id).dictionary.d_##name); \
        } break;
      #include "options.h.in"
      #undef _OPTIONS_
      #define _OPTIONS_(type,name) \
        case optionType_l_##name:{ \
          option_l_##name##_t * t=optionConfig.at(id).target.l_##name; \
          if (t) t->push_back(parseValue_##name(input,optionConfig.at(id).dictionary.d_##name)); \
        } break;
      #include "options.h.in"
      #undef _OPTIONS_
      default:
        internalError(__FILE__,__LINE__);
        return(-4);
    }
  } catch (...){
    errors<<
      _L_(en,"Unable to parse option value: ")<<
      _L_(pl,"Wystąpił problem przy parsowaniu wartości opcji: ")<<
      getOptionDesc(id)<<
    std::endl;
    return(1);
  }
  return(0);
}
int Parser::parseShort(shortOption_t input){
  if(shortMap.count(input)){
    optionId_t id(shortMap.at(input));
    if (optionConfig.count(id)){
      if (currentId==-1){
        currentId=id;
      } else {
        errors<<
          _L_(en,"Option value is missing: ")<<
          _L_(pl,"Brak wartości opcji: ")<<
          getOptionDesc(currentId)<<
        std::endl;
        return(4);
      }
      if (!optionConfig.at(id).type){
        std::wstring in;
        parseValue(in);
      }
    } else {
      internalError(__FILE__,__LINE__);
      return(-5);
    }
  } else {
    std::wstring in;
    in+=input;
    errors<<
      _L_(en,"Unknown option: -")<<
      _L_(pl,"Nieznana opcja: -")<<
      ict::utf8::get(in)<<
    std::endl;
    return(2);
  }
  return(0);
}
int Parser::parseShort(std::wstring & input){
  for (const shortOption_t & s : input){
    int out=parseShort(s);
    if (out) return(out);
  }
  return(0);
}
int Parser::parseLong(std::wstring & input){
  std::size_t v=input.find(L'=');
  std::wstring value;
  if (v!=std::string::npos){
    value=input.substr(v+1);
    input=input.substr(0,v);
  }
  if(longMap.count(input)){
    optionId_t id(longMap.at(input));
    if (optionConfig.count(id)){
      if (currentId==-1){
        currentId=id;
        if (v!=std::string::npos) return(parseValue(value));
      } else {
        internalError(__FILE__,__LINE__);
        return(-6);
      }
      if (!optionConfig.at(id).type){
        std::wstring in;
        parseValue(in);
      }
    } else {
      internalError(__FILE__,__LINE__);
      return(-7);
    }
  } else {
    errors<<
      _L_(en,"Unknown option: --")<<
      _L_(pl,"Nieznana opcja: --")<<
      ict::utf8::get(input)<<
    std::endl;
    return(3);
  }
  return(0);
}
int Parser::parseOther(std::wstring & input){
  if (otherId==-1) return(0);
  currentId=otherId;
  return(parseValue(input));
}
std::string Parser::getOptionDesc(const shortOption_t & shortOpt){
  if (shortMap.count(shortOpt)) return(getOptionDesc(shortMap.at(shortOpt)));
  return("");
}
std::string Parser::getOptionDesc(const longOption_t & longOpt){
  if (longMap.count(longOpt)) return(getOptionDesc(longMap.at(longOpt)));
  return("");
}
int Parser::parse(std::wstring & input){
  if (currentId!=-1) return(parseValue(input));
  if (input.size()){
    if (input.front()==L'-'){
      input.erase(0,1);
      if (input.size()){
        if (input.front()==L'-'){
          input.erase(0,1);
          if (input.size()){
            return(parseLong(input));
          } else {
            input=L"--";
            return(parseOther(input));
          }
        }
        return(parseShort(input));
      } else {
        input=L"-";
        return(parseOther(input));
      }
    }
    return(parseOther(input));
  }
  return(0);
}
int Parser::parse(int argc_in,const char **argv_in) noexcept {
  int out=0;
  try {
    setLang();
    execConfig(true);
    for (int k=1;k<argc_in;k++){
      if (argv_in[k]!=NULL) {
        std::wstring arg(ict::utf8::get(argv_in[k]));
        out=parse(arg);
        if (out) {
          execConfig(false);
          return(out);
        }
      } else {
        internalError(__FILE__,__LINE__);
        return(-2);
      }
    }
    if (currentId!=-1){
      errors<<
        _L_(en,"Option value is missing: ")<<
        _L_(pl,"Brak wartości opcji: ")<<
        getOptionDesc(currentId)<<
      std::endl;
      execConfig(false);
      return(5);
    }
  } catch (std::exception & e){
    internalError(__FILE__,__LINE__);
    errors<<e.what()<<std::endl;
    return(-1);
  }
  return(0);
}
void Parser::registerConfig(int priority,config_t config){
  if (config) configList[priority].push_back(config);
}
void Parser::help() noexcept{
  try {
    setLang();
    execConfig(false);
  } catch (std::exception & e){
    internalError(__FILE__,__LINE__);
    errors<<e.what()<<std::endl;
  }
}
#undef _L_
Parser & Config::getParser(){
  static Parser parser;
  return(parser);
}
Config::Config(int priority,config_t config){
  getParser().registerConfig(priority,config);
}
int Config::parse(int argc_in,const char **argv_in,std::ostream & output) noexcept {
  getParser().errors.str("");
  {
    int out=getParser().parse(argc_in,argv_in);
    output<<getParser().errors.str();
    return(out);
  }
}
void Config::help(std::ostream & output) noexcept{
  getParser().errors.str("");
  getParser().help();
  output<<getParser().errors.str();
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(options,tc1){
  int out=0;
  const char * input[]={"nic"};
  ict::options::Parser parser;
  std::cout<<" Test funkcji ict::options::Parser::parse()"<<std::endl;
  if ((out=parser.parse(sizeof(input)/sizeof(*input),input))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" out="<<out<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(options,tc2){
  int out=0;
  const char * input[]={
    "nic",
    "nic1",
    "nic2"
  };
  std::vector<std::string> output;
  ict::options::Parser parser;
  std::cout<<" Test funkcji ict::options::Parser::parse() i ict::options::Parser::registerOther(std::string)"<<std::endl;
  parser.registerOther(output);
  if ((out=parser.parse(sizeof(input)/sizeof(*input),input))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" out="<<out<<std::endl;
    return(-1);
  }
  if ((sizeof(input)/sizeof(*input)!=(output.size()+1))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" output.size()="<<output.size()<<std::endl;
    return(-1);
  }
  for (int k=1;k<(sizeof(input)/sizeof(*input));k++){
    if (output[k-1]!=input[k]){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input[k]="<<input[k]<<std::endl;
      std::cout<<" output[k-1]="<<output[k-1]<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(options,tc3){
  int out=0;
  const char * input[]={
    "nic",
    "nic1",
    "nic2"
  };
  std::vector<std::wstring> output;
  ict::options::Parser parser;
  std::cout<<" Test funkcji ict::options::Parser::parse() i ict::options::Parser::registerOther(std::wstring)"<<std::endl;
  parser.registerOther(output);
  if ((out=parser.parse(sizeof(input)/sizeof(*input),input))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" out="<<out<<std::endl;
    return(-1);
  }
  if ((sizeof(input)/sizeof(*input)!=(output.size()+1))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" output.size()="<<output.size()<<std::endl;
    return(-1);
  }
  for (int k=1;k<(sizeof(input)/sizeof(*input));k++){
    if (ict::utf8::get(output[k-1])!=input[k]){
      std::cout<<" Błąd!!!"<<std::endl;
      std::cout<<" input[k]="<<input[k]<<std::endl;
      std::wcout<<L" output[k-1]="<<output[k-1]<<std::endl;
      return(-1);
    }
  }
  return(0);
}
REGISTER_TEST(options,tc4){
  int out=0;
  const char * input[]={
    "nic",
    "-nic",
    "--nic"
  };
  ict::options::option_v_none_t n=0,i=0,c=0;
  ict::options::Parser parser;
  std::cout<<" Test funkcji ict::options::Parser::parse() i ict::options::Parser::registerOptNoValue()"<<std::endl;
  parser.registerOptNoValue(L'n',ict::options::longOptionList_t({L"nic"}),n);
  parser.registerOptNoValue(ict::options::shortOptionList_t({L'i'}),L"ic",i);
  parser.registerOptNoValue(L'c',L"c",c);
  if ((out=parser.parse(sizeof(input)/sizeof(*input),input))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" out="<<out<<std::endl;
    return(-1);
  }
  if (n!=2){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" n="<<n<<std::endl;
    return(-1);
  }
  if (i!=1){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" i="<<i<<std::endl;
    return(-1);
  }
  if (c!=1){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" c="<<c<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(options,tc5){
  int out=0;
  const char * input[]={
    "nic",
    "-n",
    "20",
    "--nic=60"
  };
  ict::options::option_v_Integer_t n=0,nic=0;
  ict::options::Parser parser;
  std::cout<<" Test funkcji ict::options::Parser::parse() i ict::options::Parser::registerOptNoValue()"<<std::endl;
  parser.registerOpt(L'n',ict::options::longOptionList_t({L"ic"}),n);
  parser.registerOpt(ict::options::shortOptionList_t({L'i'}),L"nic",nic);
  if ((out=parser.parse(sizeof(input)/sizeof(*input),input))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" out="<<out<<std::endl;
    return(-1);
  }
  if (n!=20){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" n="<<n<<std::endl;
    return(-1);
  }
  if (nic!=60){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" nic="<<nic<<std::endl;
    return(-1);
  }
  return(0);
}
REGISTER_TEST(options,tc6){
  int out=0;
  const char * input[]={
    "nic",
    "-n",
    "20",
    "--integer",
    "101",
    "niiiic0",
    "--nic=60",
    "--float=50.989",
    "niiiic1",
    "--integer=99",
    "--float=5.88"
  };
  ict::options::option_v_Integer_t n=0,nic=0;
  std::vector<std::string> output;
  std::vector<int> integer_opt;
  long double float_opt;
  
  ict::options::Parser parser;
  std::cout<<" Test funkcji ict::options::Parser::parse(), ict::options::Parser::registerOpt(), ict::options::Parser::registerLongOpt(), ict::options::Parser::registerOther()"<<std::endl;
  parser.registerOpt(L'n',ict::options::longOptionList_t({L"ic"}),n);
  parser.registerOpt(ict::options::shortOptionList_t({L'i'}),L"nic",nic);
  parser.registerLongOpt(L"integer",integer_opt);
  parser.registerLongOpt(L"float",float_opt);
  parser.registerOther(output);
  if ((out=parser.parse(sizeof(input)/sizeof(*input),input))){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" out="<<out<<std::endl;
    return(-1);
  }
  if (n!=20){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" n="<<n<<std::endl;
    return(-1);
  }
  if (nic!=60){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" nic="<<nic<<std::endl;
    return(-1);
  }
  if (output.size()!=2){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" output.size()="<<output.size()<<std::endl;
    return(-1);
  }
  if (output.at(0)!="niiiic0"){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" output.at(0)="<<output.at(0)<<std::endl;
    return(-1);
  }
  if (output.at(1)!="niiiic1"){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" output.at(1)="<<output.at(1)<<std::endl;
    return(-1);
  }
  if (integer_opt.size()!=2){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" integer_opt.size()="<<integer_opt.size()<<std::endl;
    return(-1);
  }
  if (integer_opt.at(0)!=101){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" integer_opt.at(0)="<<integer_opt.at(0)<<std::endl;
    return(-1);
  }
  if (integer_opt.at(1)!=99){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" integer_opt.at(1)="<<integer_opt.at(1)<<std::endl;
    return(-1);
  }
  if (float_opt*1000!=5880){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" float_opt="<<float_opt<<std::endl;
    return(-1);
  }
  return(0);
}
#endif
//===========================================
