//! @file
//! @brief Mutex module - Source file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2016-2018
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2016-2018, ICT-Project Mariusz Ornowski (ict-project.pl)
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
#include "mutex.hpp"
//============================================
namespace ict { namespace mutex {
//===========================================
void read_write::read_t::lock(){
  if (object.write_priority){
    object.main_mutex.lock();
    object.main_mutex.unlock();
  }
  {
    std::unique_lock<std::mutex> lock (object.read_mutex);
    if (object.count<0xffffffff){
      object.count++;
      if (object.count==1) object.write_mutex.lock();
    }
  }
}
bool read_write::read_t::try_lock() noexcept {
  if (object.write_priority){
    object.main_mutex.lock();
    object.main_mutex.unlock();
  }
  {
    std::unique_lock<std::mutex> lock (object.read_mutex);
    if (object.count<0xffffffff){
      object.count++;
      if (object.count==1) {
        if (object.write_mutex.try_lock()){
          return(true);
        } else {
          object.count--;
        }
      } else {
        return(true);
      }
    }
  }
  return(false);
}
void read_write::read_t::unlock(){
  std::unique_lock<std::mutex> lock (object.read_mutex);
  if (0<object.count) {
    object.count--;
    if (object.count==0) object.write_mutex.unlock();
  }
}
void read_write::write_t::lock(){
  object.main_mutex.lock();
  object.write_mutex.lock();
}
bool read_write::write_t::try_lock() noexcept {
  object.main_mutex.lock();
  if (object.write_mutex.try_lock()){
    return(true);
  }
  object.main_mutex.unlock();
  return(false);
}
void read_write::write_t::unlock(){
  object.write_mutex.unlock();
  object.main_mutex.unlock();
}
//============================================
}}
//============================================