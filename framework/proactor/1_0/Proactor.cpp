#include <string>
#include <iostream>
#include <thread>

#include "Proactor.hpp"
#include "AsynchResult.hpp"
#include "NotifyPipeManager.hpp"

#include "framework/reactor/1_0/EventHandler.hpp"

namespace Proactor_1_0
{
Proactor *Proactor::mInstance = 0;

Proactor::Proactor()
    : mAiocbList( nullptr ),
      mResultList( nullptr ),
      mAiocbListMaxSize( _SC_AIO_MAX ),
      mAiocbListCurSize( 0 ),
      mNumDeferredAiocb( 0 ),
      mNumStartedAio( 0 ),
      mHandle( Reactor_1_0::INVALID_HANDLE ),
      mAiocbNotifyPipeManager( nullptr ),
      mNotifyPipeReadHandle( Reactor_1_0::INVALID_HANDLE )
{
    createResultAiocbList();
    createNotifyManager();
    notifyCompletion();
}

Proactor::~Proactor()
{
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << endl;

    if ( mAiocbNotifyPipeManager != nullptr )
    {
        delete mAiocbNotifyPipeManager;
    }
}

Proactor *Proactor::getInstance()
{
    if ( Proactor::mInstance == nullptr )
    {
        Proactor::mInstance = new Proactor();
    }

    return Proactor::mInstance;
}

int Proactor::createResultAiocbList()
{
    if ( mAiocbList != nullptr )
    {
        return 0;
    }

    mAiocbList = new aiocb *[mAiocbListMaxSize];
    mResultList = new AsynchResult *[mAiocbListMaxSize];

    // Initialize the array.
    for ( size_t ai = 0; ai < mAiocbListMaxSize; ai++ )
    {
        mAiocbList[ai] = 0;
        mResultList[ai] = 0;
    }

    return 0;
}

int Proactor::deleteResultAiocbList()
{
    if ( mAiocbList == 0 ) // already deleted
        return 0;

    size_t ai;

    // Try to cancel all uncompleted operations; POSIX systems may have
    // hidden system threads that still can work with our aiocbs!
    for ( ai = 0; ai < mAiocbListMaxSize; ai++ )
    {
        if ( mAiocbList[ai] != 0 ) // active operation
        {
            // cancelAiocb( mResultList[ai] );
        }
    }

    int num_pending = 0;

    for ( ai = 0; ai < mAiocbListMaxSize; ai++ )
    {
        if ( mAiocbList[ai] == 0 ) //  not active operation
            continue;

        // Get the error and return status of the aio_ operation.
        int error_status = 0;
        size_t transfer_count = 0;
        int flg_completed = getResultStatus( mResultList[ai],
                                             error_status,
                                             transfer_count );

        //don't delete uncompleted AIOCB's
        if ( flg_completed == 0 ) // not completed !!!
        {
            num_pending++;
        }
        else // completed , OK
        {
            delete mResultList[ai];
            mResultList[ai] = 0;
            mAiocbList[ai] = 0;
        }
    }

    // If it is not possible cancel some operation (num_pending > 0 ),
    // we can do only one thing -report about this
    // and complain about POSIX implementation.
    // We know that we have memory leaks, but it is better than
    // segmentation fault!

    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << "number pending AIO="
         << num_pending
         << endl;

    delete[] mAiocbList;
    mAiocbList = 0;

    delete[] mResultList;
    mResultList = 0;

    return ( num_pending == 0 ? 0 : -1 );
    // ?? or just always return 0;
}

ssize_t Proactor::allocateAioSlot( AsynchResult *result )
{
    size_t i = 0;

    // we reserve zero slot for ACE_AIOCB_Notify_Pipe_Manager
    // so make check for ACE_AIOCB_Notify_Pipe_Manager request

    if ( mNotifyPipeReadHandle == result->aio_fildes ) // Notify_Pipe ?
    {
        // should be free,
        if ( mAiocbList[i] != 0 ) // only 1 request
        { // is allowed
            errno = EAGAIN;

            cout << "Proactor::"
                 << __FUNCTION__
                 << ": "
                 << "internal Proactor error 0"
                 << endl;
        }
    }
    else //try to find free slot as usual, but starting from 1
    {
        for ( i = 1; i < mAiocbListMaxSize; i++ )
            if ( mAiocbList[i] == 0 )
                break;
    }

    if ( i >= mAiocbListMaxSize )
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "internal Proactor error 1"
             << endl;
    }

    //setup OS notification methods for this aio
    result->aio_sigevent.sigev_notify = SIGEV_NONE;

    return static_cast<ssize_t>( i );
}

