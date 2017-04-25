//! @file
//! @brief Logger module - Header file.
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
#ifndef _LOGGER_HEADER
#define _LOGGER_HEADER
//============================================
#include "time.hpp"
#include "global.hpp"
//============================================
//!Makro ładujące informacje o miejscu w kodzie (char).
#define __LOGGER__ ict::logger::getFileName(__FILE__)<<":"<<__LINE__<<" ("<<__PRETTY_FUNCTION__<<") "
//!Makro ładujące informacje o miejscu w kodzie (wchar).
#define __WLOGGER__ ict::logger::getFileName(__WFILE__)<<L":"<<__LINE__<<L" ("<<ict::logger::pretty_function(__PRETTY_FUNCTION__)<<L") "

//!Makro ustawiające strumień wyjściowy.
#define LOGGER_SET(stream,...) ict::logger::output::set(&stream,##__VA_ARGS__)
//!Makro sprawdzające ustawienia strumienia wyjściowego.
#define LOGGER_TEST(stream) ict::logger::output::test(&stream) 

//!Makro dodające kolejną warstwę na stosie logerów w danym wątku. Średnik konieczny na końcu.
#define LOGGER_LAYER ict::logger::input::Layer _ict_logger_layer_
//!Makro dodające kolejną warstwę na stosie logerów w danym wątku (z parametrami). Średnik konieczny na końcu.
#define LOGGER_L(...) ict::logger::input::Layer _ict_logger_layer_(__VA_ARGS__)
//!Makro restartujące loggera (cały stos jest kasowany).
#define LOGGER_RESTART ict::logger::restart()
//!Makro ustawiające wartości domyślne dla warstw logowania.
#define LOGGER_DEFAULT(...) ict::logger::input::setDefault(__VA_ARGS__)
//!Makro ustawiajace katalog bazowy do ścieżek plików.
#define LOGGER_BASEDIR ict::logger::setBaseDir(__FILE__)


//!Strumień wejściowy (char) dla poziomu CRITICAL w najwyższej warstwie logowania dla danego wątku.
#define LOGGER_CRIT ict::logger::input::ostream(ict::logger::critical)
//!Strumień wejściowy (char) dla poziomu ERROR w najwyższej warstwie logowania dla danego wątku.
#define LOGGER_ERR ict::logger::input::ostream(ict::logger::error)
//!Strumień wejściowy (char) dla poziomu WARNING_ w najwyższej warstwie logowania dla danego wątku.
#define LOGGER_WARN ict::logger::input::ostream(ict::logger::warning)
//!Strumień wejściowy (char) dla poziomu NOTICE_ w najwyższej warstwie logowania dla danego wątku.
#define LOGGER_NOTICE ict::logger::input::ostream(ict::logger::notice)
//!Strumień wejściowy (char) dla poziomu INFO w najwyższej warstwie logowania dla danego wątku.
#define LOGGER_INFO ict::logger::input::ostream(ict::logger::info)
//!Strumień wejściowy (char) dla poziomu DEBUG w najwyższej warstwie logowania dla danego wątku.
#define LOGGER_DEBUG ict::logger::input::ostream(ict::logger::debug)

/*
#include <iostream>
#define LOGGER_CRIT std::cout
#define LOGGER_ERR std::cout
#define LOGGER_WARN std::cout
#define LOGGER_NOTICE std::cout
#define LOGGER_INFO std::cout
#define LOGGER_DEBUG std::cout
*/

//!Strumień wejściowy (wchar) dla poziomu CRITICAL w najwyższej warstwie logowania dla danego wątku.
#define WLOGGER_CRIT ict::logger::input::wostream(ict::logger::critical)
//!Strumień wejściowy (wchar) dla poziomu ERROR w najwyższej warstwie logowania dla danego wątku.
#define WLOGGER_ERR ict::logger::input::wostream(ict::logger::error)
//!Strumień wejściowy (wchar) dla poziomu WARNING_ w najwyższej warstwie logowania dla danego wątku.
#define WLOGGER_WARN ict::logger::input::wostream(ict::logger::warning)
//!Strumień wejściowy (wchar) dla poziomu NOTICE_ w najwyższej warstwie logowania dla danego wątku.
#define WLOGGER_NOTICE ict::logger::input::wostream(ict::logger::notice)
//!Strumień wejściowy (wchar) dla poziomu INFO w najwyższej warstwie logowania dla danego wątku.
#define WLOGGER_INFO ict::logger::input::wostream(ict::logger::info)
//!Strumień wejściowy (wchar) dla poziomu DEBUG w najwyższej warstwie logowania dla danego wątku.
#define WLOGGER_DEBUG ict::logger::input::wostream(ict::logger::debug)
//============================================
namespace ict { namespace logger {
//===========================================
  //! Typ zawierający wskazanie poziomów logowania loggera.
  typedef uint8_t flags_t;

  //! Stałe wskazujące poziomy logowania loggera (mogą być łączone sumą bitową).
  extern const flags_t critical;
  extern const flags_t error;
  extern const flags_t warning;
  extern const flags_t notice;
  extern const flags_t info;
  extern const flags_t debug;
  extern const flags_t errors;
  extern const flags_t warnings;
  extern const flags_t notices;
  extern const flags_t infos;
  extern const flags_t all;
  extern const flags_t nocritical;
  extern const flags_t noerrors;
  extern const flags_t nowarnings;
  extern const flags_t nonotices;
  extern const flags_t none;
  extern const flags_t nodebug;
  extern const flags_t defaultValue;

