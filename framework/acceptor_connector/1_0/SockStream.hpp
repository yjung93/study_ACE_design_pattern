/*
 * SockStream.hpp
 *
 *  Created on: May 9, 2025
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SOCKSTREAM_HPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SOCKSTREAM_HPP_

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "framework/acceptor_connector/1_0/Config.hpp"

namespace AcceptorConnector_1_0
{

class SockStream
{
public:
    SockStream();
    virtual ~SockStream();

    void setHandle( int handle );
    int getHandle() const;

    ssize_t recv_n( void *buf, size_t size, int flags );
    ssize_t send_n( const void *buf, size_t size, int flags );
    int close_reader();
    int close_writer();
    int close();
    int open( int type, int protocolFamily, int protocol, int reuseAddr );
    int enable( int value );
    int disable( int value );
    int getRemoteAddr( sockaddr_in &socketAddr ) const;

    typedef struct sockaddr_in PEER_ADDR;

private:
    int mHandle;
};

} /* namespace AcceptorConnector_1_0 */

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_SOCKSTREAM_HPP_ */
