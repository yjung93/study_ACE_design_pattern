/*
 * Task.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: yjung93
 */

#include <system_error>
#include "framework/common/Logger.hpp"
#include "Task.hpp"
#include "framework/stream/1_0/Module.hpp"

namespace Task_2_0
{

Task::Task()
    : mFlags( 0u ),
      mStopRequested( false ),
      mModule( nullptr ),
      mNext( nullptr )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

Task::~Task()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
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
    // thread::id this_id = this_thread::get_id();
    // LOG_INFO( "threadId=" << this_id << " message=" << message );

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

        // thread::id this_id = this_thread::get_id();
        // LOG_INFO( "threadId=" << this_id << " data=" << message );

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

int Task::open( void *a )
{
    LOG_INFO( "Default open implementation. Override in derived class." );
    return 0;
}
int Task::close( u_long flags )
{
    LOG_INFO( "Default close implementation. Override in derived class." );
    return 0;
}
int Task::put( string &msg )
{
    LOG_INFO( "Default put implementation. Override in derived class." );
    return 0;
}
int Task::svc()
{
    LOG_INFO( "Default svc implementation. Override in derived class." );
    return 0;
}

Task *Task::getNext()
{
    return mNext;
}
void Task::setNext( Task *task )
{
    mNext = task;
}

bool Task::isWriter()
{
    return ( ( mFlags & static_cast<u_long>( READER ) ) != 0 ) ? false : true;
}

int Task::putNext( string &msg )
{
    if ( mNext == nullptr )
    {
        return -1;
    }

    return mNext->put( msg );
}

Task *Task::getSibling()
{
    LOG_INFO( "Task::" << __FUNCTION__ << "() " << "called" );
    if ( mModule != nullptr )
    {
        return mModule->getSibling( this );
    }
    else
    {
        return nullptr;
    }
}

} // namespace Task_2_0