  //! Elementy pozwalające na podłączenie i manipulację wyjścia logowania.
  namespace output {
    //!
    //! @brief Ustawia strumień wyjściowy dla logera.
    //!
    //! @param ostream Wskaźnik na strumień wyjściowy. 
    //! @param filter Filtr logów - zapisywane są tylko te, które mieszczą się w podanej masce. 
    //!  Jeśli podana zostanie wartość 0x0, to strumień zostanie usunięty.
    //!
    void set(std::ostream * ostream,flags_t filter=all);
    //!
    //! @brief Ustawia strumień wyjściowy dla logera.
    //!
    //! @param ostream Wskaźnik na strumień wyjściowy. 
    //! @param filter Filtr logów - zapisywane są tylko te, które mieszczą się w podanej masce. 
    //!  Jeśli podana zostanie wartość 0x0, to strumień zostanie usunięty.
    //!
    void set(std::wostream * wostream,flags_t filter=all);
    //!
    //! @brief Ustawia wyjście do syslog dla logera.
    //!
    //! @param ident String identyfikujący wpisy tej aplikacji w syslog.
    //! @param filter Filtr logów - zapisywane są tylko te, które mieszczą się w podanej masce. 
    //!  Jeśli podana zostanie wartość 0x0, to syslog zostanie usunięty.
    //!
    void set(std::string * ident,flags_t filter=all);
    //!
    //! @brief Sprawdza, czy podany wskaźnik strumienia jest już ustawiony.
    //!
    //! @param ostream Wskaźnik na strumień wyjściowy. 
    //! @return Ustawienia filtra dla podanego strumienia. Jeśli 0x0, to strumień nie jest ustawiony.
    //!
    flags_t test(std::ostream * ostream);
    //!
    //! @brief Sprawdza, czy podany wskaźnik strumienia jest już ustawiony.
    //!
    //! @param ostream Wskaźnik na strumień wyjściowy. 
    //! @return Ustawienia filtra dla podanego strumienia. Jeśli 0x0, to strumień nie jest ustawiony.
    //!
    flags_t test(std::wostream * wostream);
    //!
    //! @brief Sprawdza, czy syslog jest już ustawiony.
    //!
    //! @return Ustawienia filtra dla syslog. Jeśli 0x0, to syslog nie jest ustawiony.
    //!
    flags_t test();
  }

  //! Elementy pozwalające na podłączenie się i manipulację do wejścia logowania.
  namespace input {
    //!
    //! @brief Ustawia domyślne parametry (dla ict::logger::defaultValue).
    //! 
    //! @param [in] direct_in Poziomy logowania bez buforowania na danej warstwie.
    //! @param [in] buffered_in Poziomy logowania z buforowaniem na danej warstwie.
    //! @param [in] dump_in Poziomy logowania, które powodują opróżnienie bufora na danej warstwie.
    //!
    void setDefault(
      ict::logger::flags_t direct_in=ict::logger::notices,
      ict::logger::flags_t buffered_in=ict::logger::nonotices,
      ict::logger::flags_t dump_in=ict::logger::errors
    );
    //! Obiekt tworzący warstwę logowania. Musi być utworzony co najmniej jeden w danym wątku, by logowanie było możliwe.
    class Layer {
    public:
      //!
      //! @brief Konstruktor.
      //! 
      //! @param [in] direct_in Poziomy logowania bez buforowania na tej warstwie.
      //! @param [in] buffered_in Poziomy logowania z buforowaniem na tej warstwie.
      //! @param [in] dump_in Poziomy logowania, które powodują opróżnienie bufora na tej warstwie.
      //!
      Layer(
        ict::logger::flags_t direct_in=ict::logger::defaultValue,
        ict::logger::flags_t buffered_in=ict::logger::defaultValue,
        ict::logger::flags_t dump_in=ict::logger::defaultValue
      );
      //!
      //! @brief Destruktor.
      //! 
      ~Layer();
    };
    //!
    //! @brief Podaje referencję do strumienia wyjścia (char) logowania dla zadanego poziomu logowania w najwyższej warstwie logowania w danym wątku.
    //!
    //! @param [in] severity Wskazanie poziomu logowania.
    //! @return Referencja do strumienia wyjścia (char) logowania.
    //!
    std::ostream & ostream(flags_t severity);
    //!
    //! @brief Podaje referencję do strumienia wyjścia (wchar) logowania dla zadanego poziomu logowania w najwyższej warstwie logowania w danym wątku.
    //!
    //! @param [in] severity Wskazanie poziomu logowania.
    //! @return Referencja do strumienia wyjścia (wchar) logowania.
    //!
    std::wostream & wostream(flags_t severity);
  }
  //!
  //! @brief Restartuje loggera (cały stos jest kasowany).
  //!
  void restart();
  //!
  //! @brief Udostępnia bazowy katalog do ścieżek plików w logerze.
  //!
  inline std::string & getBaseDir();
  //!
  //! @brief Ustawia bazowy katalog do ścieżek plików w logerze.
  //!
  inline void setBaseDir(const std::string & file);
  //!
  //! @brief Zwraca nazwę pliku relatywną do bazowego katalogu.
  //!
  inline std::string getFileName(const std::string & file);
  //!
  //! @brief Zwraca nazwę pliku relatywną do bazowego katalogu.
  //!
  inline std::wstring getFileName(const std::wstring & file);
  //!
  //! @brief Konwersja __PRETTY_FUNCTION__
  //!
  std::wstring pretty_function(const std::string & input);
//===========================================
} }
//===========================================
#endif