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
  if (base::name) {\
    info_ptr->name()=(*base::name);\
    info_ptr->showJsonProp(info_ptr->name);\
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
    default: break;
  }
  return("unknown");
}
//============================================
namespace number {
//============================================
void from_string(const std::wstring & input,int & output){
  output=std::stoi(input); //Convert string to integer (function template )
}
void from_string(const std::wstring & input,long int & output){
  output=std::stol(input); //Convert string to long int (function template )
}
void from_string(const std::wstring & input,unsigned int & output){
  output=std::stoul(input); //Convert string to unsigned integer (function template )
}
void from_string(const std::wstring & input,long long & output){
  output=std::stoll(input); // Convert string to long long (function template )
}
void from_string(const std::wstring & input,unsigned long long & output){
  output=std::stoull(input); // Convert string to unsigned long long (function template )
}
void from_string(const std::wstring & input,float & output){
  output=std::stof(input); // Convert string to float (function template )
}
void from_string(const std::wstring & input,double & output){
  output=std::stod(input); // Convert string to double (function template )
}
void from_string(const std::wstring & input,long double & output){
  output=std::stold(input); //Convert string to long double (function template )
}
void to_string(std::wstring & output,const int & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const long int & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const unsigned int & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const long long & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const unsigned long long & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const float & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const double & input){
  output=std::to_wstring(input);
}
void to_string(std::wstring & output,const long double & input){
  output=std::to_wstring(input);
}
//============================================
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
  std::wstring in;
  ict::utf8::transfer(input,in);
  return(parseJson(in));
}
int Base::serializeJson(std::string & output){
  std::wstring out;
  int o=serializeJson(out);
  ict::utf8::transfer(out,output);
  return(o);
}
int Base::infoJson(std::string & output){
  std::wstring out;
  int o=infoJson(out);
  ict::utf8::transfer(out,output);
  return(o);
}
int Base::parseJson(std::wstring & input){
  try {
    if (beforeParse) DO_JOB(beforeParse())
    DO_JOB(parseThis(input))
    if (afterParse) DO_JOB(afterParse())
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (elementName) error<<"{"<<(*elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
int Base::serializeJson(std::wstring & output){
  try {
    if (beforeSerialize) DO_JOB(beforeSerialize())
    DO_JOB(serializeThis(output))
    if (afterSerialize) DO_JOB(afterSerialize())
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (elementName) error<<"{"<<(*elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
int Base::infoJson(std::wstring & output){
  try {
    DO_JOB(infoThis(output))
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (elementName) error<<"{"<<(*elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(0);
}
int Base::testJson(){
  try {
    DO_JOB(testThis())
    if (extraTest) DO_JOB(extraTest())
  } catch(const std::invalid_argument& exc){
    std::ostringstream error;
    if (elementName) error<<"{"<<(*elementName)<<"} ";
    error<<exc.what();
    throw std::invalid_argument(error.str());
  }
  return(true);
}
//===========================================
const std::wstring _true_(L"true");
const std::wstring _false_(L"false");
int BoolType::parseThis(std::wstring & input){
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
int BoolType::serializeThis(std::wstring & output){
  std::wstring o(BaseType<bool>::value?_true_:_false_);
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
int BoolType::infoThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new BoolInfo);
    info_ptr->hideAllJsonProp();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->showJsonProp(info_ptr->jsonType);
    DO_INFO(BaseType<bool>,elementName)
    DO_INFO(BaseType<bool>,elementDesc)
    DO_INFO(BaseType<bool>,jsValidate)
    DO_INFO(BoolType,exampleIn)
    DO_INFO(BoolType,exampleOut)
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
int BoolType::testThis(){
  return(0);
}
//===========================================
const std::wstring _null_(L"null");
int NullType::parseThis(std::wstring & input){
  std::wstring s;
  if (get_json_element(input,s)){
    if (_null_==s.substr(0,_null_.size())){
      BaseType<bool>::value=true;
      return(0);
    }
  }
  if (_null_.size()<input.size())  throw std::invalid_argument("Zbyt długa wartość JSON Null - błąd parsowania!");
  return(1);
}
int NullType::serializeThis(std::wstring & output){
  std::wstring o(_null_);
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
int NullType::infoThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new NullInfo);
    info_ptr->hideAllJsonProp();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->showJsonProp(info_ptr->jsonType);
    DO_INFO(BaseType<bool>,elementName)
    DO_INFO(BaseType<bool>,elementDesc)
    DO_INFO(BaseType<bool>,jsValidate)
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
int NullType::testThis(){
  return(0);
}
//===========================================
int StringType::parseThis(std::wstring & input){
  std::wstring s;
  if (get_json_element(input,s)){
    assign(ict::utf8::get(s));
    return(0);
  }
  if (100000000<input.size())  throw std::invalid_argument("Zbyt długa wartość JSON String - błąd parsowania!");
  return(1);
}
int StringType::serializeThis(std::wstring & output){
  std::wstring o(string::escapeString(ict::utf8::get(*this)));
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
int StringType::infoThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new StringInfo);
    info_ptr->hideAllJsonProp();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->showJsonProp(info_ptr->jsonType);
    DO_INFO(Base,elementName)
    DO_INFO(Base,elementDesc)
    DO_INFO(Base,jsValidate)
    /*DO_INFO(StringType,minLen)
    DO_INFO(StringType,maxLen)
    DO_INFO(StringType,regex)
    DO_INFO(StringType,exampleIn)
    DO_INFO(StringType,exampleOut)*/
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
int StringType::testThis(){
  bool e=false;
  std::ostringstream error;
  if (minLen){//Jeśli jest minimalna długość.
    if ((*minLen)>size()) {//Sprawdź minimalną długość.
      error<<"Nieprawidłowa długość elementu JSON String - powinna być większa/równa niż "<<(*minLen)<<"! ";
      e=true;
    }
  }
  if (maxLen){//Jeśli jest maksymalna długość.
    if ((*maxLen)<size()) {//Sprawdź maksymalną długość.
      error<<"Nieprawidłowa długość elementu JSON String - powinna być mniejsza/równa niż "<<(*maxLen)<<"! ";
      e=true;
    }
  }
  if (regex){//Jeśli jest wyrażenie regularne.
    std::regex r(*regex);
    if (!std::regex_match(*this,r)) {//Sprawdź wyrażenie regularne.
      error<<"Nieprawidłowy format (regex) elementu JSON String - powinien spełniać \""<<(*regex)<<"\"! ";
      e=true;
    }
  }
  if (e) throw std::invalid_argument(error.str());
  return(0);
}
//===========================================
int ObjectType1::parseThis(std::wstring & input){
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
        if (prop_map.count(*parse_name)) 
          if (prop_map.at(*parse_name)) {
            DO_JOB(prop_map.at(*parse_name)->parseJson(input))
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
int ObjectType1::serializeThis(std::wstring & output){
  if (!serialize_name) serialize_name.reset(new StringType);
  if (!done.serialize.begin_all){
    serializeIterator=prop_map.begin();
    if ((1+output.size())<output.max_size()){
      output+=L'{';
    } else return(1);
    done.serialize.begin_all=1;
  }
  for (;serializeIterator!=prop_map.end();++serializeIterator) if ((!prop_hidden.count(serializeIterator->second))||do_info_json){
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
        if (serializeIterator->second){
          if (do_info_json){
            DO_JOB(serializeIterator->second->infoJson(output))
          } else {
            DO_JOB(serializeIterator->second->serializeJson(output))
          }
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
int ObjectType1::infoThis(std::wstring & output){
  return(0);
}
int ObjectType1::testThis(){
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
      if (it->second) DO_JOB(it->second->testJson())
    }catch(const std::invalid_argument& exc){
      std::ostringstream error;
      error<<"'"<<it->first<<"' / ";
      error<<exc.what();
      throw std::invalid_argument(error.str());
    }
  }
  return(0);
}
void ObjectType1::registerProp(Base & element,const std::string & name,bool mandatory){
  prop_map[name]=&element;
  if (mandatory) if (!prop_mandatory.count(&element)) prop_mandatory.insert(&element);
}
void ObjectType1::hideJsonProp(Base & element){
  if (!prop_hidden.count(&element)) prop_hidden.insert(&element);
}
void ObjectType1::showJsonProp(Base & element){
  if (prop_hidden.count(&element)) prop_hidden.erase(&element);
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
  std::string b;
  ArrayTest arrayTest;
  arrayTest.emplace_back();
  arrayTest[0].int1()=2;
  arrayTest[0].float2()=2.9;
  arrayTest[0].string_v()="a kuku!";

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

  while(arrayTest[0].serializeJson(b)==1){}
  std::cerr<<">"<<b<<"<"<<std::endl;b.clear();
  while(arrayTest.serializeJson(b)==1){}
  std::cerr<<">"<<b<<"<"<<std::endl;b.clear();
  while(arrayTest[0].int1.infoJson(b)==1){}
  std::cerr<<">"<<b<<"<"<<std::endl;b.clear();
  while(arrayTest[0].infoJson(b)==1){}
  std::cerr<<">"<<b<<"<"<<std::endl;b.clear();
  return(0);
}
#endif
//===========================================
