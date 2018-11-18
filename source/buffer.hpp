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
class size_type{
private:
    static const byte_t mask=0x7F;
    bool is_ready;
    byte_vector_t bytes;
    std::size_t size;
public:
    size_type(){clear();}
    size_type(const size_type & input):
        is_ready(input.is_ready),bytes(input.bytes),size(input.size){}
    size_type(const std::size_t & input){
        clear();
        operator()(input);
    }
    size_type(const byte_t & input){
        clear();
        operator()(input);
    }
    void clear();
    size_type & operator()(const std::size_t & input);
    size_type & operator()(const byte_t & input);
    bool isReady() const {return(is_ready);}
    const std::size_t & getSize() const {return(size);}
    const byte_vector_t & getBytes() const {return(bytes);}
};
class interface{
public:
    //! 
    //! @brief Zwraca maksymalny rozmiar rozmiar bufora. Funkcja do nadpisania.
    //! 
    //! @return std::size_t Maksymalny rozmiar bufora.
    //! 
    virtual std::size_t getMaxSize() const=0;
    //! 
    //! @brief Zwraca wolną przestrzeń w buforze. Funkcja do nadpisania.
    //! 
    //! @return std::size_t Rozmiar wolnej przestrzeni.
    //! 
    virtual std::size_t getFreeSpace() const=0;
    //! 
    //! @brief Zwraca zajętą przestrzeń w buforze. Funkcja do nadpisania.
    //! 
    //! @return std::size_t Rozmiar zajętej przestrzeni.
    //! 
    virtual std::size_t getSize() const=0;
    //! 
    //! @brief Czyści bufor;
    //! 
    virtual void clear()=0;

    //! 
    //! @brief Sprawdza, czy podany 'input' zmieści się w buforze.
    //! 
    //! @param $input Wejście do przetestowania.
    //! @return true 'input' zmieści się w buforze.
    //! @return false  'input' nie zmieści się w buforze.
    //! 
    virtual bool testIn(const signed char & input) const=0;
    virtual bool testIn(const signed short int & input) const=0;
    virtual bool testIn(const signed int & input) const=0;
    virtual bool testIn(const signed long int & input) const=0;
    virtual bool testIn(const signed long long int & input) const=0;
    virtual bool testIn(const unsigned char & input) const=0;
    virtual bool testIn(const unsigned short int & input) const=0;
    virtual bool testIn(const unsigned int & input) const=0;
    virtual bool testIn(const unsigned long int & input) const=0;
    virtual bool testIn(const unsigned long long int & input) const=0;
    virtual bool testIn(const float & input) const=0;
    virtual bool testIn(const double & input) const=0;
    virtual bool testIn(const long double & input) const=0;
    virtual bool testIn(const bool & input) const=0;
    virtual bool testIn(const std::string & input) const=0;
    virtual bool testIn(const std::wstring & input) const=0;
    virtual bool testIn(const byte_vector_t & input) const=0;

    //! 
    //! @brief Dodaje zawartość 'input' do bufora.
    //! 
    //! @param input Zawartość, która ma być dodana do bufora.
    //! @return interface& Referencja do siebie.
    //! 
    virtual interface & operator <<(const signed char & input)=0;
    virtual interface & operator <<(const signed short int & input)=0;
    virtual interface & operator <<(const signed int & input)=0;
    virtual interface & operator <<(const signed long int & input)=0;
    virtual interface & operator <<(const signed long long int & input)=0;
    virtual interface & operator <<(const unsigned char & input)=0;
    virtual interface & operator <<(const unsigned short int & input)=0;
    virtual interface & operator <<(const unsigned int & input)=0;
    virtual interface & operator <<(const unsigned long int & input)=0;
    virtual interface & operator <<(const unsigned long long int & input)=0;
    virtual interface & operator <<(const float & input)=0;
    virtual interface & operator <<(const double & input)=0;
    virtual interface & operator <<(const long double & input)=0;
    virtual interface & operator <<(const bool & input)=0;
    virtual interface & operator <<(const std::string & input)=0;
    virtual interface & operator <<(const std::wstring & input)=0;
    virtual interface & operator <<(const byte_vector_t & input)=0;

