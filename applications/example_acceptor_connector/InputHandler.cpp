/*
 * InputHandler.cpp
 *
 *  Created on: Jan 26, 2025
 *      Author: yjung93
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "InputHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"
#include "framework/common/Logger.hpp"

namespace ex_acceptor_connector
{

InputHandler::InputHandler()
    : ServiceHandler( Reactor_1_0::Reactor::getInstance() )
{
    LOG_INFO( "called" );
}

InputHandler::~InputHandler()
{
    LOG_INFO( "called" );
}

int InputHandler::handleInput( int fd )
{
    LOG_INFO( "called" );

    const int bufferSize = 1024;

    char buffer[bufferSize] = { 0 };

    int valread = peer().recv_n( buffer, bufferSize, 0 );
    if ( valread == 0 )
    {
        // Client disconnected
        LOG_INFO( "client disconnected, socket FD: " << fd );
        peer().close_reader();
        getReactor()->removeHandler( this, ALL_EVENTS_MASK );
    }
    else if ( valread < 0 )
    {
        perror( "recv failed" );
    }
    else
    {
        // Echo the message back to client
        LOG_INFO( "received message: " << buffer );

        string messageToSend = "Echo - " + string( buffer );
        peer().send_n( messageToSend.c_str(), messageToSend.size(), 0 );
        LOG_INFO( "replied message: " << messageToSend );
    }

    return 0;
}

int InputHandler::close()
{
    int result = 0;
    LOG_INFO( "called" );
    return result;
}

} /* namespace ex_acceptor_connector */
