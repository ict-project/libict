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
#include <deque>
#include <string>
#include <stdexcept>
//============================================
namespace ict { namespace buffer {
//===========================================
typedef unsigned char byte_t;
typedef std::vector<byte_t> byte_vector_t;
//===========================================
class interface {
protected:
    typedef uint32_t array_size_t;
private:
    template<typename T> bool testInput(const T & input){
        return(getFreeSpace()<sizeof(input)); 
    }
    template<typename T> bool testOutput(const T & output){
        return(getSize()<sizeof(output)); 
    }
    template<class T> bool testInputArray(const T & input){
        return(getFreeSpace()<(sizeof(input.back())*input.size()+sizeof(array_size_t))); 
    }
    template<class T> bool testOutputArray(const T & output){
        return(getSize()<(sizeof(output.back())*getArraySize()+sizeof(array_size_t)));
    }
    template<typename T> void dataIn(const T & input){
        const byte_t* ptr=(byte_t*)(&input);
        if (testInput(input)) throw std::range_error("Input to large for ict::buffer::interface [1]!");
        for (std::size_t k=0;k<sizeof(input);k++)  byteIn(ptr[k]);
    }
    template<typename T> void dataOut(T & output){
        byte_t* ptr=(byte_t*)(&output);
        if (testOutput(output)) throw std::range_error("Output to small for ict::buffer::interface [1]!");
        for (std::size_t k=0;k<sizeof(output);k++) byteOut(ptr[k]);
    }
    template<class T> void dataInArray(const T & input){
        array_size_t size=input.size();
        if (testInputArray(input)) throw std::range_error("Input to large for ict::buffer::interface [2]!");
        dataIn(size);
        for (std::size_t k=0;k<size;k++) dataIn(input[k]);
    }
    template<class T> void dataOutArray(T & output){
        array_size_t size;
        if (testOutputArray(output)) throw std::range_error("Output to small for ict::buffer::interface [2]!");
        dataOut(size);
        for (std::size_t k=0;k<size;k++) dataOut(output[k]);
    }
    virtual void byteIn(const byte_t & byte)=0;
    virtual void byteOut(byte_t & byte)=0;
    virtual array_size_t getArraySize()=0;
public:
    virtual std::size_t getMaxSize() const=0;
    virtual std::size_t getFreeSpace() const=0;
    virtual std::size_t getSize() const=0;

    bool testIn(const signed char & input){return(!testInput(input));}
    bool testIn(const signed short int & input){return(!testInput(input));}
    bool testIn(const signed int & input){return(!testInput(input));}
    bool testIn(const signed long int & input){return(!testInput(input));}
    bool testIn(const signed long long int & input){return(!testInput(input));}
    bool testIn(const unsigned char & input){return(!testInput(input));}
    bool testIn(const unsigned short int & input){return(!testInput(input));}
    bool testIn(const unsigned int & input){return(!testInput(input));}
    bool testIn(const unsigned long int & input){return(!testInput(input));}
    bool testIn(const unsigned long long int & input){return(!testInput(input));}
    bool testIn(const float & input){return(!testInput(input));}
    bool testIn(const double & input){return(!testInput(input));}
    bool testIn(const long double & input){return(!testInput(input));}
    bool testIn(const bool & input){return(!testInput(input));}
    bool testIn(const std::string & input){return(!testInputArray(input));}
    bool testIn(const std::wstring & input){return(!testInputArray(input));}
    bool testIn(const byte_vector_t & input){return(!testInputArray(input));}

