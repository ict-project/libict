//! @file
//! @brief OS module - header file.
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
#ifndef _OS_HEADER
#define _OS_HEADER
//============================================
#include <string>
#include <vector>
//============================================
namespace ict { namespace os {
//===========================================
//! The type is unknown. Only some filesystems have full support to return the type of the file, others might always return this value.
extern const unsigned char dt_UNKNOWN;
//! A regular file.
extern const unsigned char dt_REG;
//A directory.
extern const unsigned char dt_DIR;
//A named pipe, or FIFO. See FIFO Special Files.
extern const unsigned char dt_FIFO;
//A local-domain socket.
extern const unsigned char dt_SOCK;
//A character device.
extern const unsigned char dt_CHR;
//A block device.
extern const unsigned char dt_BLK;
//A symbolic link.
extern const unsigned char dt_LNK;
//===========================================
std::string getCurrentDir();
std::string getOnlyDir(const std::string & path);
bool isFileReadable(const std::string & base,const std::string & path);
bool isFileReadable(const std::string & path);
std::string getRealPath(const std::string & base,const std::string & path);
std::string getRealPath(const std::string & path);
std::string getRelativePath(const std::string & base,const std::string & path);
std::string getRelativePath(const std::string & path);
std::vector<std::string> getDirList(const std::string & dir, unsigned char dt=dt_UNKNOWN);
bool removeFile(const std::string & path);
bool removeDir(const std::string & path);
bool removeDirAll(const std::string & path);
//============================================
}}
//===========================================
#endif