    //! 
    //! @brief Wprowadza dane do bufora.
    //! 
    //! @param input Wejście do wprowadzenia (wprowadzone dane są kasowane).
    //! @param max Maksymalna liczba bajtów do wprowadzenia (jeśli zero - bez ograniczeń).
    //! @param min Minimalna liczba bajtów do wprowadzenia.
    //! @return std::size_t Liczba bajtów wprowadzona do bufora.
    //! 
    virtual std::size_t dataIn(std::string & input,const std::size_t & max=0,const std::size_t & min=0)=0;
    virtual std::size_t dataIn(std::wstring & input,const std::size_t & max=0,const std::size_t & min=0)=0;
    virtual std::size_t dataIn(byte_vector_t & input,const std::size_t & max=0,const std::size_t & min=0)=0;

    //! 
    //! @brief Sprawdza, czy zawartość bufora wystarczy do wypełnienia 'output'.
    //! 
    //! @param output Wyjście do przetestowania.
    //! @return true Bufor wystarczy do wypełnienia 'output'.
    //! @return false Bufor nie wystarczy do wypełnienia 'output'.
    //! 
    virtual bool testOut(const signed char & output) const=0;
    virtual bool testOut(const signed short int & output) const=0;
    virtual bool testOut(const signed int & output) const=0;
    virtual bool testOut(const signed long int & output) const=0;
    virtual bool testOut(const signed long long int & output) const=0;
    virtual bool testOut(const unsigned char & output) const=0;
    virtual bool testOut(const unsigned short int & output) const=0;
    virtual bool testOut(const unsigned int & output) const=0;
    virtual bool testOut(const unsigned long int & output) const=0;
    virtual bool testOut(const unsigned long long int & output) const=0;
    virtual bool testOut(const float & output) const=0;
    virtual bool testOut(const double & output) const=0;
    virtual bool testOut(const long double & output) const=0;
    virtual bool testOut(const bool & output) const=0;
    virtual bool testOut(const std::string & output) const=0;
    virtual bool testOut(const std::wstring & output) const=0;
    virtual bool testOut(const byte_vector_t & output) const=0;

    //! 
    //! @brief Wstawia zawartość bufora do 'output'.
    //! 
    //! @param output Miejsce, do którego ma być wstawiona zawartość bufora.
    //! @return interface& Referencja do siebie.
    //! 
    virtual interface & operator >>(signed char & output)=0;
    virtual interface & operator >>(signed short int & output)=0;
    virtual interface & operator >>(signed int & output)=0;
    virtual interface & operator >>(signed long int & output)=0;
    virtual interface & operator >>(signed long long int & output)=0;
    virtual interface & operator >>(unsigned char & output)=0;
    virtual interface & operator >>(unsigned short int & output)=0;
    virtual interface & operator >>(unsigned int & output)=0;
    virtual interface & operator >>(unsigned long int & output)=0;
    virtual interface & operator >>(unsigned long long int &output)=0;
    virtual interface & operator >>(float & output)=0;
    virtual interface & operator >>(double & output)=0;
    virtual interface & operator >>(long double & output)=0;
    virtual interface & operator >>(bool & output)=0;
    virtual interface & operator >>(std::string & output)=0;
    virtual interface & operator >>(std::wstring & output)=0;
    virtual interface & operator >>(byte_vector_t & output)=0;

