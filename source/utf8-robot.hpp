//! @file
//! @brief UTF8 robot header file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2018
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
#ifndef _UTF8_ROBOT_HEADER
#define _UTF8_ROBOT_HEADER
//============================================
#include <string>
//============================================
namespace ict { namespace utf8 {
//===========================================
class Robot{
private:
    bool ready=false;
    std::string s;
    wchar_t c;
public:
    //! 
    //! @brief Empty wchar.
    //! 
    static const wchar_t empty;
    //! 
    //! @brief Construct a new operator () object
    //! 
    //! @param input Znak do załadowania
    //! 
    Robot & operator ()(const char & input);
    //! 
    //! @brief Get the Char object
    //! 
    //! @return const wchar_t& 
    //! 
    const wchar_t & getChar() const {
        return(ready?c:empty);
    }
    //! 
    //! @brief Get the String object
    //! 
    //! @return const std::string& 
    //! 
    const std::string & getString() const {
        static const std::string empty_string;
        return(ready?s:empty_string);
    }
    //! 
    //! @brief 
    //! 
    //! @return true Gotowy do odczytu
    //! @return false Jeszcze nie gotowy do odczytu.
    //! 
    bool isReady() const {
        return(ready);
    }
    //! 
    //! @brief Sprawdza, czy podany string ma wielobajtowe znaki UTF8
    //! 
    //! @param input String do sprawdzenia.
    //! @return true Posiada
    //! @return false Nie posiada
    //! 
    static bool hasUtf8(const std::string & input);
};
//===========================================
} }
//============================================
#endif