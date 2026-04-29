/*
 * SockAcceptor.hpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SOCKACCEPTOR_HPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SOCKACCEPTOR_HPP_

#include <arpa/inet.h>

#include "framework/acceptor_connector/1_0/SockStream.hpp"

namespace AcceptorConnector_1_0
{

class SockAcceptor
{
  public:
    typedef struct sockaddr_in PEER_ADDR;

    SockAcceptor();
    virtual ~SockAcceptor();

    int open( PEER_ADDR peerAddr );
    int accept( SockStream &newStream, int fd );
    void setHandle( int handle );
    int getHandle();

  private:
    PEER_ADDR mAddress;
    int mHandle;
};

} // namespace AcceptorConnector_1_0

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SOCKACCEPTOR_HPP_ */
