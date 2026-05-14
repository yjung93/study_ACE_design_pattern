/*
 * SockConnector.hpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SockConnector_HPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SockConnector_HPP_

#include <arpa/inet.h>

#include "framework/acceptor_connector/1_0/SockStream.hpp"

namespace AcceptorConnector_1_0
{

class SockConnector
{
  public:
    typedef struct sockaddr_in PEER_ADDR;

    SockConnector();
    virtual ~SockConnector();

    int connect( SockStream &new_stream, const PEER_ADDR &remoteAddr, const timeval *timeOut = 0, int reuse_addr = 0, int flags = 0, int perms = 0, int protocol = 0 );
    void setHandle( int handle );
    int getHandle();

  protected:
    int open( SockStream &newStream, int protocolFamily, int protocol, int reuseAddr );

    int connectStart( SockStream &new_stream, int flags, const timeval *timeOut );

    int connectFinish( SockStream &new_stream, int result, const timeval *timeOut );
    int timedComplete( int handler, const timeval *timeOut );

  private:
    PEER_ADDR mAddress;
    int mHandle;
};

} // namespace AcceptorConnector_1_0

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SockConnector_HPP_ */
