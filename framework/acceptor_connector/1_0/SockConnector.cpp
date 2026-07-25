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
#include "framework/common/ErrnoGuard.hpp"

namespace AcceptorConnector_1_0
{

SockConnector::SockConnector()
    : mHandle( 0 )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

SockConnector::~SockConnector()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

void SockConnector::setHandle( int handle )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    mHandle = handle;
}
int SockConnector::getHandle()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return mHandle;
}

int SockConnector::connect( SockStream &newStream, const PEER_ADDR &remoteAddr, const timeval *timeOut, int reuse_addr, int flags, int perms, int protocol )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    int result = 0;

    result = open( newStream, remoteAddr.sin_family, protocol, reuse_addr );
    if ( result != -1 )
    {
        result = connectStart( newStream, flags, timeOut );
    }

    if ( result != -1 )
    {
        result = ::connect( newStream.getHandle(),
                            (struct sockaddr *) &remoteAddr,
                            sizeof( remoteAddr ) );
    }
    return connectFinish( newStream, result, timeOut );
}

int SockConnector::open( SockStream &newStream, int protocolFamily, int protocol, int reuseAddr )
{
    int result = 0;
    LOG_INFO( __FUNCTION__ << "() " << "called" );

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

int SockConnector::connectStart( SockStream &newStream, int flags, const timeval *timeOut )
{
    if ( timeOut != nullptr )
    {
        if ( newStream.enable( O_NONBLOCK ) == -1 )
        {
            return -1;
        }
    }
    return 0;
}

int SockConnector::connectFinish( SockStream &new_stream, int result, const timeval *timeOut )
{
    LOG_INFO( " result="
              << result
              << ", errno="
              << errno );

    Framework_Common::ErrnoGuard error;

    int returnValue = result;

    if ( ( returnValue == -1 ) && ( timeOut != nullptr ) )
    {
        if ( error == EINPROGRESS || error == EWOULDBLOCK )
        {
            if ( ( timeOut->tv_sec == 0 ) &&
                 ( timeOut->tv_usec == 0 ) )
            {
                error = EWOULDBLOCK;
            }
            else if ( this->timedComplete( new_stream.getHandle(), timeOut ) == -1 )
            {
                error = errno;
                returnValue = -1;
            }
            else
            {
                returnValue = 0;
            }
        }
    }

    if ( returnValue != -1 || error == EISCONN )
    {
        // Start out with non-blocking disabled on the new_stream.
        returnValue = new_stream.disable( O_NONBLOCK );
        if ( returnValue == -1 )
        {
            new_stream.close();
        }
    }
    else if ( !( error == EWOULDBLOCK || error == ETIMEDOUT ) )
    {
        new_stream.close();
    }
    return returnValue;
}

int SockConnector::timedComplete( int handle, const timeval *timeOut )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    fd_set wset;
    FD_ZERO( &wset );
    FD_SET( handle, &wset );
    struct timeval timeValue = *timeOut;
    int result = ::select( handle + 1, nullptr, &wset, nullptr, &timeValue );

    LOG_INFO( "select result="
              << result );

    if ( ( result == 0 ) && ( timeOut != nullptr ) )
    {
        LOG_INFO( "Connection timeout expired, sec="
                  << timeOut->tv_sec
                  << "."
                  << timeOut->tv_usec );
        // Timeout expired.
        errno = ETIMEDOUT;
        return -1;
    }

    int soError = 0;
    socklen_t len = sizeof( soError );
    if ( ::getsockopt( handle, SOL_SOCKET, SO_ERROR, &soError, &len ) == -1 )
    {
        return -1;
    }

    LOG_INFO( "getsockopt socket error="
              << soError );

    if ( soError != 0 )
    {
        errno = soError;
        return -1;
    }

    return 0;
}

} // namespace AcceptorConnector_1_0
