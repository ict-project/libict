//! @file
//! @brief Logger module - Source file.
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
#include "logger.hpp"
#include <sstream>
#include <string>
#include <streambuf>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <mutex>
#include <thread>
#include <iostream>
#include "syslog.h"
#include "utf8.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
#define TRY_BEGIN try {
#define TRY_END } catch (...) { \
  std::cerr<<__LOGGER__<<"Logger exception!!!"<<std::endl; \
}
//============================================
namespace ict { namespace logger {
//===========================================
  const flags_t critical(0x1<<0);
  const flags_t error(0x1<<1);
  const flags_t warning(0x1<<2);
  const flags_t notice(0x1<<3);
  const flags_t info(0x1<<4);
  const flags_t debug(0x1<<5);
  const flags_t errors    (critical|error);
  const flags_t warnings  (critical|error|warning);
  const flags_t notices   (critical|error|warning|notice);
  const flags_t infos     (critical|error|warning|notice|info);
  const flags_t all       (critical|error|warning|notice|info|debug);
  const flags_t nocritical         (error|warning|notice|info|debug);
  const flags_t noerrors                 (warning|notice|info|debug);
  const flags_t nowarnings                       (notice|info|debug);
  const flags_t nonotices                               (info|debug);
  const flags_t none                                           (0x0);
  const flags_t nodebug(infos);
  const flags_t defaultValue(0x1<<7);
  //==========================================================================
  void get_log_severity(flags_t severity,std::ostringstream & out){
    static const std::map<flags_t,std::string> severity_map({
      {critical,"CRITICAL"},
      {error,"ERROR"},
      {warning,"WARNING"},
      {notice,"NOTICE"},
      {info,"INFO"},
      {debug,"DEBUG"}
    });
    if (severity_map.count(severity)) out<<severity_map.at(severity);
  }
  void get_log_severity(flags_t severity,std::wostringstream & out){
    static const std::map<flags_t,std::wstring> severity_map({
      {critical,L"CRITICAL"},
      {error,L"ERROR"},
      {warning,L"WARNING"},
      {notice,L"NOTICE"},
      {info,L"INFO"},
      {debug,L"DEBUG"}
    });
    if (severity_map.count(severity)) out<<severity_map.at(severity);
  }
  //==========================================================================
  class Syslog{
  private:
    flags_t filter;
  public:
    Syslog(const std::string & ident,flags_t filter_in):filter(filter_in){
      ::openlog(ident.c_str(),LOG_PID,LOG_USER);
    }
    ~Syslog(){
      ::closelog();
    }
    void log(flags_t severity,const std::string & str){
      static const std::map<flags_t,int> severity_map({
        {critical,LOG_CRIT},//critical conditions
        {error,LOG_ERR},//error conditions
        {warning,LOG_WARNING},//warning conditions
        {notice,LOG_NOTICE},//normal, but significant, condition
        {info,LOG_INFO},// informational message
        {debug,LOG_DEBUG}// debug-level message
      });
      if ((filter&severity)&&(severity_map.count(severity))){
        ::syslog(severity_map.at(severity),"%s",str.c_str());
      }
    }
    flags_t getFilter(){return(filter);}
  };
  //==========================================================================
  template <typename charT>
  struct log_line_t {
    bool buffered=false;
    ict::time::TimeInfo time;
    flags_t severity;
    std::basic_string<charT> line;
  };
  typedef log_line_t<char> log_string_t;
  typedef log_line_t<wchar_t> log_wstring_t;
  //==========================================================================
  namespace output {
    typedef std::map<std::ostream *,flags_t> ostream_map_t;
    typedef std::map<std::wostream *,flags_t> wostream_map_t;
    //! Mutex dla strumieni wyjściowych.
    static std::mutex mutex;
    //! Zestaw strumieni wyjściowych ostream.
    static ostream_map_t ostream_map;
    //! Zestaw strumieni wyjściowych wostream.
    static wostream_map_t wostream_map;
    //! Wskaźnik na obieg obsługujący syslog.
    static std::unique_ptr<Syslog> syslog;

    template <typename S> 
    void set(S * ostream,flags_t filter,std::map<S *,flags_t> & map){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      if (filter&&ostream){
        map[ostream]=filter;
      } else if (map.count(ostream)) {
        map.erase(ostream);
      }
      TRY_END
    }
    void set(std::ostream * ostream,flags_t filter){
      set(ostream,filter,ostream_map);
    }
    void set(std::wostream * wostream,flags_t filter){
      set(wostream,filter,wostream_map);
    }
    void set(std::string * ident,flags_t filter){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      if (filter&&ident){
        syslog.reset(new Syslog(*ident,filter));
      } else if (syslog.get()) {
        syslog.reset(nullptr);
      }
      TRY_END
    }

