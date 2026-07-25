/*
 * ServerEventHandler.cpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "ServerEventHandler.hpp"

#include "StreamModule.hpp"
#include "ResponseMessage.hpp"
#include "TimeInfo.hpp"
#include "ServerInfo.hpp"
#include "Sender.hpp"
#include "DataType.hpp"

using namespace std;

namespace example_stream
{

ServerEventHandler::ServerEventHandler( Reactor_1_0::Reactor *reactor )
    : EventHandler( reactor )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

ServerEventHandler::~ServerEventHandler()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

void ServerEventHandler::open()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    getReactor()->registerHandler( this, EventHandler::READ_MASK );

    mStream.open();

    mStream.push( new StreamModule<Sender>( "Sender", new Sender( getHandle() ) ) );
    mStream.push( new StreamModule<ServerInfo>( "ServerInfo", new ServerInfo() ) );
    mStream.push( new StreamModule<TimeInfo>( "TimeInfo", new TimeInfo() ) );
    mStream.push( new StreamModule<ResponseMessage>( "ResponseMessage", new ResponseMessage() ) );
}

int ServerEventHandler::handleInput( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    const int bufferSize = 1024;

    char buffer[bufferSize] = { 0 };

    int valread = read( fd, buffer, bufferSize );
    if ( valread == 0 )
    {
        // Client disconnected
        LOG_INFO( "client disconnected, socket FD: " << fd );
        close( fd );
        getReactor()->removeHandler( this, ALL_EVENTS_MASK );
    }
    else
    {
        // Echo the message back to client
        LOG_INFO( "received message: " << buffer );

        // string messageToSend = "Echo - " + string( buffer );
        // send( fd, messageToSend.c_str(), messageToSend.size(), 0 );

        // LOG_INFO( "replied message: " << messageToSend );

        map<string, string> data = {
            { Stream_1_0::MessageType, Stream_1_0::VendorData },
            { RecievedMessage, buffer },
            { ReplyMessage, "" }
        };

        string message = Framework_Common::Utils::formatKeyValue( data );
        mStream.put( message );
    }

    return 0;
}

} /* namespace example_stream */
