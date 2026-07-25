#include <sys/socket.h>
#include <netinet/in.h>

#include "NotifyPipeManager.hpp"
#include "Proactor.hpp"
#include "framework/common/Logger.hpp"

namespace Proactor_1_0
{

NotifyPipeManager::NotifyPipeManager( Proactor *proactor )
    : mProactor( proactor ),
      mNotifyBuffer( 128 )
{
    if ( ::pipe( mNotifyPipe ) == -1 )
    {
        perror( "pipe failed" );
        return;
    }

    LOG_INFO( "mNotifyPipe[0]=" << mNotifyPipe[0] << ", mNotifyPipe[1]=" << mNotifyPipe[1] );

    mProactor->setNotifyHandle( mNotifyPipe[0] );
    mReadStream.open( *this, mNotifyPipe[0], 0, mProactor );
    mReadStream.read( mNotifyBuffer, mNotifyBuffer.size() );
}

NotifyPipeManager::~NotifyPipeManager()
{
}

void NotifyPipeManager::handleReadStream( const AsynchReadStreamResult &result )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    mReadStream.read( mNotifyBuffer, mNotifyBuffer.size() );
}

int NotifyPipeManager::notify()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );


    char byte = 1;
    ::write( mNotifyPipe[1], &byte, 1 );
    return 0;
}
} // namespace Proactor_1_0