//! @file
//! @brief UTF8 header file.
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
#ifndef _UTF8_HEADER
#define _UTF8_HEADER
//============================================
#include <string>
//============================================
namespace ict { namespace utf8 {
//===========================================
//! 
//! @brief Zamienia UTF string na wstring.
//!
//! @param input Wejściowy ciąg znaków (przetworzone znaki są usuwane ze zmiennej).
//! @param output Wyjściowy ciąg znaków (przetworzone znaki są dodawane do zmiennej).
//!
void transfer(std::string & input,std::wstring & output);
//! 
//! @brief Zamienia UTF string na wstring.
//!
//! @param input Wejściowy ciąg znaków.
//! @return Wyjściowy ciąg znaków.
//!
std::wstring get(const std::string & input_in);
//! 
//! @brief Zamienia UTF wstring na string.
//!
//! @param input Wejściowy ciąg znaków (przetworzone znaki są usuwane ze zmiennej).
//! @param output Wyjściowy ciąg znaków (przetworzone znaki są dodawane do zmiennej).
//!
void transfer(std::wstring & input,std::string & output);
//! 
//! @brief Zamienia UTF wstring na string.
//!
//! @param input Wejściowy ciąg znaków.
//! @return Wyjściowy ciąg znaków.
//!
std::string get(const std::wstring & input_in);
//===========================================
} }
//============================================
#endif