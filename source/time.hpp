//! @file
//! @brief Time module - Header file.
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
#ifndef _TIME_HEADER
#define _TIME_HEADER
//============================================
#include <string>
#include <sstream>
#include <iomanip>
#include <mutex>
//============================================
namespace ict { namespace time {
//===========================================

  //Czas epoki unix (sekundy).
  typedef long long     unix_int_t;
  typedef long double   unix_float_t;
  typedef unix_int_t    unix_t;

  //! Klasa do obsługi punktu w czasie (zabezpieczona mutex'em).
  class TimeInfo {
  private:
    mutable std::mutex mutex;
    void setTime(bool local);
    void setTime(unix_int_t time,bool local);
    void setTime(unix_float_t time,bool local);
  public:
    //! Tworzy punkt w czasie - teraz.
    TimeInfo(){
      setLocalTime();
    }
    //! Tworzy punkt w czasie - na podstawie czasu Unix - wersja integer.
    TimeInfo(unix_int_t time){
      setLocalTime(time);
    }
    //! Tworzy punkt w czasie - na podstawie czasu Unix - wersja float.
    TimeInfo(unix_float_t time){
      setLocalTime(time);
    }
    //! Tworzy punkt w czasie - na podstawie innej klasy tego samego typu.
    TimeInfo(const TimeInfo & input){
      unix_sec=input.unix_sec;
      msec=input.msec;
      sec=input.sec;
      min=input.min;  
      hour=input.hour; 
      mday=input.mday; 
      mon=input.mon;  
      year=input.year;
      wday=input.wday;
      yday=input.yday;
      isdst=input.isdst;
      gmtoff=input.gmtoff;
      zone=input.zone;
    }
    // unix seconds.
    unix_t unix_sec=0;
    // miliseconds
    int msec=0;
    // seconds after the minute 0-60*
    int sec=0;
    // minutes after the hour 0-59
    int min=0;  
    // hours since midnight 0-23
    int hour=0; 
    // day of the month 1-31
    int mday=0; 
    // months since January 0-11
    int mon=0;  
    // years since 1900
    int year=0;
    // days since Sunday 0-6
    int wday=0;
    // days since January 1 0-365
    int yday=0;
    // Daylight Saving Time flag
    int isdst=0;
    // Seconds east of UTC 
    long gmtoff;
    // Timezone abbreviation
    #ifdef __APPLE__
     char *zone;
    #else
     const char *zone;
    #endif
    // Ustawia punkt w czasie na teraz (czas lokalny).
    void setLocalTime(){std::lock_guard<std::mutex> lock(mutex);setTime(true);}
    // Ustawia punkt w czasie na teraz (czas uniwersalny).
    void setGmtTime(){std::lock_guard<std::mutex> lock(mutex);setTime(false);}
    // Ustawia punkt - na podstawie czasu Unix - wersja integer (czas lokalny).
    void setLocalTime(unix_int_t time){std::lock_guard<std::mutex> lock(mutex);setTime(time,true);}
    // Ustawia punkt - na podstawie czasu Unix - wersja integer (czas uniwersalny).
    void setGmtTime(unix_int_t time){std::lock_guard<std::mutex> lock(mutex);setTime(time,false);}
    // Ustawia punkt - na podstawie czasu Unix - wersja float (czas lokalny).
    void setLocalTime(unix_float_t time){std::lock_guard<std::mutex> lock(mutex);setTime(time,true);}
    // Ustawia punkt - na podstawie czasu Unix - wersja float (czas uniwersalny).
    void setGmtTime(unix_float_t time){std::lock_guard<std::mutex> lock(mutex);setTime(time,false);}
    //! Zobacz ::strftime()
    std::string formatTime(const std::string & format);
  };

//===========================================
} }
//===========================================
template <
  typename charT=char,
  typename traits=std::char_traits<charT>
>
inline
std::basic_ostream<charT,traits> & operator << (
  std::basic_ostream<charT,traits> & os,
  const ict::time::TimeInfo & timeinfo
) {
  // Opcja 
  int option=0;
  switch (option){
  case 0:
    os<<std::setfill<charT>('0')<<std::right<<std::setw(4)<<(timeinfo.year+1900)<<os.widen('-');
    os<<std::setfill<charT>('0')<<std::right<<std::setw(2)<<(timeinfo.mon+1)<<os.widen('-');
    os<<std::setfill<charT>('0')<<std::right<<std::setw(2)<<(timeinfo.mday)<<os.widen(' ');
    os<<std::setfill<charT>('0')<<std::right<<std::setw(2)<<(timeinfo.hour)<<os.widen(':');
    os<<std::setfill<charT>('0')<<std::right<<std::setw(2)<<(timeinfo.min)<<os.widen(':');
    os<<std::setfill<charT>('0')<<std::right<<std::setw(2)<<(timeinfo.sec)<<os.widen('.');
    os<<std::setfill<charT>('0')<<std::right<<std::setw(3)<<(timeinfo.msec);
  break;
  default:break;
  }
  return(os);
}
//===========================================
#endif
