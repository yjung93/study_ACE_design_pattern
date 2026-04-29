/*
 * Task.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: yjung93
 */

#include <system_error>
#include "framework/common/Logger.hpp"
#include "Task.hpp"

namespace Task_1_0
{

Task::Task()
    : mStopRequested( false )
{
    LOG_INFO( "called" );
}

Task::~Task()
{
    LOG_INFO( "called" );
    {
        lock_guard<mutex> guard( mQueueMutex );
        mStopRequested = true;
    }

    mQueueCondition.notify_all();

    if ( mWorkerThread.joinable() )
    {
        mWorkerThread.join();
    }
}
int Task::open( void *args )
{
    LOG_INFO( "Default open implementation. Override in derived class." );
    return 0;
}

int Task::activate()
{
    if ( mWorkerThread.joinable() )
    {
        return -1; // Thread already running
    }

    try
    {
        mWorkerThread = thread( &Task::svcRun, this );
    }
    catch ( const system_error &ex )
    {
        LOG_ERROR( "Task::activate failed: " << ex.what() );
        return -1;
    }

    return 0;
}

int Task::putQ( const string &message )
{
    thread::id this_id = this_thread::get_id();

    LOG_INFO( "threadId=" << this_id << " message=" << message );

    lock_guard<mutex> guard( mQueueMutex );

    if ( mStopRequested )
    {
        return -1;
    }

    mMessageQueue.push_back( message );
    mQueueCondition.notify_one();

    return 0;
}

int Task::getQ( string &message )
{
    int returnCode = 0;

    unique_lock<mutex> lock( mQueueMutex );

    mQueueCondition.wait( lock,
                          [this]
                          { return mStopRequested || !mMessageQueue.empty(); } );

    if ( mMessageQueue.empty() )
    {
        returnCode = -1; // Queue is empty
    }
    else
    {
        message = mMessageQueue.front();

        thread::id this_id = this_thread::get_id();
        LOG_INFO( "threadId=" << this_id << " message=" << message );

        mMessageQueue.erase( mMessageQueue.begin() );
    }
    return returnCode;
}

int Task::svcRun()
{
    bool returnCode = false;
    returnCode = svc();

    return returnCode;
}

int Task::svc()
{
    LOG_INFO( "Default svc implementation. Override in derived class." );
    return 0;
}
} // namespace Task_1_0
