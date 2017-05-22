//! @file
//! @brief Time module - Source file.
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
#include "time.hpp"
#include <sys/time.h>
#include <time.h>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace time {
//===========================================
  void TimeInfo::setTime(bool local){
    time_t rawtime;
    //struct tm * timeinfo;
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    ::time(&rawtime);
    setTime((unix_int_t)rawtime,local);
    this->msec=tv.tv_usec/1000;
  }
  void TimeInfo::setTime(unix_int_t time,bool local){
    time_t rawtime(time);
    struct tm * timeinfo;
    if (local){
      timeinfo=localtime(&rawtime);
    } else {
      timeinfo=gmtime(&rawtime);
    }
    this->unix_sec=time;
    this->msec=0;
    this->sec=timeinfo->tm_sec;
    this->min=timeinfo->tm_min;
    this->hour=timeinfo->tm_hour;
    this->mday=timeinfo->tm_mday;
    this->mon=timeinfo->tm_mon;
    this->year=timeinfo->tm_year;
    this->wday=timeinfo->tm_wday;
    this->yday=timeinfo->tm_yday;
    this->isdst=timeinfo->tm_isdst;
    this->gmtoff=timeinfo->tm_gmtoff;
    this->zone=timeinfo->tm_zone;
  }
  void TimeInfo::setTime(unix_float_t time,bool local){
    unix_float_t t=time*1000;
    unix_int_t s=t;
    unix_int_t m=t;
    s/=1000;
    m%=1000;
    setTime(s,local);
    this->msec=m;
  }
  std::string TimeInfo::formatTime(const std::string & format){
    std::lock_guard<std::mutex> lock(mutex);
    char outstr[200]="";
    struct tm timeinfo;
    timeinfo.tm_sec=this->sec;
    timeinfo.tm_min=this->min;
    timeinfo.tm_hour=this->hour;
    timeinfo.tm_mday=this->mday;
    timeinfo.tm_mon=this->mon;
    timeinfo.tm_year=this->year;
    timeinfo.tm_wday=this->wday;
    timeinfo.tm_yday=this->yday;
    timeinfo.tm_isdst=this->isdst;
    timeinfo.tm_gmtoff=this->gmtoff;
    timeinfo.tm_zone=this->zone;
    ::strftime(
      outstr,sizeof(outstr),
      format.c_str(),&timeinfo
    );
    return(outstr);
  }
  void TimeInfo::correctLocalTime(){
    std::lock_guard<std::mutex> lock(mutex);
    struct tm timeinfo;
    while(this->msec<0){
      this->msec+=1000;
      this->sec--;
    }
    this->sec+=(this->msec/1000);
    this->msec%=1000;
    timeinfo.tm_sec=this->sec;
    timeinfo.tm_min=this->min;
    timeinfo.tm_hour=this->hour;
    timeinfo.tm_mday=this->mday;
    timeinfo.tm_mon=this->mon;
    timeinfo.tm_year=this->year;
    timeinfo.tm_wday=this->wday;
    timeinfo.tm_yday=this->yday;
    timeinfo.tm_isdst=-1;
    timeinfo.tm_gmtoff=this->gmtoff;
    timeinfo.tm_zone=this->zone;
    {
      unix_int_t s=::mktime(&timeinfo);
      if (s>0) {
        unix_float_t t=this->msec;
        t/=1000;
        t+=s;
        setTime(t,true);
      }
    }
  }
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(time,tc1){
  ict::time::unix_float_t t;
  ict::time::TimeInfo ti;
  std::cout<<ti<<std::endl;
  t=999;
  t/=1000;
  std::cout<<t<<std::endl;
  t+=ti.unix_sec;
  std::cout<<t<<std::endl;
  std::cout<<"long double::digits10="<<std::numeric_limits<long double>::digits10<< std::endl;
  std::cout<<"long long int::digits10="<<std::numeric_limits<long long int>::digits10<< std::endl;
  return(0);
}
REGISTER_TEST(time,tc2){
  ict::time::TimeInfo ti1;
  ict::time::TimeInfo ti2(ti1);
  std::cout<<ti1<<"=="<<ti2<<std::endl;
  ti1.hour-=100*24;
  ti1.correctLocalTime();
  std::cout<<ti1<<"!="<<ti2<<std::endl;
  ti1.mday+=100;
  ti1.correctLocalTime();
  std::cout<<ti1<<"=="<<ti2<<std::endl;
  if (ti1.unix_sec!=ti2.unix_sec){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" unix_sec: "<<ti1.unix_sec<<"!="<<ti2.unix_sec<<std::endl;
    return(-1);
  }
  if (ti1.msec!=ti2.msec){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" msec: "<<ti1.msec<<"!="<<ti2.msec<<std::endl;
    return(-1);
  }
  if (ti1.sec!=ti2.sec){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" sec: "<<ti1.sec<<"!="<<ti2.sec<<std::endl;
    return(-1);
  }
  if (ti1.min!=ti2.min){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" min: "<<ti1.min<<"!="<<ti2.min<<std::endl;
    return(-1);
  }
  if (ti1.hour!=ti2.hour){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" hour: "<<ti1.hour<<"!="<<ti2.hour<<std::endl;
    return(-1);
  }
  if (ti1.mday!=ti2.mday){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" mday: "<<ti1.mday<<"!="<<ti2.mday<<std::endl;
    return(-1);
  }
  if (ti1.mon!=ti2.mon){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" mon: "<<ti1.mon<<"!="<<ti2.mon<<std::endl;
    return(-1);
  }
  if (ti1.year!=ti2.year){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" year: "<<ti1.year<<"!="<<ti2.year<<std::endl;
    return(-1);
  }
  if (ti1.wday!=ti2.wday){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" wday: "<<ti1.wday<<"!="<<ti2.wday<<std::endl;
    return(-1);
  }
  if (ti1.yday!=ti2.yday){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" yday: "<<ti1.yday<<"!="<<ti2.yday<<std::endl;
    return(-1);
  }
  if (ti1.isdst!=ti2.isdst){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" isdst: "<<ti1.isdst<<"!="<<ti2.isdst<<std::endl;
    return(-1);
  }
  if (ti1.gmtoff!=ti2.gmtoff){
    std::cout<<" Błąd!!!"<<std::endl;
    std::cout<<" gmtoff: "<<ti1.gmtoff<<"!="<<ti2.gmtoff<<std::endl;
    return(-1);
  }
  return(0);
}
#endif
//===========================================