    interface & operator <<(const signed char & input){dataIn(input);return(*this);}
    interface & operator <<(const signed short int & input){dataIn(input);return(*this);}
    interface & operator <<(const signed int & input){dataIn(input);return(*this);}
    interface & operator <<(const signed long int & input){dataIn(input);return(*this);}
    interface & operator <<(const signed long long int & input){dataIn(input);return(*this);}
    interface & operator <<(const unsigned char & input){dataIn(input);return(*this);}
    interface & operator <<(const unsigned short int & input){dataIn(input);return(*this);}
    interface & operator <<(const unsigned int & input){dataIn(input);return(*this);}
    interface & operator <<(const unsigned long int & input){dataIn(input);return(*this);}
    interface & operator <<(const unsigned long long int & input){dataIn(input);return(*this);}
    interface & operator <<(const float & input){dataIn(input);return(*this);}
    interface & operator <<(const double & input){dataIn(input);return(*this);}
    interface & operator <<(const long double & input){dataIn(input);return(*this);}
    interface & operator <<(const bool & input){dataIn(input);return(*this);}
    interface & operator <<(const std::string & input){dataInArray(input);return(*this);}
    interface & operator <<(const std::wstring & input){dataInArray(input);return(*this);}
    interface & operator <<(const byte_vector_t & input){dataInArray(input);return(*this);}

    bool testOut(const signed char & output){return(!testOutput(output));}
    bool testOut(const signed short int & output){return(!testOutput(output));}
    bool testOut(const signed int & output){return(!testOutput(output));}
    bool testOut(const signed long int & output){return(!testOutput(output));}
    bool testOut(const signed long long int & output){return(!testOutput(output));}
    bool testOut(const unsigned char & output){return(!testOutput(output));}
    bool testOut(const unsigned short int & output){return(!testOutput(output));}
    bool testOut(const unsigned int & output){return(!testOutput(output));}
    bool testOut(const unsigned long int & output){return(!testOutput(output));}
    bool testOut(const unsigned long long int & output){return(!testOutput(output));}
    bool testOut(const float & output){return(!testOutput(output));}
    bool testOut(const double & output){return(!testOutput(output));}
    bool testOut(const long double & output){return(!testOutput(output));}
    bool testOut(const bool & output){return(!testOutput(output));}
    bool testOut(const std::string & output){return(!testOutputArray(output));}
    bool testOut(const std::wstring & output){return(!testOutputArray(output));}
    bool testOut(const byte_vector_t & output){return(!testOutputArray(output));}

    interface & operator >>(signed char & output){dataOut(output);return(*this);}
    interface & operator >>(signed short int & output){dataOut(output);return(*this);}
    interface & operator >>(signed int & output){dataOut(output);return(*this);}
    interface & operator >>(signed long int & output){dataOut(output);return(*this);}
    interface & operator >>(signed long long int & output){dataOut(output);return(*this);}
    interface & operator >>(unsigned char & output){dataOut(output);return(*this);}
    interface & operator >>(unsigned short int & output){dataOut(output);return(*this);}
    interface & operator >>(unsigned int & output){dataOut(output);return(*this);}
    interface & operator >>(unsigned long int & output){dataOut(output);return(*this);}
    interface & operator >>(unsigned long long int &output){dataOut(output);return(*this);}
    interface & operator >>(float & output){dataOut(output);return(*this);}
    interface & operator >>(double & output){dataOut(output);return(*this);}
    interface & operator >>(long double & output){dataOut(output);return(*this);}
    interface & operator >>(bool & output){dataOut(output);return(*this);}
    interface & operator >>(std::string & output){dataOutArray(output);return(*this);}
    interface & operator >>(std::wstring & output){dataOutArray(output);return(*this);}
    interface & operator >>(byte_vector_t & output){dataOutArray(output);return(*this);}
};
//===========================================
class basic:public interface {
private:
    std::size_t max;
    std::deque<byte_t> q;
    void byteIn(const byte_t & byte);
    void byteOut(byte_t & byte);
    interface::array_size_t getArraySize();
public:
    basic();
    basic(const std::size_t & maxSize);
    std::size_t getMaxSize() const;
    std::size_t getFreeSpace() const;
    std::size_t getSize() const;
};
//===========================================
} }
//===========================================
#endif