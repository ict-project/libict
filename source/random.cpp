//! @file
//! @brief Random module - Source file.
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
#include "random.hpp"
//============================================
#ifdef ENABLE_TESTING
#include "test.hpp"
#endif
//============================================
namespace ict { namespace random {
//===========================================
std::string randomString(std::size_t size,const std::string & map){
  std::string out;
  std::size_t max=map.size();
  if (!max) return(out);
  max--;
  for (std::size_t k=0;k<size;k++){
     out+=map.at(randomInteger<std::size_t>(0,max));
  }
  return(out);
}
//===========================================
} }
//===========================================
#ifdef ENABLE_TESTING
REGISTER_TEST(random,tc1){
  std::cout<<ict::random::randomString(100)<<std::endl;
  return(0);
}
#endif
//===========================================