int Proactor::startAio( AsynchResult *result,
                        Proactor::Opcode op )
{
    lock_guard<recursive_mutex> guard( mRecursiveMutex );

    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << endl;

    int returnValue = -1;
    if ( result == 0 ) // Just check the status of the list
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "invalid result object, nullptr"
             << endl;
        return returnValue;
    }
    // Save operation code in the aiocb
    switch ( op )
    {
        case Proactor::OPCODE_READ:
            result->aio_lio_opcode = LIO_READ;
            break;

        case Proactor::OPCODE_WRITE:
            result->aio_lio_opcode = LIO_WRITE;
            break;

        default:
            cout << "Proactor::"
                 << __FUNCTION__
                 << ": "
                 << " Invalid op code="
                 << op
                 << endl;
    }

    ssize_t slot = allocateAioSlot( result );

    if ( slot < 0 )
        return -1;

    size_t index = static_cast<size_t>( slot );

    mResultList[index] = result; //Store result ptr anyway
    mAiocbListCurSize++;

    int rc = startAioI( result );

    switch ( rc )
    {
        case 0: // started OK
            mAiocbList[index] = result;
            returnValue = 0;
            break;

        case 1: // OS AIO queue overflow
            mNumDeferredAiocb++;
            returnValue = 0;
            break;

        default: // Invalid request, there is no point
            break; // to start it later
    }

    if ( returnValue == -1 )
    {
        mResultList[index] = 0;
        mAiocbListCurSize--;
    }

    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << " slot="
         << slot
         << " AiocbListCurSize="
         << mAiocbListCurSize
         << " return value="
         << returnValue
         << endl;

    return returnValue;
}

int Proactor::startAioI( AsynchResult *result )
{
    int returnValue = -1;
    string pPype;

    // Start IO
    // The following aio_ptr anathema is required to work around a bug in
    // the optimizer for GCC 4.1.2
    aiocb *aio_ptr( result );
    switch ( result->aio_lio_opcode )
    {
        case LIO_READ:
            pPype = "read ";
            returnValue = aio_read( aio_ptr );
            break;
        case LIO_WRITE:
            pPype = "write";
            returnValue = aio_write( aio_ptr );
            break;
        default:
            pPype = "?????";
            returnValue = -1;
            break;
    }

    if ( returnValue == 0 )
    {
        ++mNumStartedAio;
    }
    else // if (returnValue == -1)
    {
        if ( errno == EAGAIN || errno == ENOMEM ) //Ok, it will be deferred AIO
        {
            cout << "Proactor::"
                 << __FUNCTION__
                 << ": "
                 << pPype
                 << " errno="
                 << errno
                 << " it will be deferred AIO"
                 << endl;

            returnValue = 1;
        }
        else
        {
            cout << "Proactor::"
                 << __FUNCTION__
                 << ": "
                 << pPype
                 << " queueing failed"
                 << endl;
        }
    }

    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << " aio_fildes="
         << aio_ptr->aio_fildes
         << " aio_nbytes="
         << aio_ptr->aio_nbytes
         << " aio_offset="
         << aio_ptr->aio_offset
         << " aio_sigevent.sigev_signo="
         << aio_ptr->aio_sigevent.sigev_signo
         << " aio_sigevent.sigev_notify="
         << aio_ptr->aio_sigevent.sigev_notify
         << " pPype="
         << pPype
         << " NumStartedAio"
         << mNumStartedAio
         << " return value="
         << returnValue
         << endl;

    return returnValue;
}

int Proactor::cancelAio( int handle )
{
    lock_guard<recursive_mutex> guard( mRecursiveMutex );
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << endl;

    return 0;
}

int Proactor::handleEvents()
{
    int resultSuspend = 0;
    int retval = 0;

    // Create a temporary list with ONLY valid requests
    // to pass the compacted list to aio_suspend
    vector<aiocb *> activeList;
    activeList.reserve( mAiocbListCurSize + 1 ); // +1 safety

    {
        // mutex lock  while building the list
        lock_guard<recursive_mutex> guard( mRecursiveMutex );
        for ( int idxAioList = 0; idxAioList < static_cast<int>( mAiocbListMaxSize ); idxAioList++ )
        {
            // cout << "Proactor::"
            //      << __FUNCTION__
            //      << ": activeList["
            //      << idxAioList
            //      << "] = "
            //      << mAiocbList[idxAioList]
            //      << endl;

            if ( mAiocbList[idxAioList] != nullptr )
            {
                activeList.push_back( mAiocbList[idxAioList] );
            }
        }
    }

    cout << "Proactor::"
         << __FUNCTION__
         << ": mAiocbList.size="
         << mAiocbListMaxSize
         << endl;

    cout << "Proactor::"
         << __FUNCTION__
         << ": activeList.size="
         << activeList.size()
         << endl;

    resultSuspend = aio_suspend( activeList.data(),
                                 activeList.size(),
                                 0 );

    cout << "Proactor::"
         << __FUNCTION__
         << ": aio_suspend, resultSuspend="
         << resultSuspend
         << endl;

    // Check for errors
    if ( resultSuspend == -1 )
    {
        if ( errno != EAGAIN && // Timeout
             errno != EINTR ) // Interrupted call
        {
            cout << "Proactor::"
                 << __FUNCTION__
                 << ": "
                 << " aio_suspend Failed"
                 << endl;
        }

        // let continue work
        // we should check "post_completed" queue
    }
    else
    {
        size_t index = 0;
        size_t count = mAiocbListMaxSize; // max number to iterate
        int errorStatus = 0;
        size_t transferCount = 0;

        for ( ;; retval++ )
        {
            AsynchResult *AsynchResult = findCompletedAio( errorStatus,
                                                           transferCount,
                                                           index,
                                                           count );

            if ( AsynchResult == 0 )
            {
                break;
            }
            // Call the application code.
            applicationSpecificCode( AsynchResult,
                                     transferCount,
                                     0, // No completion key.
                                     errorStatus );
        }
    }

    // process post_completed results
    // retval +=  process_result_queue();

    return retval > 0 ? 1 : 0;
}

