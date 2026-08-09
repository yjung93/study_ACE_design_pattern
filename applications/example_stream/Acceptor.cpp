/*
 * Acceptor.cpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */
#include "Acceptor.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include "framework/common/Logger.hpp"

namespace example_stream
{

constexpr int PORT = 8080;
constexpr int BACKLOG = 5;

Acceptor::Acceptor( Reactor_1_0::Reactor *reactor )
    : EventHandler( reactor )
{
    mAddress.sin_family = AF_INET;
    mAddress.sin_addr.s_addr = INADDR_ANY;
    mAddress.sin_port = htons( PORT );

    open();
}

Acceptor::~Acceptor()
{
    // TODO Auto-generated destructor stub
}

void Acceptor::open()
{
    int acceptorFd;
    int opt = 1;

    if ( ( acceptorFd = socket( AF_INET, SOCK_STREAM, 0 ) ) == 0 )
    {
        perror( "socket failed" );
        exit( EXIT_FAILURE );
    }

    setHandle( acceptorFd );

    // Set socket options
    if ( setsockopt( acceptorFd, SOL_SOCKET, ( SO_REUSEADDR | SO_REUSEPORT ), &opt, sizeof( opt ) ) == -1 )
    {
        perror( "setsockopt failed" );
        close( acceptorFd );
        exit( EXIT_FAILURE );
    }

    // Bind the socket to the network address and port
    if ( bind( acceptorFd, (struct sockaddr *) &mAddress, sizeof( mAddress ) ) < 0 )
    {
        perror( "bind failed" );
        close( acceptorFd );
        exit( EXIT_FAILURE );
    }

    // Listen for incoming connections
    if ( listen( acceptorFd, BACKLOG ) < 0 )
    {
        perror( "listen failed" );
        close( acceptorFd );
        exit( EXIT_FAILURE );
    }

    LOG_INFO( "server listening on port " << PORT );

    getReactor()->registerHandler( this, EventHandler::ACCEPT_MASK );
}

int Acceptor::handleInput( int fd )
{
    LOG_INFO( "Acceptor::" << __FUNCTION__ << "() " << "called" );

    int addrlen = sizeof( mAddress );
    int newSocketFd = accept( fd, (struct sockaddr *) &mAddress, (socklen_t *) &( addrlen ) );

    if ( newSocketFd < 0 )
    {
        perror( "accept failed" );
        exit( EXIT_FAILURE );
    }

    std::unique_ptr<ServerEventHandler> serverEventHandler( new ServerEventHandler( *this, getReactor() ) );

    serverEventHandler->setHandle( newSocketFd );
    serverEventHandler->open();
    auto rc = mConnections.emplace( newSocketFd, std::move( serverEventHandler ) );
    if ( rc.second == false )
    {
        LOG_INFO( "Acceptor::" << __FUNCTION__ << "() " << "failed to embplace to container. FD: " << newSocketFd );
        return -1;
    }

    LOG_INFO( "Acceptor::" << __FUNCTION__ << "() " << "new connection established, socket FD: " << newSocketFd );
    return 0;
}

void Acceptor::removeConnection( int fd )
{
    auto it = mConnections.find( fd );
    if ( it != mConnections.end() )
    {
        mConnections.erase( it );
        LOG_INFO( "connection cleaned up, socket FD: " << fd );
    }
}

} /* namespace example_stream */
