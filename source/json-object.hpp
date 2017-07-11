//! @file
//! @brief JSON object module - Header file.
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
#ifndef _JSON_OBJECT_HEADER
#define _JSON_OBJECT_HEADER
//============================================
#include "utf8.hpp"
#include <functional>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <memory>
//============================================
#define JOBJECT_NAME(s) {const static std::string _elementName(s);jsonParams_elementName=&_elementName;}
#define JOBJECT_INFO(s) {const static std::string _elementInfo(s);jsonParams_elementInfo=&_elementInfo;}
#define JOBJECT_DESC(s) {const static std::vector<std::string> _elementDesc(s);jsonParams_elementDesc=&_elementDesc;}
#define JOBJECT_VALIDATE(s) {const static std::string _jsValidate(s);jsonParams_jsValidate=&_jsValidate;}

#define JOBJECT_PROP(element,...) registerProp(&(this->element),#element,##__VA_ARGS__);
#define JOBJECT_HIDE(element) hideJsonProp(&(this->element));
#define JOBJECT_SHOW(element) showJsonProp(&(this->element));
#define JOBJECT_HIDE_ALL() hideAllJsonProp();
#define JOBJECT_SHOW_ALL() showAllJsonProp();
#define JOBJECT_SELECT(element) selectJsonProp(&(this->element));

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
//===========================================
enum json_type_t {
  json_type_null,
  json_type_bool,
  json_type_number,
  json_type_string,
  json_type_array,
  json_type_object,
  json_type_mutable
};
typedef std::function<int(void)> job_t;
template<typename T> class NumberType;
class BoolType;
class NullType;
class StringType;
template<class E> class ArrayType;
class ObjectType1;
template<class O> class ObjectType2;
template<typename T> class NumberInfo;
class BoolInfo;
class NullInfo;
class StringInfo;
template<class E> class ArrayInfo;
template<class E> class ObjectInfo;
template<class E> class MutableInfo;
//===========================================
void remove_ws(std::wstring & input);
bool get_json_element(std::wstring & input,std::wstring & output);
std::string get_type_name(json_type_t type);
//===========================================
namespace number {
//===========================================
template<typename T>
int getFromString(const std::wstring & input,T & output){
  try{
    std::wistringstream s;
    s.str(input);
    s>>output;
  }catch(...){
    std::ostringstream error;
    error<<"Niepoprawna wartość JSON Number ('"<<ict::utf8::get(input)<<"') - błąd parsowania!";
    throw std::invalid_argument(error.str());
  }
  return(0);
}
template<typename T>
int putToString(std::wstring & output,T & input){
  try{
    std::wostringstream s;
    s<<input;
    output=s.str();
  }catch(...){
    std::ostringstream error;
    error<<"Niepoprawna wartość JSON Number ('"<<input<<"') - błąd serializacji!";
    throw std::invalid_argument(error.str());
  }
  return(1);
}
//===========================================
}
//===========================================
namespace string {
//============================================
std::wstring escapeString(const std::wstring & input);
std::wstring unescapeString(const std::wstring & input);
//===========================================
}
//===========================================
class Base{
protected:
  job_t jsonParams_beforeParse;
  job_t jsonParams_afterParse;
  job_t jsonParams_beforeSerialize;
  job_t jsonParams_afterSerialize;
  job_t jsonParams_extraTest;
  //! Nazwa elementu JSON (opcjonalnie).
  const std::string * jsonParams_elementName=nullptr;
  //! Opis skrócony elementu JSON (opcjonalnie).
  const std::string * jsonParams_elementInfo=nullptr;
  //! Opis elementu JSON (opcjonalnie).
  const std::vector<std::string> * jsonParams_elementDesc=nullptr;
  //! Kod (ciało funkcji) JS do walidowania zawartości (opcjonalnie).
  const std::string * jsonParams_jsValidate=nullptr;
  //! Podmienia serializeJson na infoJson.
  bool jsonForceInfo=false;
  json_type_t json_type=json_type_null;
protected:
  virtual int parseJsonThis(std::wstring & input)=0;
  virtual int serializeJsonThis(std::wstring & output)=0;
  virtual int infoJsonThis(std::wstring & output)=0;
  virtual int testJsonThis()=0;
public:
  virtual ~Base(){}
  int parseJson(std::wstring & input);
  int parseJson(std::string & input);
  int serializeJson(std::wstring & output);
  int serializeJson(std::string & output);
  int infoJson(std::wstring & output);
  int infoJson(std::string & output);
  int testJson();
};
//===========================================
template<typename T>
class BaseType: public Base{
protected:
  typedef T value_t;
  value_t value;
public:
  value_t & operator()() {return(value);}
};
//===========================================
template<typename T>
class NumberType: public BaseType<T>{
private:
  std::unique_ptr<NumberInfo<T>> info_ptr;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
  //! Dolna granica wartości (wyłącznie - opcjonalnie).
  const T * jsonParams_minExcl=nullptr;
  //! Górna granica wartości (wyłącznie - opcjonalnie).
  const T * jsonParams_maxExcl=nullptr;
  //! Dolna granica wartości (włącznie - opcjonalnie).
  const T * jsonParams_minIncl=nullptr;
  //! Górna granica wartości (włącznie - opcjonalnie).
  const T * jsonParams_maxIncl=nullptr;
  //! Przykład (opcjonalnie).
  const T * jsonParams_exampleIn=nullptr;
  //! Przykład wyjściowy (opcjonalnie).
  const T * jsonParams_exampleOut=nullptr;
public:
  NumberType(){
    BaseType<T>::value=0;
    BaseType<T>::json_type=json_type_number;
  }
};
template<typename T>
int NumberType<T>::parseJsonThis(std::wstring & input){
  std::wstring s;
  if (get_json_element(input,s)){
    number::getFromString(s,BaseType<T>::value);
    return(0);
  }
  if (100<input.size())  throw std::invalid_argument("Zbyt długa wartość JSON String - błąd parsowania!");
  return(1);
}
template<typename T>
int NumberType<T>::serializeJsonThis(std::wstring & output){
  std::wstring o;
  number::putToString(o,BaseType<T>::value);
  if ((o.size()+output.size())<output.max_size()){
    output+=o;
    return(0);
  }
  return(1);
}
template<typename T>
int NumberType<T>::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new NumberInfo<T>);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(BaseType<T>::json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType);
    DO_INFO(BaseType<T>,elementName)
    DO_INFO(BaseType<T>,elementInfo)
    DO_INFO(BaseType<T>,elementDesc)
    DO_INFO(BaseType<T>,jsValidate)
    DO_INFO(NumberType<T>,minExcl)
    DO_INFO(NumberType<T>,maxExcl)
    DO_INFO(NumberType<T>,minIncl)
    DO_INFO(NumberType<T>,maxIncl)
    DO_INFO(NumberType<T>,exampleIn)
    DO_INFO(NumberType<T>,exampleOut)
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
template<typename T>
int NumberType<T>::testJsonThis(){
  bool e=false;
  std::ostringstream error;
  if (jsonParams_minExcl){//Jeśli jest ograniczenie.
    if (!((*jsonParams_minExcl)<BaseType<T>::value)) {//Sprawdź ograniczenie.
      error<<"Wartość elementu JSON Number ("<<BaseType<T>::value<<") nie może być mniejsza/równa niż "<<(*jsonParams_minExcl)<<"! ";
      e=true;
    }
  }
  if (jsonParams_maxExcl){//Jeśli jest ograniczenie.
    if (!(BaseType<T>::value<(*jsonParams_maxExcl))) {//Sprawdź ograniczenie.
      error<<"Wartość elementu JSON Number ("<<BaseType<T>::value<<") nie może być większa/równa niż "<<(*jsonParams_maxExcl)<<"! ";
      e=true;
    }
  }
  if (jsonParams_minIncl){//Jeśli jest ograniczenie.
    if (!((*jsonParams_minIncl)<=BaseType<T>::value)) {//Sprawdź ograniczenie.
      error<<"Wartość elementu JSON Number ("<<BaseType<T>::value<<") nie może być mniejsza niż "<<(*jsonParams_minIncl)<<"! ";
      e=true;
    }
  }
  if (jsonParams_maxIncl){//Jeśli jest ograniczenie.
    if (!(BaseType<T>::value<=(*jsonParams_maxIncl))) {//Sprawdź ograniczenie.
      error<<"Wartość elementu JSON Number ("<<BaseType<T>::value<<") nie może być większa niż "<<(*jsonParams_maxIncl)<<"! ";
      e=true;
    }
  }
  if (e) throw std::invalid_argument(error.str());
  return(0);
}
//===========================================
class BoolType: public BaseType<bool>{
private:
  std::unique_ptr<BoolInfo> info_ptr;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
  //! Przykład wejściowy (opcjonalnie).
  const bool * jsonParams_exampleIn=nullptr;
  //! Przykład wyjściowy (opcjonalnie).
  const bool * jsonParams_exampleOut=nullptr;
public:
  BoolType(){
    BaseType<bool>::json_type=json_type_bool;
    BaseType<bool>::value=false;
  }
};
//===========================================
class NullType: public BaseType<bool>{
private:
  std::unique_ptr<NullInfo> info_ptr;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
public:
  NullType(){
    BaseType<bool>::json_type=json_type_null;
    BaseType<bool>::value=false;
  }
};
//===========================================
class StringType: public Base, public std::string{
private:
  std::unique_ptr<StringInfo> info_ptr;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
  typedef std::string value_t;
  //! Minimalna długość, którą String musi spełniać (opcjonalnie).
  const std::size_t * jsonParams_minLen=nullptr;
  //! Maksymalna długość, którą String musi spełniać (opcjonalnie).
  const std::size_t * jsonParams_maxLen=nullptr;
  //! Wyrażenie regularne, który String musi spełniać (opcjonalnie).
  const std::string * jsonParams_regex=nullptr;
  //! Przykład wejściowy (opcjonalnie).
  const std::string * jsonParams_exampleIn=nullptr;
  //! Przykład wyjściowy (opcjonalnie).
  const std::string * jsonParams_exampleOut=nullptr;
public:
  StringType(){
    json_type=json_type_string;
  }
  StringType(const std::string & s){
    assign(s);
    json_type=json_type_string;
  }
  StringType(const std::string & s,std::size_t p,std::size_t l=std::string::npos){
    assign(s,p,l);
    json_type=json_type_string;
  }
  StringType(const char * s){
    assign(s);
    json_type=json_type_string;
  }
  StringType(const char * s,std::size_t n){
    assign(s,n);
    json_type=json_type_string;
  }
  StringType(size_t n, char c){
    assign(n,c);
    json_type=json_type_string;
  }
  StringType & operator()() {return(*this);}
};
//===========================================
template<class E>
class ArrayType: public Base, public std::vector<E> {
private:
  struct done_t {
    int begin_all:1;
    int end_all:1;
    int begin_el:1;
    int el:1;
    int end_el:1;
  };
  struct {
    done_t parse;
    done_t serialize;
  } done={{0}};
  std::unique_ptr<ArrayInfo<E>> info_ptr;
  std::size_t parseIndex=0;
  std::size_t serializeIndex=0;
  std::size_t infoIndex=0;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
  //! Minimalna liczba elementów.
  std::size_t * jsonParams_minSize=nullptr;
  //! Maksymalna liczba elementów.
  std::size_t * jsonParams_maxSize=nullptr;
public:
  ArrayType(){
    json_type=json_type_array;
  }
  ArrayType(const std::vector<E> & i){
    assign(i);
    json_type=json_type_array;
  }
  explicit ArrayType(std::size_t n,const E & v=E()){
    assign(n,v);
    json_type=json_type_array;
  }
  template <class InputIterator>
  ArrayType(InputIterator f,InputIterator l){
    assign(f,l);
    json_type=json_type_array;
  }
  ArrayType & operator()() {return(*this);}
  template<typename R>
  ArrayType & operator=(const std::vector<R> & r){
    this->clear();
    for (const R & i:r) this->push_back(i);
    return(*this);
  }
};
template<typename E>
int ArrayType<E>::parseJsonThis(std::wstring & input){
  if (!done.parse.begin_all){
    remove_ws(input);
    std::vector<E>::clear();
    if (input.size()){
      if (input.front()==L'['){
        input.erase(0,1);
      } else {
        std::ostringstream error;
        error<<"Niepoprawne otwarcie JSON Array ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
        throw std::invalid_argument(error.str());
      }
    } else return(1);
    done.parse.begin_all=1;
  }
  for (;input.front()!=L']';parseIndex++){
    if (!done.parse.begin_el){
      remove_ws(input);
      done.parse.begin_el=1;
    }
    if (!done.parse.el){
      while (std::vector<E>::size()<=parseIndex) std::vector<E>::emplace_back();
      try{
        DO_JOB(std::vector<E>::back().parseJson(input))
      } catch(const std::invalid_argument& exc){
        std::ostringstream error;
        error<<"["<<parseIndex<<"] / ";
        error<<exc.what();
        throw std::invalid_argument(error.str());
      }
      done.parse.el=1;
    }
    if (!done.parse.end_el){
      remove_ws(input);
      if (input.size()){
        if ((input.front()==L',')||(input.front()==L']')){
          if (input.front()==L','){
            input.erase(0,1);
          } else break;
        } else {
          std::ostringstream error;
          error<<"Niepoprawne rozdzielenie elementów (["<<parseIndex<<"]) JSON Array ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
          throw std::invalid_argument(error.str());
        }
      } else return(1);
      done.parse.end_el=1;
    }
    if (100000<parseIndex) throw std::invalid_argument("Zbyt duża liczba elementów JSON Array - błąd parsowania!");
    done.parse.begin_el=0;
    done.parse.el=0;
    done.parse.end_el=0;
  }
  if (!done.parse.end_all){
    remove_ws(input);
    if (input.size()){
      if (input.front()==L']'){
        input.erase(0,1);
      } else {
        std::ostringstream error;
        error<<"Niepoprawne zamknięcie JSON Array ('"<<ict::utf8::get(input.substr(0,20))<<"') - błąd parsowania!";
        throw std::invalid_argument(error.str());
      }
    } else return(1);
    done.parse.end_all=1;
  }
  //========================
  done.parse.begin_all=0;
  done.parse.end_all=0;
  parseIndex=0;
  return(0);
}
template<typename E>
int ArrayType<E>::serializeJsonThis(std::wstring & output){
  if (!done.serialize.begin_all){
    if ((1+output.size())<output.max_size()){
      output+=L'[';
    } else return(1);
    done.serialize.begin_all=1;
  }
  for (;serializeIndex<std::vector<E>::size();serializeIndex++){
    if (!done.serialize.begin_el){
      if (serializeIndex) {
        if ((1+output.size())<output.max_size()){
          output+=L',';
        } else return(1);
      }
      done.serialize.begin_el=1;
    }
    if (!done.serialize.el){
      try{
        DO_JOB((*this)[serializeIndex].serializeJson(output))
      } catch(const std::invalid_argument& exc){
        std::ostringstream error;
        error<<"["<<serializeIndex<<"] / ";
        error<<exc.what();
        throw std::invalid_argument(error.str());
      }
      done.serialize.el=1;
    }
    if (!done.serialize.end_el){
      done.serialize.end_el=1;
    }
    done.serialize.begin_el=0;
    done.serialize.el=0;
    done.serialize.end_el=0;
  }
  if (!done.serialize.end_all){
    if ((1+output.size())<output.max_size()){
      output+=L']';
    } else return(1);
    done.serialize.end_all=1;
  }
  //========================
  done.serialize.begin_all=0;
  done.serialize.end_all=0;
  serializeIndex=0;
  return(0);
}
template<typename E>
int ArrayType<E>::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new ArrayInfo<E>);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType);
    DO_INFO(Base,elementName)
    DO_INFO(Base,elementInfo)
    DO_INFO(Base,elementDesc)
    DO_INFO(Base,jsValidate)
    DO_INFO(ArrayType<E>,minSize)
    DO_INFO(ArrayType<E>,maxSize)
    info_ptr->JOBJECT_SHOW(info_ptr->array);
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
template<typename E>
int ArrayType<E>::testJsonThis(){
  std::size_t idx=0;
  bool e=false;
  std::ostringstream error;
  if (jsonParams_minSize){//Jeśli jest minimalna liczba elementów.
    if ((*jsonParams_minSize)>std::vector<E>::size()) {//Sprawdź minimalną liczbę elementów.
      error<<"Nieprawidłowa liczba elementów JSON Array - powinna być większa/równa niż "<<(*jsonParams_minSize)<<"! ";
      e=true;
    }
  }
  if (jsonParams_maxSize){//Jeśli jest maksymalna liczba elementów.
    if ((*jsonParams_maxSize)<std::vector<E>::size()) {//Sprawdź maksymalną liczbę elementów.
      error<<"Nieprawidłowa liczba elementów JSON Array - powinna być mniejsza/równa niż "<<(*jsonParams_maxSize)<<"! ";
      e=true;
    }
  }
  if (e) throw std::invalid_argument(error.str());
  for (E & el : *this){
    try{
      DO_JOB(el.testJson())
    }catch(const std::invalid_argument& exc){
      std::ostringstream error;
      error<<"["<<idx<<"] / ";
      error<<exc.what();
      throw std::invalid_argument(error.str());
    }
    idx++;
  }
  return(0);
}
//===========================================
class ObjectType0 {
protected:
  typedef std::map<std::string,std::size_t> prop_map_t;
  prop_map_t prop_map;
  Base * getPropPointer(std::size_t offset);
  std::size_t getPropOffset(Base * pointer);
};
class ObjectType1: public Base, private ObjectType0 {
private:
  typedef std::set<std::size_t> prop_set_t;
  struct done_t {
    int begin_all:1;
    int end_all:1;
    int begin_el:1;
    int el_name:1;
    int between_el:1;
    int el_value:1;
    int end_el:1;
  };
  struct {
    done_t parse;
    done_t serialize;
  } done={{0}};
  std::unique_ptr<StringType> parse_name;
  std::unique_ptr<StringType> serialize_name;
  std::unique_ptr<StringType> info_name;
  std::size_t parseIndex=0;
  std::size_t serializeIndex=0;
  prop_map_t::iterator serializeIterator;
  prop_set_t prop_mandatory;
  prop_set_t prop_hidden;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
  bool jsonForceInfoInObject=false;
  void registerProp(Base * element,const std::string & name,bool mandatory=false);
public:
  ObjectType1(){
    json_type=json_type_object;
  }
  ObjectType1 & operator()() {return(*this);}
  void hideJsonProp(Base * element);
  void showJsonProp(Base * element);
  void hideAllJsonProp();
  void showAllJsonProp();
  bool isJsonPresent(Base * element);
  std::map<std::string,bool> getAllJsonProp();
};
template<class O>
class ObjectType2: public ObjectType1{
private:
  std::unique_ptr<ObjectInfo<O>> info_ptr;
  int infoJsonThis(std::wstring & output);
};
template<class O>
int ObjectType2<O>::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new ObjectInfo<O>);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType);
    DO_INFO(Base,elementName)
    DO_INFO(Base,elementInfo)
    DO_INFO(Base,elementDesc)
    DO_INFO(Base,jsValidate)
    {
      std::map<std::string,bool> m=getAllJsonProp();
      for (std::map<std::string,bool>::iterator it=m.begin();it!=m.end();++it){
        if (it->second){
          info_ptr->mandatory().push_back(it->first);
        }
      }
    }
    info_ptr->JOBJECT_SHOW(info_ptr->mandatory);
    info_ptr->JOBJECT_SHOW(info_ptr->object);
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
//===========================================
class MutableType1: public Base, private ObjectType0 {
private:
  typedef std::vector<std::size_t> prop_list_t;
  prop_list_t prop_list;
  std::size_t prop_selected=-1;
  int parseJsonThis(std::wstring & input);
  int serializeJsonThis(std::wstring & output);
  int infoJsonThis(std::wstring & output);
  int testJsonThis();
protected:
  bool jsonForceInfoInMutable=false;
  void registerProp(Base * element,const std::string & name);
  bool isJsonPresent(Base * element);
public:
  MutableType1(){
    json_type=json_type_mutable;
  }
  MutableType1 & operator()() {return(*this);}
  void selectJsonProp(Base * element);
};
template<class O>
class MutableType2: public MutableType1{
private:
  std::unique_ptr<MutableInfo<O>> info_ptr;
  int infoJsonThis(std::wstring & output);
};
template<class O>
int MutableType2<O>::infoJsonThis(std::wstring & output){
  if (!info_ptr){
    info_ptr.reset(new MutableInfo<O>);
    info_ptr->JOBJECT_HIDE_ALL();
    info_ptr->jsonType=get_type_name(json_type);
    info_ptr->JOBJECT_SHOW(info_ptr->jsonType);
    DO_INFO(Base,elementName)
    DO_INFO(Base,elementInfo)
    DO_INFO(Base,elementDesc)
    DO_INFO(Base,jsValidate)
    info_ptr->JOBJECT_SHOW(info_ptr->mut);
  }
  DO_JOB(info_ptr->serializeJson(output))
  info_ptr.reset(nullptr);
  return(0);
}
//===========================================
class BaseInfo : public ObjectType1{
public:
  StringType jsonType;
  StringType elementName;
  StringType elementInfo;
  ArrayType<StringType> elementDesc;
  StringType jsValidate;
  BaseInfo(){
    JOBJECT_PROP(jsonType)
    JOBJECT_PROP(elementName)
    JOBJECT_PROP(elementInfo)
    JOBJECT_PROP(elementDesc)
    JOBJECT_PROP(jsValidate)
  }
};
template<typename T>
class NumberInfo : public BaseInfo{
public:
  NumberType<T> minExcl;
  NumberType<T> maxExcl;
  NumberType<T> minIncl;
  NumberType<T> maxIncl;
  NumberType<T> exampleIn;
  NumberType<T> exampleOut;
  NumberInfo(){
    JOBJECT_PROP(minExcl)
    JOBJECT_PROP(maxExcl)
    JOBJECT_PROP(minIncl)
    JOBJECT_PROP(maxIncl)
    JOBJECT_PROP(exampleIn)
    JOBJECT_PROP(exampleOut)
  }
};
class BoolInfo : public BaseInfo{
public:
  BoolType exampleIn;
  BoolType exampleOut;
  BoolInfo(){
    JOBJECT_PROP(exampleIn)
    JOBJECT_PROP(exampleOut)
  }
};
class NullInfo : public BaseInfo{
public:
  NullInfo(){
  }
};
class StringInfo : public BaseInfo{
public:
  NumberType<unsigned long long> minLen;
  NumberType<unsigned long long> maxLen;
  StringType regex;
  StringType exampleIn;
  StringType exampleOut;
  StringInfo(){
    JOBJECT_PROP(minLen)
    JOBJECT_PROP(maxLen)
    JOBJECT_PROP(regex)
    JOBJECT_PROP(exampleIn)
    JOBJECT_PROP(exampleOut)
  }
};
template<class E>
class ArrayInfo : public BaseInfo{
public:
  NumberType<unsigned long long> minSize;
  NumberType<unsigned long long> maxSize;
  class _ArrayInfo: public E {
  public:
    _ArrayInfo(){
      E::jsonForceInfo=true;
    }
  } array;
  ArrayInfo(){
    JOBJECT_PROP(minSize)
    JOBJECT_PROP(maxSize)
    JOBJECT_PROP(array)
  }
};
template<class E>
class ObjectInfo : public BaseInfo{
public:
  ArrayType<StringType> mandatory;
  class _ObjectInfo: public E {
  public:
    _ObjectInfo(){
      E::jsonForceInfoInObject=true;
    }
  } object;
  ObjectInfo(){
    JOBJECT_PROP(mandatory)
    JOBJECT_PROP(object)
  }
};
template<class E>
class MutableInfo : public BaseInfo{
public:
  class _MutableInfo: public E {
  public:
    _MutableInfo(){
      E::jsonForceInfoInMutable=true;
    }
  } mut;
  MutableInfo(){
    JOBJECT_PROP(mut)
  }
};
//===========================================
#undef DO_JOB
#undef DO_INFO
//===========================================
typedef NumberType<int> int_t;
typedef NumberType<long int> l_int_t;
typedef NumberType<unsigned int> u_int_t;
typedef NumberType<long long> ll_int_t;
typedef NumberType<unsigned long long> ull_int_t;
typedef NumberType<float> float_t;
typedef NumberType<double> double_t;
typedef NumberType<long double> l_double_t;
typedef BoolType bool_t;
typedef NullType null_t;
typedef StringType string_t;
template<class E> class array_t : public ArrayType<E>{};
template<class O> class object_t : public ObjectType2<O>{};
template<class O> class mutable_t : public MutableType2<O>{};
//===========================================
} }
//===========================================
#endif