    //! 
    //! @brief Wyprowadza dane z bufora.
    //! 
    //! @param output Wyjście z wprowadzenia (wyprowadzane dane są dodawane na końcu).
    //! @param max  Maksymalna liczba bajtów do wyprowadzenia (jeśli zero - bez ograniczeń).
    //! @param min Minimalna liczba bajtów do wprowadzenia.
    //! @return std::size_t Liczba bajtów wyprowadzona z bufora.
    //! 
    virtual std::size_t dataOut(std::string & output,const std::size_t & max=0,const std::size_t & min=0)=0;
    virtual std::size_t dataOut(std::wstring & output,const std::size_t & max=0,const std::size_t & min=0)=0;
    virtual std::size_t dataOut(byte_vector_t & output,const std::size_t & max=0,const std::size_t & min=0)=0;
};
class byte_interface : public interface{
private:
    template<typename T> bool testInput(const T & input) const {
        return(getFreeSpace()<sizeof(input)); 
    }
    template<typename T> bool testOutput(const T & output) const {
        return(getSize()<sizeof(output)); 
    }
    bool testInput(const size_type & input) const;
    bool testOutput(const size_type & output) const;
    template<class T> bool testInputArray(const T & input) const {
        size_type s(input.size()*sizeof(input.back()));
        return(getFreeSpace()<(s.getBytes().size()+s.getSize())); 
    }
    template<class T> bool testOutputArray(const T & output) const {
        size_type s;
        try{
            s=getArraySize();
        }catch(...){
            return(true);
        }
        return(getSize()<(s.getBytes().size()+s.getSize()));
    }
    template<typename T> void dataIn(const T & input,bool test=true){
        const byte_t* ptr=(byte_t*)(&input);
        if (test) if (testInput(input)) throw std::range_error("Input to large for ict::buffer::interface [1]!");
        for (std::size_t k=0;k<sizeof(input);k++)  byteIn(ptr[k]);
    }
    template<typename T> void dataOut(T & output,bool test=true){
        byte_t* ptr=(byte_t*)(&output);
        if (test) if (testOutput(output)) throw std::range_error("Output to small for ict::buffer::interface [1]!");
        for (std::size_t k=0;k<sizeof(output);k++) byteOut(ptr[k]);
    }
    void dataIn(const size_type & input,bool test=true);
    void dataOut(size_type & output,bool test=true);
    template<class T> void dataInArray(const T & input,bool test=true){
        size_type s(input.size()*sizeof(input.back()));
        if (test) if (testInputArray(input)) throw std::range_error("Input to large for ict::buffer::interface [2]!");
        dataIn(s,false);
        for (const auto & i: input) dataIn(i,false);
    }
    template<class T> void dataOutArray(T & output,bool test=true){
        size_type s;
        if (test) if (testOutputArray(output)) throw std::range_error("Output to small for ict::buffer::interface [2]!");
        dataOut(s,false);
        output.resize(s.getSize()/sizeof(output.back()));
        for (std::size_t k=0;k<output.size();k++) dataOut(output[k],false);
    }

    template<class T> std::size_t dataIn_priv(T & input,const std::size_t & max,const std::size_t & min){
        std::size_t out=0;
        typename T::iterator last=input.begin();
        if (min>getFreeSpace()) return(0);
        if (min>input.size()*sizeof(input.back())) return(0);
        for (typename T::iterator it=input.begin();it!=input.end();++it){
            std::size_t next=out+sizeof(input.back());
            if (((max==0)||(next<=max))&&(testIn(input.back()))){
                out=next;
                dataIn(*it,false);
            } else {
                input.erase(input.begin(),last);
                return(out);
            }
            last=it;
        }
        input.clear();
        return(out);
    }
    template<class T> std::size_t dataOut_priv(T & output,const std::size_t & max=0,const std::size_t & min=0){
        std::size_t out=0;
        if (min>getSize()) return(0);
        if (min>(output.max_size()-output.size())*sizeof(output.back())) return(0);
        for (;;){
            std::size_t next=out+sizeof(output.back());
            if (((max==0)||(next<=max))&&(testOut(output.back()))){
                out=next;
                output.resize(output.size()+1);
                dataOut(output.back(),false);
            } else {
                return(out);
            }
        }
        return(out);
    }

    virtual void byteIn(const byte_t & byte)=0;
    virtual void byteOut(byte_t & byte)=0;
    virtual size_type getArraySize() const=0;
public:
    //! Patrz: ict::buffer::interface
    virtual std::size_t getMaxSize() const=0;
    //! Patrz: ict::buffer::interface
    virtual std::size_t getFreeSpace() const=0;
    //! Patrz: ict::buffer::interface
    virtual std::size_t getSize() const=0;
    //! Patrz: ict::buffer::interface 
    virtual void clear()=0;

