/*
 * ServerEventHandler.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "ServerEventHandler.hpp"
#include "Acceptor.hpp"
#include "framework/common/Logger.hpp"

namespace example_proactor
{

ServerEventHandler::ServerEventHandler( Acceptor &owner )
    : mOwner( &owner ),
      mMessageRead( 1024 ),
      mMessageWrite( 0 )

{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

ServerEventHandler::~ServerEventHandler()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    close( handle() );
}

void ServerEventHandler::open( int new_handle )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    mReader.open( *this, new_handle, 0, proactor() );
    mWriter.open( *this, new_handle, 0, proactor() );

    mReader.read( mMessageRead, mMessageRead.size() );
}

void ServerEventHandler::handleReadStream( const Proactor_1_0::AsynchReadStreamResult &result )
{
    if ( result.success() != true || result.bytes_transferred() == 0 )
    {
        delete this;
    }
    else if ( result.bytes_transferred() > 0 )
    {
        string messageReceived( result.message().begin(), result.message().begin() + result.bytes_transferred() );
        LOG_INFO( "received=" << messageReceived );

        string messageToSend = "Echo - " + string( messageReceived );
        mMessageWrite.assign( messageToSend.begin(), messageToSend.end() );
        mWriter.write( mMessageWrite, mMessageWrite.size() );

        LOG_INFO( "replied message: " << messageToSend );

        mReader.read( mMessageRead, mMessageRead.size() );
    }
}

void ServerEventHandler::handleWriteStream( const Proactor_1_0::AsynchWriteStreamResult &result )
{
    if ( result.success() != true || result.bytes_transferred() == 0 )
    {
        LOG_ERROR( "fail to send" );
    }
    else if ( result.bytes_transferred() > 0 )
    {
        string messageReceived( result.message().begin(), result.message().begin() + result.bytes_transferred() );
        LOG_INFO( "sent=" << messageReceived );
    }
}
} /* namespace example_proactor */
