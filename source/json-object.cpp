//! @file
//! @brief JSON object module - Source file.
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
#include "json-object.hpp"
#include <regex>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
#define DO_JOB(operation)\
  switch(operation){\
    case 0:break;\
    case 1:return(1);\
    default:return(-1);\
  }
#define DO_INFO(base,name) \
  if (base::jsonParams_##name) {\
    info_ptr->name()=(*base::jsonParams_##name);\
    info_ptr->JOBJECT_SHOW(info_ptr->name);\
  }
//============================================
namespace ict { namespace jobject {
//============================================
void remove_ws(std::wstring & input){
  while (std::iswspace(input.front())) input.erase(0,1);
}
bool get_json_element(std::wstring & input,std::wstring & output){
  bool qutation=false;//Czy otwarty był cudzysłów.
  wchar_t last(L'\0');//Ostatni znak.
  std::size_t idx=0;//Indeks ostatniego znaku.
  std::size_t nws=0;//Indeks ostatniego znaku, który nie był białym znakiem.
  remove_ws(input);
  for (const wchar_t & c : input){
    switch(c){
      case L':': case L',': case L'{': case L'}': case L'[': case L']':case L'\0':
        if ((!qutation)||(c==L'\0')) {
          output.assign(input,0,nws);
          input.erase(0,idx);
          return(true);
        }
        break;
      case L'"':
        if (qutation){
          if (last!=L'\\') qutation=false;
        } else {
          qutation=true;
        }
        break;
      default:
        break;
    }
    last=c;
    idx++;
    if (!std::iswspace(c)) nws=idx;
  }
  return(false);
}
std::string get_type_name(json_type_t type){
  switch(type){
    case json_type_null:return("null");
    case json_type_bool:return("bool");
    case json_type_number:return("number");
    case json_type_string:return("string");
    case json_type_array:return("array");
    case json_type_object:return("object");
    case json_type_mutable:return("mutable");
    default: break;
  }
  return("unknown");
}
//============================================
namespace string {
//============================================
std::wstring escapeString(const std::wstring & input){
  std::wstring out;
  out+=L'\"';
  for (std::wstring::const_iterator it=input.begin();it!=input.end();++it){
    switch(*it){
    case L'\"':
      out+=L'\\';out+=L'\"';
      break;
    case L'\\':
      out+=L'\\';out+=L'\\';
      break;
    case L'/':
      out+=L'\\';out+=L'/';
      break;
    case L'\b':
      out+=L'\\';out+=L'b';
      break;
    case L'\f':
      out+=L'\\';out+=L'f';
      break;
    case L'\n':
      out+=L'\\';out+=L'n';
      break;
    case L'\r':
      out+=L'\\';out+=L'r';
      break;
    case L'\t':
      out+=L'\\';out+=L't';
      break;
    default:
      out+=*it;
      break;
    }
  }
  out+=L'\"';
  return(out);
}
std::wstring unescapeString(const std::wstring & input){
  std::wstring out;
  //! Informacja, czy był znak cudzysłów.
  bool quot=false;
  //! Poprzedni przetwarzany znak.
  wchar_t last_char=L'\0';
  wchar_t hex_char;
  int i=-1;
  for (std::wstring::const_iterator it=input.begin();it!=input.end();++it){
    if (quot){//Jeśli znak cudzysłów był. 
      if (i<0){//To nie jest analiza zakodowanej heksadecymalnie liczby.
        if (last_char==L'\\'){//Jeśli ostatnim był znak ucieczki.
          switch(*it){
          case L'\"':
            out+=L'\"';
            break;
          case L'\\':
            out+=L'\\';
            break;
          case L'/':
            out+=L'/';
            break;
          case L'b':case L'B':
            out+=L'\b';
            break;
          case L'f':case L'F':
            out+=L'\f';
            break;
          case L'n':case L'N':
            out+=L'\n';
            break;
          case L'r':case L'R':
            out+=L'\r';
            break;
          case L't':case L'T':
            out+=L'\t';
            break;
          case L'u':case L'U':
            i=1;
            hex_char=0x0;
            break;
          default:
            out+=*it;
            break;
          }
        } else {//Jeśli ostatnim nie był znak ucieczki.
          if (*it==L'"'){//Jeśli mamy cudzysłów i poprzednim znakiem nie był znak ucieczki.
            quot=false;//Koniec ciągu w cudzysłów.
          } else {
            if (*it!=L'\\') out+=*it;//Zapis bez analizy jeśli nie jest to znak ucieczki.
          }
        }
      } else {//To jest analiza zakodowanej heksadecymalnie liczby.
        hex_char<<=4;
        switch (*it){
        default: case L'0':
          break;
        case L'1':
          hex_char|=0x1;
          break;
        case L'2':
          hex_char|=0x2;
          break;
        case L'3':
          hex_char|=0x3;
          break;
        case L'4':
          hex_char|=0x4;
          break;
        case L'5':
          hex_char|=0x5;
          break;
        case L'6':
          hex_char|=0x6;
          break;
        case L'7':
          hex_char|=0x7;
          break;
        case L'8':
          hex_char|=0x8;
          break;
        case L'9':
          hex_char|=0x9;
          break;
        case L'a':case L'A':
          hex_char|=0xa;
          break;
        case L'b':case L'B':
          hex_char|=0xb;
          break;
        case L'c':case L'C':
          hex_char|=0xc;
          break;
        case L'd':case L'D':
          hex_char|=0xd;
          break;
        case L'e':case L'E':
          hex_char|=0xe;
          break;
        case L'f':case L'F':
          hex_char|=0xf;
          break;
        }
        i++;
        if (i>4) {
          out+=hex_char;
          i=-1;
        }
      }
    } else {//Jeśli znaku cudzysłów nie było.
      if (L'"'==*it){
        quot=true;//Początek ciągu w cudzysłów.
      } else {
        out+=*it;//Zapis bez analizy.
      }
    }
    last_char=*it;
  }
  return(out);
}
//============================================
}
//============================================
int Base::parseJson(std::string & input){
  int o;
  std::wstring in;
  ict::utf8::transfer(input,in);
  o=parseJson(in);
  ict::utf8::transfer(in,input);
  return(o);
}
int Base::serializeJson(std::string & output){
  int o;
  std::wstring out;
  o=serializeJson(out);
  ict::utf8::transfer(out,output);
  return(o);
}
int Base::infoJson(std::string & output){
  int o;
  std::wstring out;
  o=infoJson(out);
  ict::utf8::transfer(out,output);
  return(o);
}
int Base::parseJson(std::wstring & input){
  try {
    if (jsonParams_beforeParse) DO_JOB(jsonParams_beforeParse())
    DO_JOB(parseJsonThis(input))
    if (jsonParams_afterParse) DO_JOB(jsonParams_afterParse())
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (jsonParams_elementName) error<<"{"<<(*jsonParams_elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
int Base::serializeJson(std::wstring & output){
  try {
    if (jsonForceInfo){
      DO_JOB(infoJsonThis(output))
    } else {
      if (jsonParams_beforeSerialize) DO_JOB(jsonParams_beforeSerialize())
      DO_JOB(serializeJsonThis(output))
      if (jsonParams_afterSerialize) DO_JOB(jsonParams_afterSerialize())
    }
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (jsonParams_elementName) error<<"{"<<(*jsonParams_elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
int Base::infoJson(std::wstring & output){
  try {
    DO_JOB(infoJsonThis(output))
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (jsonParams_elementName) error<<"{"<<(*jsonParams_elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
int Base::testJson(){
  try {
    DO_JOB(testJsonThis())
    if (jsonParams_extraTest) DO_JOB(jsonParams_extraTest())
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (jsonParams_elementName) error<<"{"<<(*jsonParams_elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
//===========================================
const std::wstring _true_(L"true");
const std::wstring _false_(L"false");
int BoolType::parseJsonThis(std::wstring & input){
  std::wstring s;
  if (get_json_element(input,s)){
    if (_true_==s.substr(0,_true_.size())){
      BaseType<bool>::value=true;
      return(0);
    }
    if (_false_==s.substr(0,_false_.size())){
      BaseType<bool>::value=false;
      return(0);
    }
  }
  if ((_true_.size()<input.size())&&(_false_.size()<input.size())) throw std::invalid_argument("Zbyt długa wartość JSON Bool - błąd parsowania!");
  return(1);
}
int BoolType::serializeJsonThis(std::wstring & output){
  std::wstring o(BaseType<bool>::value?_true_:_false_);
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
int BoolType::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new BoolInfo);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType)
    DO_INFO(BaseType<bool>,elementName)
    DO_INFO(BaseType<bool>,elementInfo)
    DO_INFO(BaseType<bool>,elementDesc)
    DO_INFO(BaseType<bool>,jsValidate)
    DO_INFO(BoolType,exampleIn)
    DO_INFO(BoolType,exampleOut)
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
int BoolType::testJsonThis(){
  return(0);
}
//===========================================
const std::wstring _null_(L"null");
int NullType::parseJsonThis(std::wstring & input){
  std::wstring s;
  if (get_json_element(input,s)){
    if (_null_==s.substr(0,_null_.size())){
      BaseType<bool>::value=false;
      return(0);
    }
  }
  if (_null_.size()<input.size())  throw std::invalid_argument("Zbyt długa wartość JSON Null - błąd parsowania!");
  return(1);
}
int NullType::serializeJsonThis(std::wstring & output){
  std::wstring o(_null_);
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
int NullType::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new NullInfo);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType);
    DO_INFO(BaseType<bool>,elementName)
    DO_INFO(BaseType<bool>,elementInfo)
    DO_INFO(BaseType<bool>,elementDesc)
    DO_INFO(BaseType<bool>,jsValidate)
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
int NullType::testJsonThis(){
  return(0);
}
//===========================================
int StringType::parseJsonThis(std::wstring & input){
  std::wstring s;
  if (get_json_element(input,s)){
    assign(ict::utf8::get(string::unescapeString(s)));
    return(0);
  }
  if (100000000<input.size())  throw std::invalid_argument("Zbyt długa wartość JSON String - błąd parsowania!");
  return(1);
}
int StringType::serializeJsonThis(std::wstring & output){
  std::wstring o(string::escapeString(ict::utf8::get(*this)));
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
int StringType::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new StringInfo);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType);
    DO_INFO(Base,elementName)
    DO_INFO(Base,elementInfo)
    DO_INFO(Base,elementDesc)
    DO_INFO(Base,jsValidate)
    DO_INFO(StringType,minLen)
    DO_INFO(StringType,maxLen)
    DO_INFO(StringType,regex)
    DO_INFO(StringType,exampleIn)
    DO_INFO(StringType,exampleOut)
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
int StringType::testJsonThis(){
  bool e=false;
  std::ostringstream error;
  if (jsonParams_minLen){//Jeśli jest minimalna długość.
    if ((*jsonParams_minLen)>size()) {//Sprawdź minimalną długość.
      error<<"Nieprawidłowa długość elementu JSON String - powinna być większa/równa niż "<<(*jsonParams_minLen)<<"! ";
      e=true;
    }
  }
  if (jsonParams_maxLen){//Jeśli jest maksymalna długość.
    if ((*jsonParams_maxLen)<size()) {//Sprawdź maksymalną długość.
      error<<"Nieprawidłowa długość elementu JSON String - powinna być mniejsza/równa niż "<<(*jsonParams_maxLen)<<"! ";
      e=true;
    }
  }
  if (jsonParams_regex){//Jeśli jest wyrażenie regularne.
    std::regex r(*jsonParams_regex);
    if (!std::regex_match(*this,r)) {//Sprawdź wyrażenie regularne.
      error<<"Nieprawidłowy format (regex) elementu JSON String - powinien spełniać \""<<(*jsonParams_regex)<<"\"! ";
      e=true;
    }
  }
  if (e) throw std::invalid_argument(error.str());
  return(0);
}
//===========================================
Base * ObjectType0::getPropPointer(std::size_t offset){
  char * pointer=((char*)this)+offset;
  return((Base*)pointer);
}
std::size_t ObjectType0::getPropOffset(Base * pointer){
  std::size_t offset=((char*)pointer)-((char*)this);
  return(offset);
}
int ObjectType1::parseJsonThis(std::wstring & input){
  if (!parse_name) parse_name.reset(new StringType);
  if (!done.parse.begin_all){
    remove_ws(input);
    hideAllJsonProp();
    if (input.size()){
      if (input.front()==L'{'){
        input.erase(0,1);
      } else {
        std::ostringstream error;
        error<<"Niepoprawne otwarcie JSON Object ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
        throw std::invalid_argument(error.str());
      }
    } else return(1);
    done.parse.begin_all=1;
  }
  for (;;parseIndex++){
    if (!done.parse.begin_el){
      remove_ws(input);
      done.parse.begin_el=1;
    }
    if (!done.parse.el_name){
      try{
        DO_JOB(parse_name->parseJson(input))
      } catch(const std::invalid_argument& exc){
        std::ostringstream error;
        error<<"Nieprawidłowa nazwa elementu w JSON Object! ";
        error<<exc.what();
        throw std::invalid_argument(error.str());
      }
      if (!prop_map.count(*parse_name)) {
        std::ostringstream error;
        error<<"Nieznany element ('"<<(*parse_name)<<"') w JSON Object - błąd parsowania!";
        throw std::invalid_argument(error.str());
      }
      if (!prop_hidden.count(prop_map.at(*parse_name))) {
        std::ostringstream error;
        error<<"Powtórzony element ('"<<(*parse_name)<<"') w JSON Object - błąd parsowania!";
        throw std::invalid_argument(error.str());
      }
      done.parse.el_name=1;
    }
    if (!done.parse.between_el){
      remove_ws(input);
      if (input.size()){
        if (input.front()==L':'){
          input.erase(0,1);
        } else {
          std::ostringstream error;
          error<<"Niepoprawne rozdzielenie elementów ('"<<(*parse_name)<<"') JSON Object ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
          throw std::invalid_argument(error.str());
        }
      } else return(1);
      done.parse.between_el=1;
    }
    if (!done.parse.el_value){
      try{
        if (prop_map.count(*parse_name)){
          DO_JOB(getPropPointer(prop_map.at(*parse_name))->parseJson(input))
          if (prop_hidden.count(prop_map.at(*parse_name))) 
            prop_hidden.erase(prop_map.at(*parse_name));
        }
      } catch(const std::invalid_argument& exc){
        std::ostringstream error;
        error<<"'"<<(*parse_name)<<"' / ";
        error<<exc.what();
        throw std::invalid_argument(error.str());
      }
      done.parse.el_value=1;
    }
    if (!done.parse.end_el){
      remove_ws(input);
      if (input.size()){
        if ((input.front()==L',')||(input.front()==L'}')){
          if (input.front()==L','){
            input.erase(0,1);
          } else break;
        } else {
          std::ostringstream error;
          error<<"Niepoprawne rozdzielenie elementów ('"<<(*parse_name)<<"') JSON Object ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
          throw std::invalid_argument(error.str());
        }
      } else return(1);
      done.parse.end_el=1;
    }
    if (10000<parseIndex) throw std::invalid_argument("Zbyt duża liczba elementów JSON Object - błąd parsowania!");
    done.parse.begin_el=0;
    done.parse.el_name=0;
    done.parse.between_el=0;
    done.parse.el_value=0;
    done.parse.end_el=0;
  }
  if (!done.parse.end_all){
    remove_ws(input);
    if (input.size()){
      if (input.front()==L'}'){
        input.erase(0,1);
      } else {
        std::ostringstream error;
        error<<"Niepoprawne zamknięcie JSON Object ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
        throw std::invalid_argument(error.str());
      }
    } else return(1);
    done.parse.end_all=1;
  }
  //========================
  done.parse.begin_all=0;
  done.parse.end_all=0;
  parseIndex=0;
  parse_name.reset(nullptr);
  return(0);
}
int ObjectType1::serializeJsonThis(std::wstring & output){
  if (!serialize_name) serialize_name.reset(new StringType);
  if (!done.serialize.begin_all){
    serializeIterator=prop_map.begin();
    if ((1+output.size())<output.max_size()){
      output+=L'{';
    } else return(1);
    done.serialize.begin_all=1;
  }
  for (;serializeIterator!=prop_map.end();++serializeIterator) if ((!prop_hidden.count(serializeIterator->second))||jsonForceInfoInObject){
    if (!done.serialize.begin_el){
      if (serializeIndex) {
        if ((1+output.size())<output.max_size()){
          output+=L',';
        } else return(1);
      }
      done.serialize.begin_el=1;
    }
    if (!done.serialize.el_name){
      (*serialize_name)=serializeIterator->first;
      DO_JOB(serialize_name->serializeJson(output))
      done.serialize.el_name=1;
    }
    if (!done.serialize.between_el){
      if ((1+output.size())<output.max_size()){
        output+=L':';
      } else return(1);
      done.serialize.between_el=1;
    }
    if (!done.serialize.el_value){
      try{
        if (jsonForceInfoInObject){
          DO_JOB(getPropPointer(serializeIterator->second)->infoJson(output))
        } else {
          DO_JOB(getPropPointer(serializeIterator->second)->serializeJson(output))
        }
      } catch(const std::invalid_argument& exc){
        std::ostringstream error;
        error<<"'"<<(*serialize_name)<<"' / ";
        error<<exc.what();
        throw std::invalid_argument(error.str());
      }
      done.serialize.el_value=1;
    }
    if (!done.serialize.end_el){
      done.serialize.end_el=1;
    }
    done.serialize.begin_el=0;
    done.serialize.el_name=0;
    done.serialize.between_el=0;
    done.serialize.el_value=0;
    done.serialize.end_el=0;
    serializeIndex++;
  }
  if (!done.serialize.end_all){
    if ((1+output.size())<output.max_size()){
      output+=L'}';
    } else return(1);
    done.serialize.end_all=1;
  }
  //========================
  done.serialize.begin_all=0;
  done.serialize.end_all=0;
  serializeIndex=0;
  serialize_name.reset(nullptr);
  return(0);
}
int ObjectType1::infoJsonThis(std::wstring & output){
  return(0);
}
int ObjectType1::testJsonThis(){
  std::size_t idx=0;
  bool e=false;
  std::ostringstream error;
  for (prop_map_t::iterator it=prop_map.begin();it!=prop_map.end();++it){
    if (prop_mandatory.count(it->second)&&prop_hidden.count(it->second)){
      error<<"Brak obowiązkowego elementu JSON Object ('"<<it->first<<"')! ";
      e=true;
    }
  }
  if (e) throw std::invalid_argument(error.str());
  for (prop_map_t::iterator it=prop_map.begin();it!=prop_map.end();++it){
    try{
      if (it->second) {
        DO_JOB(getPropPointer(it->second)->testJson())
      }
    }catch(const std::invalid_argument& exc){
      std::ostringstream error;
      error<<"'"<<it->first<<"' / ";
      error<<exc.what();
      throw std::invalid_argument(error.str());
    }
  }
  return(0);
}
void ObjectType1::registerProp(Base * element,const std::string & name,bool mandatory){
  if (element){
    std::size_t offset=getPropOffset(element);
    prop_map[name]=offset;
    if (mandatory) if (!prop_mandatory.count(offset)) prop_mandatory.insert(offset);
  }
}
void ObjectType1::hideJsonProp(Base * element){
  if (element){
    std::size_t offset=getPropOffset(element);
    if (!prop_hidden.count(offset)) prop_hidden.insert(offset);
  }
}
void ObjectType1::showJsonProp(Base * element){
  if (element){
    std::size_t offset=getPropOffset(element);
    if (prop_hidden.count(offset)) prop_hidden.erase(offset);
  }
}
void ObjectType1::hideAllJsonProp(){
  for (prop_map_t::const_iterator it=prop_map.cbegin();it!=prop_map.cend();++it){
    if (!prop_hidden.count(it->second)) prop_hidden.insert(it->second);
  }
}
void ObjectType1::showAllJsonProp(){
  prop_hidden.clear();
}
std::map<std::string,bool> ObjectType1::getAllJsonProp(){
  std::map<std::string,bool> m;
  for (prop_map_t::iterator it=prop_map.begin();it!=prop_map.end();++it){
    m[it->first]=(prop_mandatory.count(it->second))?true:false;
  }
  return(m);
}
//===========================================
int MutableType1::parseJsonThis(std::wstring & input){
  throw std::invalid_argument("TODO");
  return(0);
}
int MutableType1::serializeJsonThis(std::wstring & output){
  for (prop_map_t::iterator it=prop_map.begin();it!=prop_map.end();++it){
    if (prop_selected==it->second) try{
      if (it->second) {
        DO_JOB(getPropPointer(it->second)->serializeJson(output))
        return(0);
      }
    }catch(const std::invalid_argument& exc){
      std::ostringstream error;
      error<<"'"<<it->first<<"' / ";
      error<<exc.what();
      throw std::invalid_argument(error.str());
    }
  }
  {
    std::wstring o(_null_);
    if ((o.size()+output.size())<output.max_size()){
      output+=o;
      return(0);
    }
    return(1);
  }
}
int MutableType1::infoJsonThis(std::wstring & output){
  return(0);
}
int MutableType1::testJsonThis(){
  for (prop_map_t::iterator it=prop_map.begin();it!=prop_map.end();++it){
    if (prop_selected==it->second) try{
      if (it->second) {
        DO_JOB(getPropPointer(it->second)->testJson())
        return(0);
      }
    }catch(const std::invalid_argument& exc){
      std::ostringstream error;
      error<<"'"<<it->first<<"' / ";
      error<<exc.what();
      throw std::invalid_argument(error.str());
    }
  }
  throw std::logic_error("Brak wybranego typu elementu JSON !");
  return(0);
}
void MutableType1::registerProp(Base * element,const std::string & name){
  if (element){
    std::size_t offset=getPropOffset(element);
    prop_map[name]=offset;
    if (prop_selected<0) prop_selected=offset;
  }
}
void MutableType1::selectJsonProp(Base * element){
  if (element){
    prop_selected=getPropOffset(element);
  }
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
class IntTest: public ict::jobject::int_t{
public:
  IntTest(){
    JOBJECT_NAME("IntTest")
  }
};
class LIntTest: public ict::jobject::l_int_t{
public:
  LIntTest(){
    JOBJECT_NAME("LIntTest")
  }
};
class UIntTest: public ict::jobject::u_int_t{
public:
  UIntTest(){
    JOBJECT_NAME("UIntTest")
  }
};
class LLIntTest: public ict::jobject::ll_int_t{
public:
  LLIntTest(){
    JOBJECT_NAME("LLIntTest")
  }
};
class ULLIntTest: public ict::jobject::ull_int_t{
public:
  ULLIntTest(){
    JOBJECT_NAME("ULLIntTest")
  }
};
class FloatTest: public ict::jobject::float_t{
public:
  FloatTest(){
    JOBJECT_NAME("FloatTest")
  }
};
class DoubleTest: public ict::jobject::double_t{
public:
  DoubleTest(){
    JOBJECT_NAME("DoubleTest")
  }
};
class LDoubleTest: public ict::jobject::l_double_t{
public:
  LDoubleTest(){
    JOBJECT_NAME("LDoubleTest")
  }
};
class BoolTest: public ict::jobject::bool_t{
public:
  BoolTest(){
    JOBJECT_NAME("BoolTest")
  }
};
class NullTest: public ict::jobject::null_t{
public:
  NullTest(){
    JOBJECT_NAME("NullTest")
  }
};
class StringTest: public ict::jobject::string_t{
public:
  StringTest(){
    JOBJECT_NAME("StringTest")
  }
};
class ObjectTest: public ict::jobject::object_t<ObjectTest>{
public:
  IntTest int1;
  LIntTest int2;
  UIntTest int3;
  LLIntTest int4;
  ULLIntTest int5;
  FloatTest float1;
  DoubleTest float2;
  LDoubleTest float3;
  BoolTest bool_v;
  NullTest null_v;
  StringTest string_v;
  ObjectTest(){
    JOBJECT_NAME("ObjectTest")
    JOBJECT_PROP(int1)
    JOBJECT_PROP(int2)
    JOBJECT_PROP(int3)
    JOBJECT_PROP(int4)
    JOBJECT_PROP(int5)
    JOBJECT_PROP(float1)
    JOBJECT_PROP(float2)
    JOBJECT_PROP(float3)
    JOBJECT_PROP(bool_v)
    JOBJECT_PROP(null_v)
    JOBJECT_PROP(string_v,true)
  }
};
class ArrayTest: public ict::jobject::array_t<ObjectTest>{
public:
  ArrayTest(){
    JOBJECT_NAME("ArrayTest")
  }
};
REGISTER_TEST(jobject,tc1){
  try{
    std::string b;
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
}
REGISTER_TEST(jobject,tc2){
  try{
    int out;
    std::string b;
    ArrayTest arrayTest1;
    ArrayTest arrayTest2;
    arrayTest1.emplace_back();
    arrayTest1.emplace_back();
    arrayTest1[0].int1()=3;
    arrayTest1[0].int2()=65;
    arrayTest1[0].int3()=656;
    arrayTest1[0].int4()=0x5555;
    arrayTest1[0].int5()=55353;
    arrayTest1[0].float1()=2.4;
    arrayTest1[0].float2()=66.001;
    arrayTest1[0].float3()=56.4e3;
    arrayTest1[0].bool_v()=true;
    arrayTest1[0].null_v();
    arrayTest1[0].string_v()="a kuku!";
    arrayTest1[1].int1()=34;
    arrayTest1[1].int2()=655;
    arrayTest1[1].int3()=6576;
    arrayTest1[1].int4()=0x55455;
    arrayTest1[1].int5()=553453;
    arrayTest1[1].float1()=2.47;
    arrayTest1[1].float2()=66.0071;
    arrayTest1[1].float3()=56.4e-30;
    arrayTest1[1].bool_v()=false;
    arrayTest1[1].null_v();
    arrayTest1[1].string_v()="a kuku2!";

    if (out=arrayTest1.testJson()) {
      std::cerr<<"ERROR: arrayTest1.testJson(): "<<out<<"!"<<std::endl;
      return(1);
    }
    while(arrayTest1.serializeJson(b)==1){}
    std::cerr<<"arrayTest1.serializeJson(b)>"<<b<<"<"<<std::endl;

    if (out=arrayTest2.parseJson(b)) {
      std::cerr<<"ERROR: arrayTest2.parseJson(b): "<<out<<", '"<<b<<"'!"<<std::endl;
      return(1);
    }
    b.clear();
    while(arrayTest2.serializeJson(b)==1){}
    std::cerr<<"arrayTest2.serializeJson(b)>"<<b<<"<"<<std::endl;

    if (out=arrayTest2.testJson()) {
      std::cerr<<"ERROR: arrayTest2.testJson(): "<<out<<"!"<<std::endl;
      return(1);
    }
    if (arrayTest2.size()!=2){
      std::cerr<<"ERROR: arrayTest2.size()!=2 : "<<arrayTest2.size()<<" !"<<std::endl;
      return(1);
    }
  #define TC2_TEST(index,value) \
    if (arrayTest1[index].value()!=arrayTest2[index].value()){ \
      std::cerr<<"ERROR: arrayTest1[" #index "]." #value "()!=arrayTest2[" #index "]." #value "() : "<<arrayTest1[index].value()<<"!="<<arrayTest2[index].value()<<" !"<<std::endl; \
      return(1); \
    }
    TC2_TEST(0,int1)
    TC2_TEST(0,int2)
    TC2_TEST(0,int3)
    TC2_TEST(0,int4)
    TC2_TEST(0,int5)
    TC2_TEST(0,float1)
    TC2_TEST(0,float2)
    TC2_TEST(0,float3)
    TC2_TEST(0,bool_v)
    TC2_TEST(0,null_v)
    TC2_TEST(0,string_v)
    TC2_TEST(1,int1)
    TC2_TEST(1,int2)
    TC2_TEST(1,int3)
    TC2_TEST(1,int4)
    TC2_TEST(1,int5)
    TC2_TEST(1,float1)
    TC2_TEST(1,float2)
    //TC2_TEST(1,float3)
    TC2_TEST(1,bool_v)
    TC2_TEST(1,null_v)
    TC2_TEST(1,string_v)
  }catch(const std::exception & e){
    std::cerr<<"ERROR: "<<e.what()<<"!"<<std::endl;
    return(1);
  }
  return(0);
}
#endif
//===========================================
