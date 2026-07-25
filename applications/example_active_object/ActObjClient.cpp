
#include "ActObjClient.hpp"
#include "ActObjMethodCallback.hpp"

#include <sys/socket.h>
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{
ActObjClient::ActObjClient( int socketFd )
    : mSocketFd( socketFd ),
      mUsecaseItterate( 0 )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}
ActObjClient::~ActObjClient()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

int ActObjClient::open( void *args )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return activate();
}
int ActObjClient::svc()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    string message;
    while ( getQ( message ) == 0 )
    {
        processMessage( message );
    }

    return 0;
}

int ActObjClient::getHandle()
{
    return mSocketFd;
}

void ActObjClient::processMessage( const std::string &message )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    string messageToSend = "Unknown Error";

    unsigned int usecase = mUsecaseItterate % 3;
    mUsecaseItterate++;

    switch ( usecase )
    {
        case 0: // sync timeout 2s
        {
            ActiveObject_1_0::Future<string> future = mServantProxy.requestGetReturnMessage( message );

            int rc = future.get( messageToSend, 2000 );
            if ( rc == -1 )
            {
                messageToSend = "Request Timeout";
            }
            send( mSocketFd, messageToSend.c_str(), messageToSend.size(), 0 );
            break;
        }
        case 1: // sync timeout 500ms
        {
            ActiveObject_1_0::Future<string> future = mServantProxy.requestGetReturnMessage( message );

            int rc = future.get( messageToSend, 500 );
            if ( rc == -1 )
            {
                messageToSend = "Request Timeout";
            }
            send( mSocketFd, messageToSend.c_str(), messageToSend.size(), 0 );
            break;
        }
        case 2: // async.
        {
            ActiveObject_1_0::Future<string> future = mServantProxy.requestGetReturnMessage( message );
            future.attach( new CallbackGetReturnMessage( *this ) );
            break;
        }
        default:
        {
            break;
        }
    }
}
} // namespace ExActiveObject