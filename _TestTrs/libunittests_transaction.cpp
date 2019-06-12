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

#include "libunittests_transaction.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/uio.h>
#include <errno.h>

#include <iostream>
#include <iomanip>

namespace unittestigt{

using std::string;
using std::cerr;
using std::setfill;
using std::setw;
using std::dec;
using std::hex;
using std::endl;
using std::vector;
using Iovect=struct iovec;

Libunittests_transaction::Libunittests_transaction(const TRANSNAME* trns, const string& addr, unsigned short prt) noexcept
    :  address{addr},
       port{prt},
       sockaddrIn{},
       sockFd{-1},
       incoming(MAX_RECV_SIZE_BUFF, 0),
       incomingLen{0},
       transaction{trns},
       header{'@', '@', '@'},
       footer{'%', '%', '%'}
{
   sockaddrIn.sin_family      = AF_INET;
   sockaddrIn.sin_port        = htons(port);
   sockaddrIn.sin_addr.s_addr = inet_addr(address.c_str());
}

Libunittests_transaction::~Libunittests_transaction(void) noexcept{
    close(sockFd);
}

bool Libunittests_transaction::openSocket(void) noexcept{
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockFd < 0 ){
        cerr << "Libunittests_transaction::openSocket -> can't create socket fd\n";
        cerr << "Libunittests_transaction::openSocket " << strerror(errno) << "\n";
        return false;
    }

    int retco { connect(sockFd, reinterpret_cast<Sockaddr*>(&sockaddrIn), sizeof(sockaddrIn)) };
    if(retco < 0 ){
        cerr << "Libunittests_transaction::openSocket -> can't connect the socket \n";
        cerr << "Libunittests_transaction::openSocket " << strerror(errno) << "\n";
        return false;
    }

    return true;
}

void Libunittests_transaction::setIdentity(const string& receiving,  char term,           const string& rel,
                                           const string& card,      const string& region)     noexcept{
    idReceivingOffice  = receiving;
    idRelease          = rel;
    idCard             = card;
    idRegion           = region;
    idTerminal         = term;

    cerr << "Libunittests_transaction::setIdentity : Rec: " << idReceivingOffice
         << " release: " << idRelease << " card: " << idCard
         << " region: " << idRegion << " terminal: " << idTerminal << "\n";

    transaction->SETHEADERTX(idReceivingOffice.c_str(), idTerminal, idRelease.c_str(),
                             idCard.c_str(), idRegion.c_str());
}

bool Libunittests_transaction::sendTransaction(void) noexcept{
    dumpHex("Request:", reinterpret_cast<const unsigned char*>(transaction->GETTXDATA()),
                                                               transaction->GETTXDATASIZE(),
                                                               transaction->GETTXDATASIZE());
    unsigned short len{htons(transaction->GETTXDATASIZE())};
    cerr << "Libunittests_transaction::sendTransaction -> header size: " << header.size() << "\n";
    cerr << "Libunittests_transaction::sendTransaction -> data size: "   << transaction->GETTXDATASIZE() << "\n";
    cerr << "Libunittests_transaction::sendTransaction -> footer size: " << footer.size() << "\n";

    vector<Iovect>  iovect(4);
    cerr << "Libunittests_transaction::sendTransaction -> iovect elems: " << iovect.size() << "\n";
    iovect[0].iov_base = const_cast<void*>(static_cast<const void*>(header.data()));
    iovect[0].iov_len  = header.size();
    iovect[1].iov_base = const_cast<void*>(static_cast<const void*>(&len));
    iovect[1].iov_len  = sizeof(unsigned short);
    iovect[2].iov_base = const_cast<void*>(static_cast<const void*>(transaction->getTxbuf().constData()));
    iovect[2].iov_len  = transaction->GETTXDATASIZE();
    iovect[3].iov_base = const_cast<void*>(static_cast<const void*>(footer.data()));
    iovect[3].iov_len  = footer.size();

    dumpHex("Request:", reinterpret_cast<const unsigned char*>(transaction->getTxbuf().constData()),
                                                               transaction->GETTXDATASIZE(),
                                                               transaction->GETTXDATASIZE());

    cerr << "Libunittests_transaction::sendTransaction -> sending request \n";
    ssize_t ret { writev(sockFd, iovect.data(), iovect.size()) };

    if(ret <= 0 ){
        cerr << "Libunittests_transaction::sendTransaction -> error sending req: " << ret << "\n";
        cerr << "Libunittests_transaction::sendTransaction -> " << strerror(errno) << "\n";
        return false;
    }

    cerr << "Libunittests_transaction::sendTransaction -> reading data response\n";
    ret = read(sockFd, incoming.data(), incoming.size());
    if(ret < 0 ){
        cerr << "Libunittests_transaction::sendTransaction -> error receiving transaction: " << ret << "\n";
        cerr << "Libunittests_transaction::sendTransaction -> " << strerror(errno) << "\n";
        return false;
    }
    if(ret == 0 ){
        cerr << "Libunittests_transaction::sendTransaction -> error receiving transaction: connection closed by peer \n";
        return false;
    }

    incomingLen = ret;

    dumpHex("Response:", &incoming, ret);

    return true;
}

