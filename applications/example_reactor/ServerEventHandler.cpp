/*
 * ServerEventHandler.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#include "applications/example_reactor/ServerEventHandler.hpp"
#include <cstring>
#include "framework/common/Logger.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

namespace example_reactor
{

ServerEventHandler::ServerEventHandler( Reactor_1_0::Reactor *reactor ) :
                EventHandler( reactor )
{
    LOG_INFO( "called" );
}

ServerEventHandler::~ServerEventHandler()
{
    LOG_INFO( "called" );
}

void ServerEventHandler::open()
{
    LOG_INFO( "called" );
    getReactor()->registerHandler( this, EventHandler::READ_MASK );
}

int ServerEventHandler::handleInput( int fd )
{
    LOG_INFO( "called" );

    const int bufferSize = 1024;

    char buffer[bufferSize] =
    { 0 };

    int valread = read( fd, buffer, bufferSize );
    if ( valread == 0 )
    {
        // Client disconnected
        LOG_INFO( "client disconnected, socket FD: " << fd );
        close( fd );
        getReactor()->removeHandler( this, ALL_EVENTS_MASK );
    }else
    {
        // Echo the message back to client
        LOG_INFO( "received message: " << buffer );

        string messageToSend = "Echo - " + string( buffer );
        send( fd, messageToSend.c_str(), messageToSend.size(), 0 );

        LOG_INFO( "replied message: " << messageToSend );
    }

    return 0;
}

} /* namespace example_reactor */
