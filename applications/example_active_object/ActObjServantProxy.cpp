#include "ActObjServantProxy.hpp"
#include "ActObjMethodRequests.hpp"
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{

ActObjServantProxy::ActObjServantProxy()
{
    LOG_INFO( "called" );
}

ActObjServantProxy::~ActObjServantProxy()
{
    LOG_INFO( "called" );
}

ActiveObject_1_0::Future<string> ActObjServantProxy::requestGetReturnMessage( const string &message )
{
    LOG_INFO( "called" );

    ActiveObject_1_0::Future<string> result;
    ActiveObject_1_0::MethodRequest *methodRequest = new RequestGetReturnMessage( mServant, result, message );

    mScheduler.enqueue( methodRequest );

    return result;
}


} // namespace ExActiveObject