    template <typename S> 
    flags_t test(S * ostream,std::map<S *,flags_t> & map){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      if (map.count(ostream)){
        return(map.at(ostream));
      }
      TRY_END
      return(0x0);
    }
    flags_t test(std::ostream * ostream){
      return(test(ostream,ostream_map));
    }
    flags_t test(std::wostream * wostream){
      return(test(wostream,wostream_map));
    }
    flags_t test(){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      if (syslog.get()) {
        return(syslog->getFilter());
      }
      TRY_END
      return(0x0);
    }
    //Zapisuje pojedynczy log w syslog.
    static inline void log_syslog_out(flags_t severity,const std::string & in){
      TRY_BEGIN
      if (syslog.get()) syslog->log(severity,in);
      TRY_END
    }
    //Zapisuje pojedynczy log w syslog.
    static inline void log_syslog_out(flags_t severity,const std::wstring & in){
      TRY_BEGIN
      std::wstring l_in(in);
      std::string l_out;
      ict:utf8::transfer(l_in,l_out);
      log_syslog_out(severity,l_out);
      TRY_END
    }
    //Zapisuje pojedynczy log w syslog.
    template <typename charT>
    static inline void log_syslog_out(const log_line_t<charT> & in){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      if (syslog.get()){//Jeśli syslog jest ustawiony
        if ((in.severity)&(syslog->getFilter())){//Jeśli został ustawiony i poziom logu się zgadza.
          std::basic_ostringstream<charT> out;
          //Jeśli jest to wpis buforowany, to go oznacz i wstaw czas powstania tego logu.
          if (in.buffered) out<<out.widen('|')<<out.widen(' ')<<in.time<<out.widen(' ');
          //Wstaw znacznik severity do strumienia.
          get_log_severity(in.severity,out);
          //Wstaw spację do strumienia.
          out<<out.widen(' ')<<in.line;
          //Wstaw do syslog.
          log_syslog_out(in.severity,out.str());
        }
      }
      TRY_END
    }

