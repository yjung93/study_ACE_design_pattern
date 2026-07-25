
#include "applications/example_active_object/ActObjAcceptor.hpp"
#include "applications/example_active_object/ActObjMain.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{

constexpr int PORT = 8080;
constexpr int BACKLOG = 5;

ActObjAcceptor::ActObjAcceptor( Reactor_1_0::Reactor *reactor )
    : EventHandler( reactor )
{
    mAddress.sin_family = AF_INET;
    mAddress.sin_addr.s_addr = INADDR_ANY;
    mAddress.sin_port = htons( PORT );

    open();
}

ActObjAcceptor::~ActObjAcceptor()
{
    // TODO Auto-generated destructor stub
}

void ActObjAcceptor::open()
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

int ActObjAcceptor::handleInput( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    int addrlen = sizeof( mAddress );
    int newSocketFd = accept( fd, (struct sockaddr *) &mAddress, (socklen_t *) &( addrlen ) );

    if ( newSocketFd < 0 )
    {
        perror( "accept failed" );
        exit( EXIT_FAILURE );
    }

    std::unique_ptr<ExActiveObject::ActObjMain> handler( new ExActiveObject::ActObjMain( getReactor(), *this ) );
    handler->setHandle( newSocketFd );
    handler->open();
    
    mConnections.emplace( newSocketFd, std::move( handler ) );

    LOG_INFO( "new connection established, socket FD: " << newSocketFd );
    return 0;
}

void ActObjAcceptor::removeConnection( int fd )
{
    auto it = mConnections.find( fd );
    if ( it != mConnections.end() )
    {
        mConnections.erase( it );
        LOG_INFO( "connection cleaned up, socket FD: " << fd );
    }
}

} /* namespace  ExActiveObject */
