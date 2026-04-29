#include <sys/socket.h>

#include "ActObjMethodCallback.hpp"
#include "ActObjClient.hpp"
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{

CallbackGetReturnMessage::CallbackGetReturnMessage( ActObjClient &parent )
    : mParent( parent )
{
}
void CallbackGetReturnMessage::update( const ActiveObject_1_0::Future<string> &future )
{
    string returnMessage;
    int rc = future.get( returnMessage );

    if ( rc != -1 )
    {
        LOG_INFO( "ReturnMessage=" << returnMessage );

        send( mParent.getHandle(), returnMessage.c_str(), returnMessage.size(), 0 );
    }
}

} // namespace ExActiveObject