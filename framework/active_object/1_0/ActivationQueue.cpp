#include <thread>
#include "framework/common/Logger.hpp"

#include "ActivationQueue.hpp"

namespace ActiveObject_1_0
{

ActivationQueue::ActivationQueue()
    : mState( STATE_ACTIVE )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

ActivationQueue::~ActivationQueue()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    exit();
}

MethodRequest *ActivationQueue::dequeue()
{
    MethodRequest *methodRequest = nullptr;

    unique_lock<mutex> lock( mQueueMutex );

    // mQueueCondition.wait( lock,
    //                       [this]
    //                       { return !mMethodRequestQueue.empty(); } );

    mQueueCondition.wait( lock );
    if ( mState == STATE_SHUTDOWN )
    {
        LOG_INFO( "exit" );

        return methodRequest;
    }

    if ( mMethodRequestQueue.empty() )
    {
        LOG_INFO( "mMethodRequestQueue is empty" );
    }
    else
    {
        methodRequest = mMethodRequestQueue.front();

        thread::id this_id = this_thread::get_id();
        LOG_INFO( "threadId=" << this_id );

        mMethodRequestQueue.erase( mMethodRequestQueue.begin() );
    }

    return methodRequest;
}

int ActivationQueue::enqueue( MethodRequest *methodRequest )
{
    thread::id this_id = this_thread::get_id();

    LOG_INFO( "threadId=" << this_id );

    lock_guard<mutex> guard( mQueueMutex );

    mMethodRequestQueue.push_back( methodRequest );
    mQueueCondition.notify_one();
    return 0;
}

void ActivationQueue::exit()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    mState = STATE_SHUTDOWN;
    mQueueCondition.notify_one();
}

} // namespace ActiveObject_1_0