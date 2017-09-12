//! @file
//! @brief OS module - Source file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2016-2017
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2016-2017, ICT-Project Mariusz Ornowski (ict-project.pl)
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
#include "os.hpp"
#include "global.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <dirent.h>
//============================================
namespace ict { namespace os {
//===========================================
const unsigned char dt_UNKNOWN(::DT_UNKNOWN);
const unsigned char dt_REG(::DT_REG);
const unsigned char dt_DIR(::DT_DIR);
const unsigned char dt_FIFO(::DT_FIFO);
const unsigned char dt_SOCK(::DT_SOCK);
const unsigned char dt_CHR(::DT_CHR);
const unsigned char dt_BLK(::DT_BLK);
const unsigned char dt_LNK(::DT_LNK);
const static char separator='/';
//===========================================
std::string getCurrentDir(){
  std::string out;
  char *path=getcwd(NULL,0);
  if (path){
    out=path;
    free(path);
  }
  if (out.back()!=separator) out+=separator;
  return(out);
}
std::string getOnlyDir(const std::string & path){
  std::string out(path);
  std::size_t p;
  if (path=="-") return(getCurrentDir());
  p=path.rfind(separator);
  if (p==std::string::npos) {
    out="/";
  } else {
    out=out.substr(0,p+1);
  }
  return(out);
}
bool isFileReadable(const std::string & base,const std::string & path){
  if (path=="-") return(true);
  return(access(getRealPath(base,path).c_str(),R_OK)==0);
}
bool isFileReadable(const std::string & path){
  return(isFileReadable("-",path));
}
std::string getRealPath(const std::string & path){
  std::string out(path);
  if (path=="-") return(getRealPath(path));
  {
    char *full_path=realpath(path.c_str(),NULL);
    if (full_path) {
      out=full_path;
      free(full_path);
    }
  }
  if ((path.back()==separator)&&(out.back()!=separator)) out+=separator;
  return(out);
}
std::string getRealPath(const std::string & base,const std::string & path){
  if (path=="-") return(getRealPath(path));
  if (base=="-") return(getRealPath(path));
  if (path.front()==separator) return(getRealPath(path));
  return(getRealPath(getOnlyDir(base)+path));
}
std::string getRelativePath(const std::string & base,const std::string & path){
  std::string out(".");
  out+=separator;
  std::size_t i=0;
  std::size_t k=0;
  std::string tmp_base((base=="-")?getCurrentDir():getRealPath(base));
  std::string tmp_path(getRealPath(path));
  if (path=="-") return(path);
  for (;(i<tmp_base.size())&&(i<tmp_path.size());i++){
    if (tmp_base.at(i)==tmp_path.at(i)) {
      if (tmp_base.at(i)==separator) k=i+1;
    } else {
      break;
    }
  }
  tmp_base=tmp_base.substr(k);
  tmp_path=tmp_path.substr(k);
  k=0;
  for (auto c : tmp_base) if (c==separator) k++;
  for (i=0;i<k;i++) {
    out+="..";
    out+=separator;
  }
  out+=tmp_path;
  return(out);
}
std::string getRelativePath(const std::string & path){
  return(getRelativePath("-",path));
}
std::vector<std::string> getDirList(const std::string & dir,unsigned char dt){
  std::vector<std::string> out;
  ::DIR * d=::opendir(dir.c_str());
  if (d){
    struct ::dirent * de;
    while (de=::readdir(d)) if ((dt==dt_UNKNOWN)||(de->d_type==dt)) {
      out.emplace_back(de->d_name);
    }
    ::closedir(d);
  }
  return(out);
}
//============================================
}}
//============================================