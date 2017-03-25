//! @file
//! @brief Option parser header file.
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
#ifndef _OPTIONS_HEADER
#define _OPTIONS_HEADER
//============================================
#include <sys/types.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>
//============================================
namespace ict { namespace options {
//===========================================
typedef std::size_t     option_v_none_t;           // Warość opcji bez parametru (liczba wystąpień).
// Warość opcji z parametrem
#define _OPTIONS_(type,name) typedef type option_v_##name##_t;
#include "options.h.in"
#undef _OPTIONS_
// Lista wartości dla opcji z parametrem
#define _OPTIONS_(type,name) typedef std::vector<option_v_##name##_t>  option_l_##name##_t;
#include "options.h.in"
#undef _OPTIONS_

typedef std::map<std::wstring,option_v_bool_t>    dictionary_none_t;  // Mapowanie (słownik) ignorowany.
//Mapowanie (słownik) dla opcji z parametrem
#define _OPTIONS_(type,name) typedef std::map<std::wstring,option_v_##name##_t>    dictionary_##name##_t;
#include "options.h.in"
#undef _OPTIONS_

typedef wchar_t        shortOption_t;  // Krótka nazwa opcji.
typedef std::wstring   longOption_t;   // Długa nazwa opcji.
typedef std::vector<shortOption_t> shortOptionList_t;  // Lista krótkich nazw opcji.
typedef std::vector<longOption_t>  longOptionList_t;   // Lista długich nazw opcji.

//! Obsługuje opcje (parametry z funkcji ::main(int argc, char **argv)).
class Parser{
private:
  typedef enum {
    optionLang_en=0,
    optionLang_pl
  } optionLang_t;
  typedef std::size_t optionId_t;
  typedef std::map<shortOption_t,optionId_t> shortMap_t;
  typedef std::map<longOption_t,optionId_t>  longMap_t;
  typedef enum {
    #define _OPTIONS_(type,name) optionType_v_##name=__LINE__,
    #include "options.h.in"
    #undef _OPTIONS_
    #define _OPTIONS_(type,name) optionType_l_##name=-__LINE__,
    #include "options.h.in"
    #undef _OPTIONS_
    optionType_v_none=0
  } optionType_t;
  struct optionPtr_t {
    optionType_t type=optionType_v_none;
    union  {
      option_v_none_t *        v_none=nullptr;
      #define _OPTIONS_(type,name) option_v_##name##_t *      v_##name;
      #include "options.h.in"
      #undef _OPTIONS_
      #define _OPTIONS_(type,name) option_l_##name##_t *      l_##name;
      #include "options.h.in"
      #undef _OPTIONS_
    } target;
    union  {
      const void *                   d_none=nullptr;
      #define _OPTIONS_(type,name) const dictionary_##name##_t *    d_##name;
      #include "options.h.in"
      #undef _OPTIONS_
    } dictionary;
  };
  typedef std::map<optionId_t,optionPtr_t> optionConfig_t;
  typedef std::map<std::string,optionLang_t>  langMap_string_t;
  typedef std::map<std::wstring,optionLang_t>  langMap_wstring_t;
private:
  optionId_t currentId=-1;
  optionId_t otherId=-1;
  optionLang_t lang;
  shortMap_t shortMap;
  longMap_t longMap;
  optionConfig_t optionConfig;
  void registerConfigNoValue(optionId_t id,option_v_none_t * target){
    optionConfig[id].type=optionType_v_none;
    optionConfig[id].target.v_none=target;
    optionConfig[id].dictionary.d_none=nullptr;
  }
  #define Options_registerConfig(prefix,name) \
    void registerConfig(optionId_t id,option_##prefix##_##name##_t * target=nullptr,const dictionary_##name##_t * dictionary=nullptr){ \
      optionConfig[id].type=optionType_##prefix##_##name; \
      optionConfig[id].target.prefix##_##name=target; \
      optionConfig[id].dictionary.d_##name=dictionary; \
    }
  #define _OPTIONS_(type,name) Options_registerConfig(v,name)
  #include "options.h.in"
  #undef _OPTIONS_
  #define _OPTIONS_(type,name) Options_registerConfig(l,name)
  #include "options.h.in"
  #undef _OPTIONS_
  #undef Options_registerConfig
  void registerOption (
    optionId_t id,
    const shortOptionList_t & shortOpt,
    const longOptionList_t & longOpt
  );
  void registerOption(
    optionId_t id,
    const shortOption_t & shortOpt,
    const longOptionList_t & longOpt
  ){
    shortOptionList_t s;
    s.push_back(shortOpt);
    registerOption(id,s,longOpt);
  }
  void registerOption(
    optionId_t id,
    const shortOptionList_t & shortOpt,
    const longOption_t & longOpt
  ){
    longOptionList_t l;
    l.push_back(longOpt);
    registerOption(id,shortOpt,l);
  }
  void registerOption(
    optionId_t id,
    const shortOption_t & shortOpt,
    const longOption_t & longOpt
  ){
    longOptionList_t l;
    l.push_back(longOpt);
    registerOption(id,shortOpt,l);
  }
  std::string getOptionDesc(optionId_t id);
  #define _OPTIONS_(type,name) option_v_##name##_t  parseValue_##name(std::wstring & input,const dictionary_##name##_t * dictionary);
  #include "options.h.in"
  #undef _OPTIONS_
  int parseValue(std::wstring & input);
  int parseShort(shortOption_t input);
  int parseShort(std::wstring & input);
  int parseLong(std::wstring & input);
  int parseOther(std::wstring & input);
  int parse(std::wstring & input);
  void internalError(const char * file,int line);
  std::wstring tolower(const std::wstring & input);
public:
  //! Konstruktor.
  Parser();
  //! Strumień z ewentualnymi błędami.
  std::ostringstream errors;
  //! 
  //! Parsuje wejściowe parametry funkcji main().
  //!
  //! @param argc_in Liczba parametrów.
  //! @param argv_in Tabela parametrów.
  //! @return Wartości: 0 sukces, inne - błąd.
  //! 
  int parse(int argc_in,const char **argv_in) noexcept;
  //! 
  //! Zwraca zestaw opcji (o tym samym działaniu) na podstawie jednej krótkiej opcji.
  //!
  //! @param shortOpt Krótka opcja do wyszukania.
  //! @return Zestaw opcji (pusty jeśli nie znaleziono).
  //! 
  std::string getOptionDesc(const shortOption_t & shortOpt);
  //! 
  //! Zwraca zestaw opcji (o tym samym działaniu) na podstawie jednej długiej opcji.
  //!
  //! @param longOpt Dłyga opcja do wyszukania.
  //! @return Zestaw opcji (pusty jeśli nie znaleziono).
  //! 
  std::string getOptionDesc(const longOption_t & longOpt);
  template<typename shortOption,typename longOption> 
    void registerOptNoValue(
      const shortOption & shortOpt,
      const longOption & longOpt,
      option_v_none_t & target
    ){
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfigNoValue(id,&target);
    }
  template<typename shortOption> 
    void registerShortOptNoValue(
      const shortOption & shortOpt,
      option_v_none_t & target
    ){
      const static longOptionList_t longOpt;
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfigNoValue(id,&target);
    }
  template<typename longOption> 
    void registerLongOptNoValue(
      const longOption & longOpt,
      option_v_none_t & target
    ){
      const static shortOptionList_t shortOpt;
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfigNoValue(id,&target);
    }
  template<typename shortOption,typename longOption,typename targetType> 
    void registerOpt(
      const shortOption & shortOpt,
      const longOption & longOpt,
      targetType & target
    ){
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfig(id,&target);
    }
  template<typename shortOption,typename targetType> 
    void registerShortOpt(
      const shortOption & shortOpt,
      targetType & target
    ){
      const static longOptionList_t longOpt;
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfig(id,&target);
    }
  template<typename longOption,typename targetType> 
    void registerLongOpt(
      const longOption & longOpt,
      targetType & target
    ){
      const static shortOptionList_t shortOpt;
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfig(id,&target);
    }
  template<typename shortOption,typename longOption,typename targetType> 
    void registerOpt(
      const shortOption & shortOpt,
      const longOption & longOpt,
      targetType & target,
      const std::map<std::wstring,targetType> & dictionary
    ){
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfig(id,&target,&dictionary);
    }
  template<typename shortOption,typename targetType> 
    void registerShortOpt(
      const shortOption & shortOpt,
      targetType & target,
      const std::map<std::wstring,targetType> & dictionary
    ){
      const static longOptionList_t longOpt;
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfig(id,&target,&dictionary);
    }
  template<typename longOption,typename targetType> 
    void registerLongOpt(
      const longOption & longOpt,
      targetType & target,
      const std::map<std::wstring,targetType> & dictionary
    ){
      const static shortOptionList_t shortOpt;
      optionId_t id=optionConfig.size();
      registerOption(id,shortOpt,longOpt);
      registerConfig(id,&target,&dictionary);
    }
  template<typename targetType> 
    void registerOther(
      targetType & target
    ){
      if (otherId!=-1) std::invalid_argument("target for other values given more then once");
      otherId=optionConfig.size();
      registerConfig(otherId,&target);
    }
};
//============================================
} }
//============================================
#endif