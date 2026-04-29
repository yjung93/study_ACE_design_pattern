#include "ActObjMethodRequests.hpp"
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{

RequestGetReturnMessage::RequestGetReturnMessage( ActObjServant &Servant,
                                                  ActiveObject_1_0::Future<string> &returnVal,
                                                  const string &message )
    : mServant( Servant ),
      mReturnValue( returnVal ),
      mMessage( message )
{
    LOG_INFO( "called" );
}

int RequestGetReturnMessage::call()
{
    LOG_INFO( "called" );

    // status_update with the controller.
    this->mReturnValue.set( this->mServant.RequestGetReturnMessage( mMessage ) );
    return 0;
}


} // namespace ExActiveObject