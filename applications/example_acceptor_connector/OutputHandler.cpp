/*
 * OutputHandler.cpp
 *
 *  Created on: Jan 26, 2025
 *      Author: yjung93
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "OutputHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"
#include "framework/common/Logger.hpp"

namespace ex_acceptor_connector
{

OutputHandler::OutputHandler() :
                ServiceHandler( Reactor_1_0::Reactor::getInstance() )
{
    LOG_INFO( "called" );
}

OutputHandler::~OutputHandler()
{
    LOG_INFO( "called" );
}

int OutputHandler::handleInput( int fd )
{
    LOG_INFO( "called" );

    const int bufferSize = 1024;

    char buffer[bufferSize] =
    { 0 };

    int valread = peer().recv_n( buffer, bufferSize, 0 );
    if ( valread == 0 )
    {
        // Client disconnected
        LOG_INFO( "client disconnected, socket FD: " << fd );
        peer().close_reader();
        getReactor()->removeHandler( this, ALL_EVENTS_MASK );
    }else if ( valread < 0 )
    {
        perror( "recv failed" );
    }else
    {
        // Echo the message back to client
        LOG_INFO( "received message: " << buffer );

    }

    return 0;
}

int OutputHandler::handleOutput( int fd )
{
    LOG_INFO( "called" );

    return 0;
}

int OutputHandler::close()
{
    int result = 0;
    LOG_INFO( "called" );
    return result;
}

void OutputHandler::sendMessage( string message )
{
    peer().send_n( message.c_str(), message.size(), 0 );
}

} /* namespace ex_acceptor_connector */

