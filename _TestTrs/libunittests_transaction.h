// -----------------------------------------------------------------
// TestTrs  - a protocol test util
// Copyright (C) 2019  Gabriele Bonacini
//
// This program is free software for no profit use; you can redistribute
// it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
// A commercial license is also available for a lucrative use.
// ----------------------------------------------------------------

#pragma once

#include "libunittests_global.h"

#include <string>
#include <vector>

#include <netinet/in.h>

#include "common_wrapper.h"

namespace unittestigt{

using SockaddrIn=struct sockaddr_in;
using Sockaddr=struct sockaddr;
using Buffer= std::vector<uint8_t>;

enum ConstConf { MAX_RECV_SIZE_BUFF=2048 };

class LIBUNITTESTSSHARED_EXPORT Libunittests_transaction {
    public:
        Libunittests_transaction(const LMTransBase* trns,
                                 const std::string& addr=DEF_ENDPOINT_ADDR,
                                 unsigned short     prt =DEF_ENDPOINT_PORT)  noexcept;
        ~Libunittests_transaction(void)                                      noexcept;
        bool openSocket(void)                                                noexcept;
        void setIdentity( const std::string& receiving  =  DEF_REC,
                         char                term       =  DEF_TERM,
                         const std::string&  rel        =  DEF_REL,
                         const std::string&  card       =  DEF_CARD,
                         const std::string&  region     =  DEF_REGION)       noexcept;
        bool sendTransaction(void)                                           noexcept;
        void dumpHex(const std::string label, const Buffer* buff,
                     size_t maxlen)                                 const    noexcept;
        void dumpHex(const std::string label, const unsigned char* buff,
                     size_t buffsize, size_t maxlen)                const    noexcept;
        const Buffer&   getResponseBuffer(void)                     const    noexcept;
        size_t          getResponseLen(void)                        const    noexcept;
        unsigned char*  getResponseBufferOnlyTrans(void)                     noexcept;
        size_t          getResponseLenOnlyTrans(void)               const    noexcept;
        size_t          getHeaderLen(void)                          const    noexcept;
        size_t          getFooterLen(void)                          const    noexcept;

    private:
        std::string           address,
                              idReceivingOffice,
                              idRelease,
                              idCard,
                              idRegion;
        char                  idTerminal;
        unsigned short        port;
        SockaddrIn            sockaddrIn;
        int                   sockFd;

        Buffer                incoming;
        size_t                incomingLen;
        const TRANSNAME*      transaction;
        const Buffer          header,
                              footer;
};

} // End namespace
