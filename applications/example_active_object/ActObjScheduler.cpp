
#include "ActObjScheduler.hpp"
#include <sys/socket.h>
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{

ActObjScheduler::ActObjScheduler()
{
    LOG_INFO( "called" );

    activate();
}
ActObjScheduler::~ActObjScheduler()
{
    LOG_INFO( "called" );
}

int ActObjScheduler::svc()
{
    LOG_INFO( "called" );

    while ( true )
    {
        ActiveObject_1_0::MethodRequest *methodRequest = this->mActivationQueue.dequeue();
        if ( methodRequest == nullptr)
        {
            break;
        }

        // Dequeue the next method object
        std::unique_ptr<ActiveObject_1_0::MethodRequest> request( methodRequest);

        // Invoke the method request.
        if ( request->call() == -1 )
        {
            break;
        }
    }
    return 0;
}

int ActObjScheduler::enqueue( ActiveObject_1_0::MethodRequest *request )
{
    LOG_INFO( "called" );

    return mActivationQueue.enqueue( request );
}

} //namespace ExActiveObject