    //Zapisuje pojedynczy log w strumieniach wyjściowych w jednym strumieniu.
    template <typename charT> 
    static inline void log_stream_out(const std::basic_string<charT>& in, std::basic_ostream<charT> & out){
      TRY_BEGIN
      out<<in;
      TRY_END
    }
    //Zapisuje pojedynczy log w strumieniach wyjściowych w jednym strumieniu.
    template <typename charIn,typename charOut> 
    static inline void log_stream_out(const std::basic_string<charIn>& in, std::basic_ostream<charOut> & out){
      TRY_BEGIN
      std::basic_string<charIn> l_in(in);
      std::basic_string<charOut> l_out;
      ict::utf8::transfer(l_in,l_out);
      out<<l_out;
      TRY_END
    }
    //Zapisuje pojedynczy log w strumieniach wyjściowych jednego typu.
    template <typename charIn,typename charOut> 
    static inline void log_stream_out(flags_t severity,const std::basic_string<charIn>& in, std::map<std::basic_ostream<charOut>*,flags_t> & out){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      for (typename std::map<std::basic_ostream<charOut>*,flags_t>::iterator it=out.begin();it!=out.end();++it){//Przejdź po liście strumieni.
        if (severity&(it->second))//Jeśli filtr przepuszcza ten wpis
          if (it->first){//Jeśli to nie jest wpis dotyczący logera systemowego.
            log_stream_out(in,*(it->first));//Zapisz do strumienia.
            it->first->flush();
          }
      }
      TRY_END
    }
    //Zapisuje pojedynczy log w strumieniach wyjściowych.
    template <typename charT> 
    static void log_stream_out(const log_line_t<charT> & in){
      TRY_BEGIN
      std::basic_ostringstream<charT> out;
      // Wstaw czas.
      out<<in.time;
      //Wstaw spację do strumienia.
      out<<out.widen(' ');
      //Jeśli jest to wpis buforowany, to go oznacz.
      if (in.buffered) out<<out.widen('|')<<out.widen(' ');
      //Wstaw znacznik severity do strumienia.
      get_log_severity(in.severity,out);
      //Wstaw spację do strumienia.
      out<<out.widen(' ');
      //Wstaw linię.
      out<<in.line<<std::endl;
      //Zapisz do wszystkich strumieni wyjściowych ostream.
      log_stream_out(in.severity,out.str(),ostream_map);
      //Zapisz do wszystkich strumieni wyjściowych wostream.
      log_stream_out(in.severity,out.str(),wostream_map);
      TRY_END
    }
  }
  //==========================================================================
  //! Klasa obsługująca pusty bufor.
  template <
    typename charT=char,
    typename traits=std::char_traits<charT>
  > 
  class BlackHole:public std::basic_streambuf<charT,traits> {
  public:
    typedef typename traits::int_type int_type_t;
  public:
    //!
    //! @brief Konstruktor.
    //! 
    BlackHole(){}
    //!
    //! @brief Destruktor.
    //! 
    ~BlackHole(){}
    //!
    //! @brief Przetwarza jeden znak.
    //! 
    //! @param [in] c Przetwarzany znak.
    //! @return Przetworzony znak.
    //!
    int_type_t overflow (int_type_t c){
      //Zakończ.
      return(c);
    }
  };
  //==========================================================================
  //! Klasa obsługująca bufor logowania na wybranym poziomie.
  template <
    typename charT=char,
    typename traits=std::char_traits<charT>
  > 
  class Buffer:public std::basic_streambuf<charT,traits> {
  public:
    typedef std::basic_string<charT> basic_string_t;
    typedef std::basic_ostream<charT,traits> basic_ostream_t;
    typedef typename traits::int_type int_type_t;
    typedef std::vector<log_line_t<charT>> log_line_vector_t;
  private:
    //! Pojedyncza linia loga.
    log_line_t<charT> log_line;
    //! Bufor linii loga.
    log_line_vector_t * log_buffer;
    //! Informacja o tym, że ostatnio została złamana linia (rozpoczyna się nowy wpis loga).
    bool newline=true;
  public:
    //!
    //! @brief Konstruktor.
    //! 
    //! @param [in] severity_in Poziom logowania.
    //! @param [in] buffered_in Informacja, czy poziom jest buforowany.
    //! @param [in] buffered_in Informacja, czy poziom jest buforowany.
    //!
    Buffer(ict::logger::flags_t severity_in,bool buffered_in,log_line_vector_t * log_buffer_in):log_buffer(log_buffer_in){
      TRY_BEGIN
      log_line.buffered=buffered_in;
      log_line.severity=severity_in;
      TRY_END
    }
    //!
    //! @brief Destruktor.
    //! 
    ~Buffer(){}
    //!
    //! @brief Filtruje znaki char.
    //! 
    static char charFilter(char c,Buffer<char>*p){
      static const std::map<char,char> char_map({
        {'\t',' '},//Zamiana na spacje.
        {'\v',' '},
        {'\0',' '},
        {'\n',0},//Zero oznacza koniec linii.
        {'\r',0}
      });
      if (char_map.count(c)) return(char_map.at(c));
      return(c);
    }
    //!
    //! @brief Filtruje znaki wchar.
    //! 
    static wchar_t charFilter(wchar_t c,Buffer<wchar_t>*p){
      static const std::map<wchar_t,wchar_t> char_map({
        {L'\t',L' '},//Zamiana na spacje.
        {L'\v',L' '},
        {L'\0',L' '},
        {L'\n',0},//Zero oznacza koniec linii.
        {L'\r',0}
      });
      if (char_map.count(c)) return(char_map.at(c));
      return(c);
    }
    //!
    //! @brief Przetwarza jeden znak.
    //! 
    //! @param [in] c Przetwarzany znak.
    //! @return Przetworzony znak.
    //!
    int_type_t overflow (int_type_t c){
      TRY_BEGIN
      //! Przetwarzany znak.
      int_type_t znak(charFilter(c,this));
      if (znak){//Nie było znaku nowej linii
        if (newline) {
          //Jeśli rozpoczyna się nowy wpis, to pobierz aktualny czas.
          log_line.time.setLocalTime();
          //Oznacz, że linia się rozpoczyna.
          newline=false;
          //Wyczyść linię.
          log_line.line.clear();
        }
        //Wstaw przetwarzany znak.
        log_line.line+=znak;
      } else {//Pojawił się znak nowej linii
        //Jeśli znaki nowej linii.
        if (newline) {
          //Ignoruj powtarzające się jeden po drugim.
          return(c);
        }
        //Oznacz nową linię.
        newline=true;
        if (log_line.buffered){//Jeśli zapis jest buforowany.
          const static std::size_t max(100);//Maksymalny rozmiar bufora.
          if (log_buffer) if (log_buffer->size()<max){//Jeśli mniejszy, niż maksymalny rozmiar.
            log_buffer->push_back(log_line);//Dodaj do bufora.
          } else if (log_buffer->size()==max) {
            log_line_t<char> log_warn;
            log_warn.buffered=true;
            log_warn.line=__FILE__;
            log_warn.line+=" (";
            log_warn.line+=__PRETTY_FUNCTION__;
            log_warn.line+=") ";
            log_warn.line+="Bufor loggera osiągnął maksymalny rozmiar!";
            log_warn.severity=warning;
            output::log_stream_out(log_warn);//Zapisz w strumieniach wyjściowych.
            output::log_syslog_out(log_warn);//Zapisz w syslog.
            log_buffer->push_back(log_line);//Dodaj do bufora.
          }
        } else {//Jeśli zapis nie jest buforowany.
          output::log_stream_out(log_line);//Zapisz w strumieniach wyjściowych.
          output::log_syslog_out(log_line);//Zapisz w syslog.
        }
      }
      TRY_END
      //Zakończ.
      return(c);
    }
  };
  //==========================================================================
  //! Klasa obsługująca logowanie na wszystkich poziomach (pojedynczy logger).
  template <
    typename charT=char,
    typename traits=std::char_traits<charT>
  > 
  class Single:
    public std::basic_ostringstream<charT,traits> 
  {
  public:
    typedef logger::Buffer<charT,traits> logger_buffer_t;
    typedef std::basic_ostream<charT,traits> basic_ostream_t;
    typedef std::vector<log_line_t<charT>> log_line_vector_t;
  private:
    //! Loger dla pojedynczego poziomu logowania.
    class StreamPack{
    public:
      logger_buffer_t buffer;
      basic_ostream_t stream;
      StreamPack(ict::logger::flags_t severity,bool buffered,log_line_vector_t * log_buffer):
        buffer(severity,buffered,log_buffer),stream(&buffer)
      {}
    };
    typedef std::map<ict::logger::flags_t,std::unique_ptr<StreamPack>> stream_map_t;
    //! Mapa logerów dla różnych poziomów.
    stream_map_t logger_map;
    //! Poziomy logowania bez buforowania na tej warstwie.
    ict::logger::flags_t direct;
    //! Poziomy logowania, które powodują opróżnienie bufora na tej warstwie.
    ict::logger::flags_t dump;
    //! Poziomy logowania, które są aktywne na tej warstwie.
    ict::logger::flags_t active;
    //! Poziomy logowania, które zostały wykonane na tej warstwie.
    ict::logger::flags_t done;
    
  public:
    //!
    //! @brief Konstruktor.
    //! 
    //! @param [in] direct_in Poziomy logowania bez buforowania na tej warstwie.
    //! @param [in] buffered_in Poziomy logowania z buforowaniem na tej warstwie.
    //! @param [in] dump_in Poziomy logowania, które powodują opróżnienie bufora na tej warstwie.
    //!
    Single(
      ict::logger::flags_t direct_in,
      ict::logger::flags_t buffered_in,
      ict::logger::flags_t dump_in
    ):direct(direct_in),dump(dump_in),active(direct_in|buffered_in),done(0){}
    ~Single(){
      TRY_BEGIN
      if (dump&done)//Jeśli pojawił się poziom, który wyzwala zrzut z buforów logujących
        doDump();//Zrób zrzut.
      TRY_END
    };
    //! Bufor linii loga.
    log_line_vector_t log_buffer;
    //!
    //! @brief Zrzuca cały bufor linii loga.
    //! 
    void doDump(){
      TRY_BEGIN
      for (log_line_t<charT> & l: log_buffer){//Cały bufor.
        output::log_stream_out(l);//Zapisz w strumieniach wyjściowych.
        output::log_syslog_out(l);//Zapisz w syslog.
      }
      log_buffer.clear();//Wyczyść bufor.
      TRY_END
    }
    basic_ostream_t & getLogger(flags_t severity){
      static BlackHole<charT> blackHoleBuff;
      static basic_ostream_t blackHole(&blackHoleBuff);
      TRY_BEGIN
      const static std::set<ict::logger::flags_t> severity_set({
        critical,error,warning,
        notice,info,debug
      });
      done|=severity;//Zaznacz, że był taki.
      if ((severity_set.count(severity))&&(active&severity)){//Jeśli poziom logowania jest prawidłowy i aktywny na tej warstwie.
        if (!logger_map.count(severity)){//Jeśli loger na takim poziomie nie istnieje
          logger_map[severity].reset(new StreamPack(severity,!(severity&direct),&log_buffer));//Stwórz logera.
        }
        return(logger_map[severity]->stream);//Zwróć go.
      }
      TRY_END
      return(blackHole);
    }
  };
  typedef Single<char> single_char_t;
  typedef Single<wchar_t> single_wchar_t;
  //==========================================================================
  //! Klasa obsługująca stos logerów.
  template <
    typename charT=char,
    typename traits=std::char_traits<charT>
  > 
  class Stack
  {
  public:
    typedef Single<charT,traits> single_t;
    typedef std::stack<std::unique_ptr<single_t>> stack_t;
  private:
    //! Muteks stosu logerów,
    std::mutex stack_mutex;
    //! Stos logerów,
    stack_t stack;
  public:
    //!
    //! @brief Podaje referencję do najwyższego logera.
    //! 
    single_t & operator ()(){
      std::lock_guard<std::mutex> lock(stack_mutex);
      return(*(stack.top()));
    }
    //!
    //! @brief Dokłada nowego logger na stos.
    //! 
    //! @param [in] direct_in Poziomy logowania bez buforowania na tej warstwie.
    //! @param [in] buffered_in Poziomy logowania z buforowaniem na tej warstwie.
    //! @param [in] dump_in Poziomy logowania, które powodują opróżnienie bufora na tej warstwie.
    //! @return Liczba logerów na stosie.
    //!
    std::size_t push(
      ict::logger::flags_t direct_in,
      ict::logger::flags_t buffered_in,
      ict::logger::flags_t dump_in
    ){
      std::lock_guard<std::mutex> lock(stack_mutex);
      stack.emplace(new single_t(direct_in,buffered_in,dump_in));
      return(stack.size());
    }
    //!
    //! @brief Zdejmuje loggera ze stosu, jeśli nie jest to ostatni loger.
    //! 
    //! @return Liczba logerów na stosie.
    //!
    std::size_t pop(){
      std::lock_guard<std::mutex> lock(stack_mutex);
      if (stack.size()>0){
        stack.pop();
      }
      return(stack.size());
    }
    //!
    //! @brief Zwraca liczbę logerów na stosie.
    //! 
    //! @return Liczba logerów na stosie.
    //!
    std::size_t size(){
      std::lock_guard<std::mutex> lock(stack_mutex);
      return(stack.size());
    }
  };
  //==========================================================================
  //! Mapa stosów logerów dla wątków (char).
  typedef Stack<char> stack_char_t;
  typedef std::map<std::thread::id,stack_char_t> stack_map_char_t;
  static stack_map_char_t & get_map_char(){
    static stack_map_char_t map_char;
    return(map_char);
  }
  //! Mapa stosów logerów dla wątków (wchar).
  typedef Stack<wchar_t> stack_wchar_t;
  typedef std::map<std::thread::id,stack_wchar_t> stack_map_wchar_t;
  static stack_map_wchar_t & get_map_wchar(){
    static stack_map_wchar_t map_wchar;
    return(map_wchar);
  }
  //==========================================================================
  namespace input {
    //! Mutex dla strumieni wejściowych.
    static std::mutex mutex;
    //! Wartość domyślna dla poziomów logowania bez buforowania na danej warstwie.
    static ict::logger::flags_t directDefault(ict::logger::notices);
    //! Wartość domyślna dla poziomów logowania z buforowaniem na danej warstwie.
    static ict::logger::flags_t bufferedDefault(ict::logger::nonotices);
    //! Wartość domyślna dla poziomów logowania, które powodują opróżnienie bufora na danej warstwie.
    static ict::logger::flags_t dumpDefault(ict::logger::errors);
    void setDefault(
      ict::logger::flags_t direct_in,
      ict::logger::flags_t buffered_in,
      ict::logger::flags_t dump_in
    ){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      directDefault=direct_in;
      bufferedDefault=buffered_in;
      dumpDefault=dump_in;
      TRY_END
    }
    Layer::Layer(
      ict::logger::flags_t direct_in,
      ict::logger::flags_t buffered_in,
      ict::logger::flags_t dump_in
    ){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      std::thread::id id(std::this_thread::get_id());//Ustal ID wątku.
      if (ict::logger::defaultValue&direct_in) direct_in=directDefault;//Jeśli wartość domyślna.
      if (ict::logger::defaultValue&buffered_in) buffered_in=bufferedDefault;//Jeśli wartość domyślna.
      if (ict::logger::defaultValue&dump_in) dump_in=dumpDefault;//Jeśli wartość domyślna.
      get_map_char()[id].push(direct_in,buffered_in,dump_in);//Dodaj stos logerów char dla tej warstwy.
      get_map_wchar()[id].push(direct_in,buffered_in,dump_in);//Dodaj stos logerów wchar dla tej warstwy.
      TRY_END
    }
    Layer::~Layer(){
      TRY_BEGIN
      std::lock_guard<std::mutex> lock(mutex);
      std::thread::id id(std::this_thread::get_id());//Ustal ID wątku.
      //Jeśli istnieje stos dla wątku.
      if (get_map_char().count(id)){
        //Zdejmij loggera ze stosu. Jeśli to był ostatni logger na stosie, to wykasuj stos.
        if (!get_map_char()[id].pop()){
          get_map_char().erase(id);
        }
      }
      //Jeśli istnieje stos dla wątku.
      if (get_map_wchar().count(id)){
        //Zdejmij loggera ze stosu. Jeśli to był ostatni logger na stosie, to wykasuj stos.
        if (!get_map_wchar()[id].pop()){
          get_map_wchar().erase(id);
        }
      }
      TRY_END
    }
    std::ostream & ostream(flags_t severity){
      std::lock_guard<std::mutex> lock(mutex);
      static BlackHole<char> blackHoleBuff;
      static std::basic_ostream<char> blackHole(&blackHoleBuff);
      TRY_BEGIN
      std::thread::id id(std::this_thread::get_id());//Ustal ID wątku.
      if (get_map_char().count(id)){//Jeśli istnieje stos dla wątku.
        if(get_map_char()[id].size())//Jeśli są logery na stosie.
          return(get_map_char()[id]().getLogger(severity));//Pobierz najwyższego loggera.
      }
      TRY_END
      return(blackHole);
    }
    std::wostream & wostream(flags_t severity){
      std::lock_guard<std::mutex> lock(mutex);
      static BlackHole<wchar_t> blackHoleBuff;
      static std::basic_ostream<wchar_t> blackHole(&blackHoleBuff);
      TRY_BEGIN
      std::thread::id id(std::this_thread::get_id());//Ustal ID wątku.
      if (get_map_wchar().count(id)){//Jeśli istnieje stos dla wątku.
        if(get_map_wchar()[id].size())//Jeśli są logery na stosie.
          return(get_map_wchar()[id]().getLogger(severity));//Pobierz najwyższego loggera.
      }
      TRY_END
      return(blackHole);
    }
  }
  void restart(){
    std::lock_guard<std::mutex> lock(input::mutex);
    TRY_BEGIN
    get_map_char().clear();
    get_map_wchar().clear();
    TRY_END
  }
  std::wstring pretty_function(const std::string & input){
    TRY_BEGIN
    return(ict::utf8::get(input));
    TRY_END
  }
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(logger,tc1){
  LOGGER_LAYER;
  LOGGER_SET(std::cerr);
  LOGGER_SET(std::wcerr,0x0);
  LOGGER_NOTICE<<__LOGGER__<<"Test string > string ... "<<ict::test::test_string[0]<<std::endl;
  WLOGGER_NOTICE<<__WLOGGER__<<L"Test wstring > string ... "<<ict::test::test_wstring[0]<<std::endl;
  std::cerr<<ict::test::test_string[0]<<std::endl;
  return(0);
}
REGISTER_TEST(logger,tc2){
  LOGGER_LAYER;
  LOGGER_SET(std::cerr,0x0);
  LOGGER_SET(std::wcerr);
  LOGGER_NOTICE<<__LOGGER__<<"Test string > wstring ... "<<ict::test::test_string[0]<<std::endl;
  WLOGGER_NOTICE<<__WLOGGER__<<L"Test wstring > wstring ... "<<ict::test::test_wstring[0]<<std::endl;
  std::wcerr<<ict::test::test_wstring[0]<<std::endl;
  return(0);
}
#endif
//===========================================