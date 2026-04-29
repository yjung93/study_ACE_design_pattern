
#include "SyncService.hpp"
#include <sys/socket.h>
#include "framework/common/Logger.hpp"
SyncService::SyncService( int socketFd )
    : mSocketFd( socketFd )
{
    LOG_INFO( "called" );
}
SyncService::~SyncService()
{
    LOG_INFO( "called" );
}

int SyncService::open( void *args )
{
    LOG_INFO( "called" );

    return activate();
}
int SyncService::svc()
{
    LOG_INFO( "called" );

    string message;
    while ( getQ( message ) == 0 )
    {
        processMessage( message );
    }

    return 0;
}

void SyncService::processMessage( const std::string &message )
{
    LOG_INFO( "called" );

    string messageToSend = "Echo - " + message;
    send( mSocketFd, messageToSend.c_str(), messageToSend.size(), 0 );
}