    //! Patrz: ict::buffer::interface
    bool testIn(const signed char & input) const {return(!testInput(input));}
    bool testIn(const signed short int & input) const {return(!testInput(input));}
    bool testIn(const signed int & input) const {return(!testInput(input));}
    bool testIn(const signed long int & input) const {return(!testInput(input));}
    bool testIn(const signed long long int & input) const {return(!testInput(input));}
    bool testIn(const unsigned char & input) const {return(!testInput(input));}
    bool testIn(const unsigned short int & input) const {return(!testInput(input));}
    bool testIn(const unsigned int & input) const {return(!testInput(input));}
    bool testIn(const unsigned long int & input) const {return(!testInput(input));}
    bool testIn(const unsigned long long int & input) const {return(!testInput(input));}
    bool testIn(const float & input) const {return(!testInput(input));}
    bool testIn(const double & input) const {return(!testInput(input));}
    bool testIn(const long double & input) const {return(!testInput(input));}
    bool testIn(const bool & input) const {return(!testInput(input));}
    bool testIn(const std::string & input) const {return(!testInputArray(input));}
    bool testIn(const std::wstring & input) const {return(!testInputArray(input));}
    bool testIn(const byte_vector_t & input) const {return(!testInputArray(input));}

    //! Patrz: ict::buffer::interface
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

    //! Patrz: ict::buffer::interface
    std::size_t dataIn(std::string & input,const std::size_t & max=0,const std::size_t & min=0){return(dataIn_priv(input,max,min));}
    std::size_t dataIn(std::wstring & input,const std::size_t & max=0,const std::size_t & min=0){return(dataIn_priv(input,max,min));}
    std::size_t dataIn(byte_vector_t & input,const std::size_t & max=0,const std::size_t & min=0){return(dataIn_priv(input,max,min));}

    //! Patrz: ict::buffer::interface
    bool testOut(const signed char & output) const {return(!testOutput(output));}
    bool testOut(const signed short int & output) const {return(!testOutput(output));}
    bool testOut(const signed int & output) const {return(!testOutput(output));}
    bool testOut(const signed long int & output) const {return(!testOutput(output));}
    bool testOut(const signed long long int & output) const {return(!testOutput(output));}
    bool testOut(const unsigned char & output) const {return(!testOutput(output));}
    bool testOut(const unsigned short int & output) const {return(!testOutput(output));}
    bool testOut(const unsigned int & output) const {return(!testOutput(output));}
    bool testOut(const unsigned long int & output) const {return(!testOutput(output));}
    bool testOut(const unsigned long long int & output) const {return(!testOutput(output));}
    bool testOut(const float & output) const {return(!testOutput(output));}
    bool testOut(const double & output) const {return(!testOutput(output));}
    bool testOut(const long double & output) const {return(!testOutput(output));}
    bool testOut(const bool & output) const {return(!testOutput(output));}
    bool testOut(const std::string & output) const {return(!testOutputArray(output));}
    bool testOut(const std::wstring & output) const {return(!testOutputArray(output));}
    bool testOut(const byte_vector_t & output) const {return(!testOutputArray(output));}

    //! Patrz: ict::buffer::interface 
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

    //! Patrz: ict::buffer::interface
    std::size_t dataOut(std::string & output,const std::size_t & max=0,const std::size_t & min=0){return(dataOut_priv(output,max,min));}
    std::size_t dataOut(std::wstring & output,const std::size_t & max=0,const std::size_t & min=0){return(dataOut_priv(output,max,min));}
    std::size_t dataOut(byte_vector_t & output,const std::size_t & max=0,const std::size_t & min=0){return(dataOut_priv(output,max,min));}
};
//===========================================
//! 
//! @brief Bufor oparty na std::deque<byte_t>
//! 
class basic:public byte_interface {
private:
    std::size_t max;
    std::deque<byte_t> q;
    void byteIn(const byte_t & byte);
    void byteOut(byte_t & byte);
    size_type getArraySize() const;
public:
    //! 
    //! @brief Tworzy Bufor.
    //! 
    basic();
    //! 
    //! @brief Tworzy bufor i ustawia jego maksymalny rozmiar.
    //! 
    //! @param maxSize Maksymalny rozmiar bufora.
    //! 
    basic(const std::size_t & maxSize);
    //! Patrz: ict::buffer::interface
    std::size_t getMaxSize() const;
    //! Patrz: ict::buffer::interface
    std::size_t getFreeSpace() const;
    //! Patrz: ict::buffer::interface
    std::size_t getSize() const;
    //! Patrz: ict::buffer::interface
    void clear(){q.clear();}
};
//===========================================
} }
//===========================================
#endif