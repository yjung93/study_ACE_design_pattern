
#include "SyncService.hpp"
#include <sys/socket.h>
#include "framework/common/Logger.hpp"
SyncService::SyncService( int socketFd )
    : mSocketFd( socketFd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}
SyncService::~SyncService()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

int SyncService::open( void *args )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    return activate();
}
int SyncService::svc()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    string message;
    while ( getQ( message ) == 0 )
    {
        processMessage( message );
    }

    return 0;
}

void SyncService::processMessage( const std::string &message )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    string messageToSend = "Echo - " + message;
    send( mSocketFd, messageToSend.c_str(), messageToSend.size(), 0 );
}