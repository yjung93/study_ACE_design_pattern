/*
 * AsyncService.cpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#include "applications/example_half-sync_half-async/AsyncService.hpp"
#include "applications/example_half-sync_half-async/Acceptor.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "framework/common/Logger.hpp"

namespace ExHalfSyncAsync
{

AsyncService::AsyncService( Reactor_1_0::Reactor *reactor, Acceptor &owner )
    : EventHandler( reactor ),
      mOwner( &owner )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

AsyncService::~AsyncService()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

void AsyncService::open()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    getReactor()->registerHandler( this, EventHandler::READ_MASK );

    // Initialize SyncService
    mSyncService.reset( new SyncService( getHandle() ) );
    mSyncService->open();
}

int AsyncService::handleInput( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

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

    mSyncService->putQ( string( buffer, static_cast<size_t>( valread ) ) );

    return 0;
}

} /* namespace  ExHalfSyncAsync */