AsynchResult *Proactor::findCompletedAio( int &errorStatus,
                                          size_t &transferCount,
                                          size_t &index,
                                          size_t &count )
{
    // parameter index defines initial slot to scan
    // parameter count tells us how many slots should we scan

    lock_guard<recursive_mutex> guard( mRecursiveMutex );
    AsynchResult *AsynchResult = 0;

    if ( mNumStartedAio == 0 ) // save time
        return 0;

    for ( ; count > 0; index++, count-- )
    {
        if ( index >= mAiocbListMaxSize ) // like a wheel
        {
            index = 0;
        }

        if ( mAiocbList[index] == nullptr ) // Dont process null blocks.
        {
            continue;
        }

        if ( 0 != getResultStatus( mResultList[index],
                                   errorStatus,
                                   transferCount ) ) // completed
        {
            break;
        }

    } // end for

    if ( count == 0 ) // all processed , nothing found
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "all processed , nothing found."
             << " transferCount="
             << transferCount
             << " index="
             << index
             << " count="
             << count
             << " mAiocbListMaxSize="
             << mAiocbListMaxSize
             << endl;
        return 0;
    }

    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << "found completed ."
         << " transferCount="
         << transferCount
         << " index="
         << index
         << " count="
         << count
         << " mAiocbListMaxSize="
         << mAiocbListMaxSize
         << endl;

    AsynchResult = mResultList[index];

    mAiocbList[index] = 0;
    mResultList[index] = 0;
    mAiocbListCurSize--;

    mNumStartedAio--; // decrement count active aios
    index++; // for next iteration
    count--; // for next iteration

    startDeferredAio();

    return AsynchResult;
}

int Proactor::getResultStatus( AsynchResult *asynchResult,
                               int &errorStatus,
                               size_t &transferCount )
{
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << " aio_fildes="
         << asynchResult->aio_fildes
         << " aio_nbytes="
         << asynchResult->aio_nbytes
         << " aio_offset="
         << asynchResult->aio_offset
         << " aio_sigevent.sigev_signo="
         << asynchResult->aio_sigevent.sigev_signo
         << " aio_sigevent.sigev_notify="
         << asynchResult->aio_sigevent.sigev_notify
         << endl;

    transferCount = 0;

    // Get the error status of the aio_ operation.
    // The following aio_ptr anathema is required to work around a bug in an over-aggressive
    // optimizer in GCC 4.1.2.
    aiocb *aio_ptr( asynchResult );
    errorStatus = aio_error( aio_ptr );
    if ( errorStatus == EINPROGRESS )
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "errorStatus="
             << errorStatus
             << endl;
        return 0; // not completed
    }

    ssize_t op_return = aio_return( aio_ptr );
    if ( op_return > 0 )
    {
        transferCount = static_cast<size_t>( op_return );
    }
    // else transferCount is already 0, errorStatus reports the error.
    return 1; // completed
}

