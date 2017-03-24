//! @file
//! @brief Base64 module - header file.
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
#ifndef _BASE64_HEADER
#define _BASE64_HEADER
//============================================
#include <string>
//============================================
namespace ict { namespace base64 {
//===========================================
//! 
//! @brief Koduje strng do base64.
//!
//! @param input Wejściowy ciąg znaków.
//! @param output Wyjściowy ciąg znaków (nie jest czyszczony przed wstawieniem).
//! @param url Informacja, czy znaki '+' i '/' mają być zastąpione (odpowiednio) '-' i '_' (by zakodowany string mógł być użyty w URL).
//! @param padding Informacja, czy znaki uzupełnienia (padding - '=') mają być użyte.
//!
void encode(const std::string & input,std::string & output,bool url=false,bool padding=true);
//! 
//! @brief Koduje strng do base64.
//!
//! @param input Wejściowy ciąg znaków.
//! @param url Informacja, czy znaki '+' i '/' mają być zastąpione (odpowiednio) '-' i '_' (by zakodowany string mógł być użyty w URL).
//! @param padding Informacja, czy znaki uzupełnienia (padding - '=') mają być użyte.
//! @return Wyjściowy ciąg znaków.
//!
std::string encode(const std::string & input,bool url=false,bool padding=true);
//! 
//! @brief Dekoduje strng z base64.
//!
//! @param input Wejściowy ciąg znaków.
//! @param output Wyjściowy ciąg znaków (nie jest czyszczony przed wstawieniem).
//!
void decode(const std::string & input,std::string & output);
//! 
//! @brief Dekoduje strng z base64.
//!
//! @param input Wejściowy ciąg znaków.
//! @returns Wyjściowy ciąg znaków.
//!
std::string decode(const std::string & input);
//===========================================
} }
//============================================
#endif