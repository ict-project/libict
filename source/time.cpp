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
    //this->unix=tv.tv_sec;
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
    unix_int_t t=time*1000;
    //int m=t%1000;
    t/=1000;
    setTime(t,local);
    this->msec=t;
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

//===========================================
} }
//===========================================
