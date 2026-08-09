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
#include "framework/stream/1_0/StreamThrough.hpp"
#include "ServerEventHandler.hpp"

#include "StreamModule.hpp"
#include "ResponseMessage.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"
#include "MetaData.hpp"
#include "DataType.hpp"
#include "Acceptor.hpp"

using namespace std;

namespace example_stream
{

ServerEventHandler::ServerEventHandler( Acceptor &owner, Reactor_1_0::Reactor *reactor )
    : EventHandler( reactor ),
      mBottomModule( nullptr ),
      mOwner( &owner )
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

    mBottomModule = new Stream_1_0::Module( "Network Layer", new Sender( this, getHandle() ), new Receiver() );

    mStream.push( mBottomModule );
    mStream.push( new Stream_1_0::Module( "MetaData Layer", new MetaData(), new Stream_1_0::StreamThrough() ) );
    mStream.push( new Stream_1_0::Module( "Application Layer", new Stream_1_0::StreamThrough(), new ResponseMessage() ) );
}
void ServerEventHandler::close()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    mOwner->removeConnection( getHandle() );
}

int ServerEventHandler::handleInput( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called ---------------------" );

    const int bufferSize = 1024;

    char buffer[bufferSize] = { 0 };

    int valread = read( fd, buffer, bufferSize );
    if ( valread <= 0 )
    {
        // Client disconnected
        LOG_INFO( "client disconnected, socket FD: " << fd );

        ::close( fd );
        getReactor()->removeHandler( this, ALL_EVENTS_MASK );

        // send stop to modules
        map<string, string> data;
        data = {
            { Stream_1_0::MessageType, Stream_1_0::Stop }
        };

        // to up-stream
        string message = Framework_Common::Utils::formatKeyValue( data );
        if ( mBottomModule != nullptr )
        {
            mBottomModule->getReader()->put( message );
        }

        // to down-stream
        mStream.put( message );

        sleep( 1 ); // Wait until module cleaned up the thread and etc. not a pretty design, but simple and easy.

        close();// let this instance be deleted
        return 0;
    }
    else
    {
        // Echo the message back to client
        LOG_INFO( "received message: " << buffer << "valread=" << valread );

        // TCP may merge or split sends, so buffer bytes and only dispatch complete '\n'-terminated messages
        mRecvBuffer.append( buffer, valread );

        size_t delimiterPos;
        while ( ( delimiterPos = mRecvBuffer.find( '\n' ) ) != string::npos )
        {
            string message = mRecvBuffer.substr( 0, delimiterPos );
            mRecvBuffer.erase( 0, delimiterPos + 1 );

            if ( mBottomModule != nullptr && !message.empty() )
            {
                mBottomModule->getReader()->put( message );
            }
        }
    }

    return 0;
}

} /* namespace example_stream */
