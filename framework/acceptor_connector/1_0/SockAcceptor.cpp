/*
 * SockAcceptor.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */
#include "SockAcceptor.hpp"

#include <arpa/inet.h>
#include <unistd.h>
#include "framework/common/Logger.hpp"

namespace AcceptorConnector_1_0
{



SockAcceptor::SockAcceptor()
    : mHandle( 0 )
{
    LOG_INFO( "called" );
}

SockAcceptor::~SockAcceptor()
{
    LOG_INFO( "called" );
}

int SockAcceptor::open( PEER_ADDR peerAddr )
{
    LOG_INFO( "called" );

    int acceptorFd;
    int opt = 1;
    int result = 0;

    mAddress = peerAddr;

    if ( result != -1 && ( acceptorFd = socket( AF_INET, SOCK_STREAM, 0 ) ) == 0 )
    {
        LOG_ERROR( "socket failed" );

        result = -1;
    }

    // Set socket options
    if ( result != -1 && setsockopt( acceptorFd,
                                     SOL_SOCKET,
                                     ( SO_REUSEADDR | SO_REUSEPORT ),
                                     &opt,
                                     sizeof( opt ) ) == -1 )
    {
        LOG_ERROR( "setsockopt failed" );

        close( acceptorFd );
        result = -1;
    }

    // Bind the socket to the network address and port
    if ( result != -1 && bind( acceptorFd, (struct sockaddr *) &mAddress, sizeof( mAddress ) ) < 0 )
    {
        LOG_ERROR( "bind failed" );

        close( acceptorFd );
        result = -1;
    }

    // Listen for incoming connections
    if ( result != -1 && listen( acceptorFd, BACKLOG ) < 0 )
    {
        LOG_ERROR( "listen failed" );

        close( acceptorFd );
        result = -1;
    }

    if ( result != -1 )
    {
        setHandle( acceptorFd );

        LOG_INFO( "server listening on port " << ntohs( mAddress.sin_port )
                  << " acceptorFd=" << acceptorFd );
    }

    return result;
}

int SockAcceptor::accept( SockStream &newStream, int fd )
{
    LOG_INFO( "fd=" << fd );

    socklen_t addrlen = sizeof( mAddress );
    int result = 0;

    int newSocketFd = ::accept( fd, (struct sockaddr *) &mAddress, &addrlen );
    if ( newSocketFd < 0 )
    {
        LOG_ERROR( "accept failed, fd=" << fd );
        result = -1;
    }
    else
    {
        newStream.setHandle( newSocketFd );
    }

    return ( result != -1 ) ? newSocketFd : result;
}

void SockAcceptor::setHandle( int handle )
{
    LOG_INFO( "called" );
    mHandle = handle;
}
int SockAcceptor::getHandle()
{
    LOG_INFO( "called" );
    return mHandle;
}

} // namespace AcceptorConnector_1_0
