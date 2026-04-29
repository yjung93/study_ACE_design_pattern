/*
 * SockConnector.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */
#include "SockConnector.hpp"

#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "framework/common/Logger.hpp"

namespace AcceptorConnector_1_0
{

SockConnector::SockConnector()
    : mHandle( 0 )
{
    LOG_INFO( "called" );
}

SockConnector::~SockConnector()
{
    LOG_INFO( "called" );
}

void SockConnector::setHandle( int handle )
{
    LOG_INFO( "called" );
    mHandle = handle;
}
int SockConnector::getHandle()
{
    LOG_INFO( "called" );
    return mHandle;
}

int SockConnector::connect( SockStream &newStream, const PEER_ADDR &remoteAddr, int reuse_addr, int flags, int perms, int protocol )
{
    LOG_INFO( "called" );

    int result = 0;

    result = open( newStream, remoteAddr.sin_family, protocol, reuse_addr );
    if ( result != -1 )
    {
        result = connectStart( newStream, flags );
    }

    if ( result != -1 )
    {
        result = ::connect( newStream.getHandle(),
                            (struct sockaddr *) &remoteAddr,
                            sizeof( remoteAddr ) );
    }
    return connectFinish( newStream, result );
}

int SockConnector::open( SockStream &newStream, int protocolFamily, int protocol, int reuseAddr )
{
    int result = 0;
    LOG_INFO( "called" );

    // Only open a new socket if we don't already have a valid handle.
    if ( ( newStream.getHandle() == INVALID_HANDLE ) && ( newStream.open( SOCK_STREAM, protocolFamily, protocol, reuseAddr ) == -1 ) )
    {
        result = -1;
    }
    else
    {
        result = 0;
    }

    return result;
}

int SockConnector::connectStart( SockStream &newStream, int flags )
{
    int result = 0;

    if ( ( flags & O_NONBLOCK ) != 0 )
    {
        newStream.enable( O_NONBLOCK );
    }
    return result;
}

int SockConnector::connectFinish( SockStream &new_stream, int result )
{
    int returnVaue = 0;
    returnVaue = result;

    return returnVaue;
}

} // namespace AcceptorConnector_1_0