size_t   Libunittests_transaction::getHeaderLen(void)  const noexcept{
      return header.size() + sizeof(unsigned short);
}

size_t   Libunittests_transaction::getFooterLen(void)  const noexcept{
      return footer.size();
}

const Buffer& Libunittests_transaction::getResponseBuffer(void) const noexcept{
     return incoming;
}

size_t  Libunittests_transaction::getResponseLen(void) const  noexcept{
    return incomingLen;
}

unsigned char* Libunittests_transaction::getResponseBufferOnlyTrans(void)  noexcept{
     // Necessario perche' il relativo parametro della decode non e' const:
     // controllare se e' possibile modificare la lib
     return const_cast<unsigned char*>(incoming.data() + getHeaderLen());
}

size_t   Libunittests_transaction::getResponseLenOnlyTrans(void)  const    noexcept{
     return getResponseLen() - getHeaderLen() - getFooterLen();
}

void  Libunittests_transaction::dumpHex(string label, const Buffer* buff, size_t maxlen) const noexcept{
     cerr << label << endl << endl;

     size_t len    = maxlen ? maxlen : buff->size();
     for (size_t i = 0; i < len; i += 16) {
        cerr << setfill('0') << setw(5) << dec << i << ":  ";
        for (size_t j = i; j < i + 16; j++) {
           if(j < len)
              cerr << setfill('0') << setw(2) << hex
                   << static_cast<int>(buff->at(j)) << " ";
           else cerr << "   ";
        }
        cerr << " ";
        for (size_t j = i; j < i + 16; j++) {
           if(j < len){
              if((buff->at(j) > 31) && (buff->at(j) < 128) && (buff->at(j) != 127))
                 cerr << buff->at(j) ;
              else cerr << "." ;
           }
        }
        cerr << endl;
     }
     cerr << endl << endl;
}

void  Libunittests_transaction::dumpHex(string label, const unsigned char* buff, size_t buffsize, size_t maxlen) const noexcept{
     cerr << label << endl << endl;

     size_t len    = maxlen ? maxlen : buffsize;
     for (size_t i = 0; i < len; i += 16) {
        cerr << setfill('0') << setw(5) << dec << i << ":  ";
        for (size_t j = i; j < i + 16; j++) {
           if(j < len)
              cerr << setfill('0') << setw(2) << hex
                   << static_cast<int>(buff[j]) << " ";
           else cerr << "   ";
        }
        cerr << " ";
        for (size_t j = i; j < i + 16; j++) {
           if(j < len){
              if((buff[j] > 31) && (buff[j] < 128) && (buff[j] != 127))
                 cerr << buff[j] ;
              else cerr << "." ;
           }
        }
        cerr << endl;
     }
     cerr << endl << endl;
}

} // End Namespace
