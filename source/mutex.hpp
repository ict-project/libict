//! @file
//! @brief Mutex module - header file.
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
#ifndef _MUTEX_HEADER
#define _MUTEX_HEADER
//============================================
#include <mutex>
//============================================
namespace ict { namespace mutex {
//===========================================
class read_write {
friend class read_t;
friend class write_t;
private:
    bool write_priority;
    std::mutex main_mutex;
    std::mutex write_mutex;
    std::mutex read_mutex;
    u_int32_t count=0;
public:
    class read_t {
    private:  
        read_write & object;
    public:
        read_t(read_write & object_in):object(object_in){}
        void lock();
        bool try_lock() noexcept;
        void unlock();
    };
    class write_t {
    private:  
        read_write & object;
    public:
        write_t(read_write & object_in):object(object_in){}
        void lock();
        bool try_lock() noexcept;
        void unlock();
    };
    //! Interfejs mutexu odczytu.
    read_t read;
    //! Interfejs mutexu zapisu.
    write_t write;
    //! 
    //! @brief Construct a new read write object
    //! 
    //! @param write_priority_in Priorytet zapisu - wartości:
    //!  @li true - zapis ma wyższy priorytet niż odczyt;
    //!  @li false - zapis ma niższy priorytet niż odczyt.
    //! 
    read_write(bool write_priority_in=true):read(*this),write(*this),write_priority(write_priority_in){}
};
//============================================
}}
//===========================================
#endif