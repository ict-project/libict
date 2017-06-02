//! @file
//! @brief Daemon module - Source file.
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
#include "daemon.hpp"
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <fstream>
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace daemon {
//===========================================
bool daemonize(bool log){
  static const int write_flags=O_WRONLY|O_CREAT|O_APPEND;
  static const mode_t write_mode=S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
  int out;
  pid_t pid;
  // First fork
  if (log) ::syslog(LOG_DEBUG,"daemonize: First fork is about to start ...");
  if (pid=::fork()) {
    if (pid>0) {//Parent
      if (log) ::syslog(LOG_DEBUG,"daemonize: Parent exits after first fork (%i)...",pid);
      ::exit(0);
    } else {//Error
      if (log) ::syslog(LOG_ERR,"daemonize: Error after first fork (%i)...",pid);
      return(false);
    }
  }
  pid=::setsid();
  out=::chdir("/");
  ::umask(0);
  // Second fork.
  if (log) ::syslog(LOG_DEBUG,"daemonize: Second fork is about to start ...");
  if (pid=::fork()) {
    if (pid>0){//Parent
      if (log) ::syslog(LOG_DEBUG,"daemonize: Parent exits after second fork (%i)...",pid);
      ::exit(0);
    } else {//Error
      if (log) ::syslog(LOG_ERR,"daemonize: Error after second fork (%i)...",pid);
      return(false);
    }
  }
  // Close the standard streams.
  out=::close(0);
  out=::close(1);
  out=::close(2);
  if (::open("/dev/null",O_RDONLY)<0){//STDIN from /dev/null
    if (log) ::syslog(LOG_ERR,"daemonize: Could not open STDIN (%i)...",errno);
    return(false);//Error
  }
  if (::open("/dev/null",write_flags,write_mode)<0) {//STDOUT to /dev/null
    if (log) ::syslog(LOG_ERR,"daemonize: Could not open STDOUT (%i)...",errno);
    return(false);//Error
  }
  if (::dup(1)<0) {//STDERR to /dev/null
    if (log) ::syslog(LOG_ERR,"daemonize: Could not open STDERR (%i)...",errno);
    return(false);//Error
  }
  ::syslog(LOG_INFO,"daemonize: Daemon is ready (PID:%i)...",::getpid());
  return(true);
}
bool savePid(const std::string & path){
  try{
    std::ofstream pid(path,std::ofstream::out|std::ofstream::trunc);
    pid<<::getpid();
    pid.close();
  } catch(...){
    return(false);
  }
  return(true);
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(daemon,tc1){
  if (!ict::daemon::savePid("/tmp/test.pid")){
    std::cout<<"Błąd("<<__LINE__<<")!!!"<<std::endl;
    return(-1);
  }
  return(0);
}
#endif
//===========================================