int Proactor::startDeferredAio()
{
    // This protected method is called from
    // find_completed_aio after any AIO completion
    // We should call this method always with locked
    // ACE_POSIX_AIOCB_Proactor::mutex_
    //
    // It tries to start the first deferred AIO
    // if such exists

    if ( mNumDeferredAiocb == 0 )
    {
        return 0; //  nothing to do
    }
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << " NumDeferredAiocb="
         << mNumDeferredAiocb
         << endl;

    size_t i = 0;

    for ( i = 0; i < mAiocbListMaxSize; i++ )
    {
        if ( mResultList[i] != 0 // check for
             && mAiocbList[i] == 0 ) // deferred AIO
        {
            break;
        }
    }

    if ( i >= mAiocbListMaxSize )
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "internal Proactor error 3\n"
             << endl;

        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << " i="
             << i
             << ", mAiocbListMaxSize="
             << mAiocbListMaxSize
             << endl;
    }

    AsynchResult *result = mResultList[i];

    int rc = startAioI( result );

    switch ( rc )
    {
        case 0: //started OK , decrement count of deferred AIOs
            mAiocbList[i] = result;
            mNumDeferredAiocb--;
            notifyCompletion(); // to wake up suspended event handling loop
            return 0;

        case 1:
            return 0; //try again later

        default: // Invalid Parameters , should never be
            break;
    }

    //AL notify  user

    mResultList[i] = 0;
    --mAiocbListCurSize;

    --mNumDeferredAiocb;

    result->setError( errno );
    result->setBytesTransferred( 0 );
    putqResult( result ); // we are with locked mutex_ here !

    return -1;
}

int Proactor::putqResult( AsynchResult *result )
{
    // this protected method should be called with locked mutex_
    // we can't use GUARD as Proactor uses non-recursive mutex

    if ( !result )
        return -1;

    int sigNum = result->signalNumber();
    mResultQueue.push_back( result );

    (void) sigNum; // avoid unused variable
    notifyCompletion();

    return 0;
}

void Proactor::applicationSpecificCode( AsynchResult *asynchResult,
                                        size_t bytes_transferred,
                                        const void * /* completionKey*/,
                                        u_long error )
{
    // ACE_SEH_TRY
    {
        // Call completion hook
        asynchResult->complete( bytes_transferred,
                                error ? 0 : 1,
                                error );
    }
    // ACE_SEH_FINALLY
    {
        // This is crucial to prevent memory leaks
        delete asynchResult;
    }
}

int Proactor::proactorRunEventLoop()
{
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << endl;

    int result = 0;

    {
        lock_guard<recursive_mutex> guard( mRecursiveMutex );

        // Early check. It is ok to do this without lock, since we care just
        // whether it is zero or non-zero.
        if ( mEndEventLoop != 0 )
            return 0;

        // First time you are in. Increment the thread count.
        mEventLoopThreadCount++;
    }

    // Run the event loop.
    for ( ;; )
    {
        // Check the end loop flag. It is ok to do this without lock,
        // since we care just whether it is zero or non-zero.
        if ( mEndEventLoop != 0 )
            break;

        // <end_event_loop> is not set. Ready to do <handle_events>.
        result = handleEvents();

        if ( result == -1 )
            break;
    }

    // Leaving the event loop. Decrement the thread count.

    {
        // Obtain the lock in the MT environments.
        lock_guard<recursive_mutex> guard( mRecursiveMutex );

        // Decrement the thread count.
        mEventLoopThreadCount--;

        if ( mEventLoopThreadCount > 0 && mEndEventLoop != 0 )
            postWakeupCompletions( 1 );
    }

    return result;
}

int Proactor::postWakeupCompletions( int how_many )
{
    // Jake TBD
    //   ACE_POSIX_Wakeup_Completion *wakeupCompletion = 0;

    //   for (int ci = 0; ci < how_many; ci++)
    //     {
    //       ACE_NEW_RETURN
    //         (wakeupCompletion,
    //          ACE_POSIX_Wakeup_Completion ( wakeup_handler_.proxy ()),
    //          -1);
    //       if ( postCompletion (wakeupCompletion) == -1)
    //         return -1;
    //     }

    return 0;
}

int Proactor::postCompletion( AsynchResult *result )
{
    lock_guard<recursive_mutex> guard( mRecursiveMutex );

    int ret_val = putqResult( result );

    return ret_val;
}

int Proactor::get_handle() const
{
    return mHandle;
}

void Proactor::createNotifyManager()
{
    if ( mAiocbNotifyPipeManager == nullptr )
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << endl;

        mAiocbNotifyPipeManager = new NotifyPipeManager( this );
    }
    else
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "something wrong"
             << endl;
    }
}

int Proactor::notifyCompletion()
{
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << endl;

    int rc = -1;

    if ( mAiocbNotifyPipeManager != nullptr )
    {
        rc = mAiocbNotifyPipeManager->notify();
    }
    else
    {
        cout << "Proactor::"
             << __FUNCTION__
             << ": "
             << "mAiocbNotifyPipeManager : nullptr"
             << endl;
    }
    return rc;
}

void Proactor::setNotifyHandle( int handle )
{
    cout << "Proactor::"
         << __FUNCTION__
         << ": "
         << "handle="
         << handle
         << endl;
    mNotifyPipeReadHandle = handle;
}

} // namespace Proactor_1_0