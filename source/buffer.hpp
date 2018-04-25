//! @file
//! @brief Buffer module - Header file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2012-2018
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2012-2018, ICT-Project Mariusz Ornowski (ict-project.pl)
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
#ifndef _BUFFER_HEADER
#define _BUFFER_HEADER
//============================================
#include <vector>
#include <string>
//============================================
namespace ict { namespace buffer {
//===========================================
typedef unsigned char byte_t;
typedef std::vector<byte_t> byte_vector_t;
//===========================================
class vector: public byte_vector_t{
private:
    std::size_t max;
public:
    vector();
    vector(std::size_t maxSize);
    std::size_t getMaxSize() const;
    std::size_t getFreeSpace() const;
    std::size_t getSize() const;

    bool testPlus(const signed char & input);
    bool testPlus(const signed short int & input);
    bool testPlus(const signed int & input);
    bool testPlus(const signed long int & input);
    bool testPlus(const signed long long int & input);
    bool testPlus(const unsigned char & input);
    bool testPlus(const unsigned short int & input);
    bool testPlus(const unsigned int & input);
    bool testPlus(const unsigned long int & input);
    bool testPlus(const unsigned long long int & input);
    bool testPlus(const float & input);
    bool testPlus(const double & input);
    bool testPlus(const long double & input);
    bool testPlus(const bool & input);
    bool testPlus(const std::string & input);
    bool testPlus(const std::wstring & input);
    bool testPlus(const byte_vector_t & input);

    vector & operator +=(const signed char & input);
    vector & operator +=(const signed short int & input);
    vector & operator +=(const signed int & input);
    vector & operator +=(const signed long int & input);
    vector & operator +=(const signed long long int & input);
    vector & operator +=(const unsigned char & input);
    vector & operator +=(const unsigned short int & input);
    vector & operator +=(const unsigned int & input);
    vector & operator +=(const unsigned long int & input);
    vector & operator +=(const unsigned long long int & input);
    vector & operator +=(const float & input);
    vector & operator +=(const double & input);
    vector & operator +=(const long double & input);
    vector & operator +=(const bool & input);
    vector & operator +=(const std::string & input);
    vector & operator +=(const std::wstring & input);
    vector & operator +=(const byte_vector_t & input);

    bool testMinus(const signed char & input);
    bool testMinus(const signed short int & input);
    bool testMinus(const signed int & input);
    bool testMinus(const signed long int & input);
    bool testMinus(const signed long long int & input);
    bool testMinus(const unsigned char & input);
    bool testMinus(const unsigned short int & input);
    bool testMinus(const unsigned int & input);
    bool testMinus(const unsigned long int & input);
    bool testMinus(const unsigned long long int & input);
    bool testMinus(const float & input);
    bool testMinus(const double & input);
    bool testMinus(const long double & input);
    bool testMinus(const bool & input);
    bool testMinus(const std::string & input);
    bool testMinus(const std::wstring & input);
    bool testMinus(const byte_vector_t & input);

    vector & operator -=(signed char & output);
    vector & operator -=(signed short int & output);
    vector & operator -=(signed int & output);
    vector & operator -=(signed long int & output);
    vector & operator -=(signed long long int & output);
    vector & operator -=(unsigned char & output);
    vector & operator -=(unsigned short int & output);
    vector & operator -=(unsigned int & output);
    vector & operator -=(unsigned long int & output);
    vector & operator -=(unsigned long long int &output);
    vector & operator -=(float & output);
    vector & operator -=(double & output);
    vector & operator -=(long double & output);
    vector & operator -=(bool & output);
    vector & operator -=(std::string & output);
    vector & operator -=(std::wstring & output);
    vector & operator -=(byte_vector_t & output);
};
//===========================================
} }
//===========================================
#endif