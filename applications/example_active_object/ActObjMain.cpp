
#include "applications/example_active_object/ActObjMain.hpp"
#include "applications/example_active_object/ActObjAcceptor.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{

ActObjMain::ActObjMain( Reactor_1_0::Reactor *reactor, ActObjAcceptor &owner )
    : EventHandler( reactor ),
      mOwner( &owner )
{
    LOG_INFO( "called" );
}

ActObjMain::~ActObjMain()
{
    LOG_INFO( "called" );
}

void ActObjMain::open()
{
    LOG_INFO( "called" );
    getReactor()->registerHandler( this, EventHandler::READ_MASK );

    // Initialize ActObjClient
    mActObjClient.reset( new ActObjClient( getHandle() ) );
    mActObjClient->open();
}

int ActObjMain::handleInput( int fd )
{
    LOG_INFO( "called" );

    const int bufferSize = 1024;

    char buffer[bufferSize] = { 0 };

    int valread = read( fd, buffer, bufferSize );
    if ( valread <= 0 )
    {
        if ( valread == 0 )
        {
            LOG_INFO( "client disconnected, socket FD: " << fd );
        }
        else
        {
            perror( "read failed" );
        }

        close( fd );
        getReactor()->removeHandler( this, ALL_EVENTS_MASK );
        if ( mOwner != nullptr )
        {
            mOwner->removeConnection( fd );
        }
        // return here immediately to avoid further processing since this object is likely destroyed.
        return 0;
    }

    LOG_INFO( "received message: " << buffer );

    mActObjClient->putQ( string( buffer, static_cast<size_t>( valread ) ) );

    return 0;
}

} /* namespace  ExActiveObject */
