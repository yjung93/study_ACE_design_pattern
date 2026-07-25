
#include <mutex>

#include "framework/common/Logger.hpp"
#include "Stream.hpp"
#include "StreamHead.hpp"
#include "StreamTail.hpp"

using namespace std;

namespace Stream_1_0
{
Stream::Stream( void *arg, Module *head, Module *tail )
    : mStreamHead( nullptr ),
      mStreamTail( nullptr ),
      mLinkedUs( nullptr )
{
}
int Stream::open( void *arg,
                  Module *head,
                  Module *tail )
{
    Task_2_0::Task *headWriter = nullptr;
    Task_2_0::Task *headReader = nullptr;
    Task_2_0::Task *tailWriter = nullptr;
    Task_2_0::Task *tailReader = nullptr;

    if ( head == nullptr )
    {
        headWriter = new StreamHead();
        headReader = new StreamHead();
        head = new Module( "SteamHead", headWriter, headReader, arg );
    }

    if ( tail == nullptr )
    {
        tailWriter = new StreamTail();
        tailReader = new StreamTail();
        tail = new Module( "SteamTail", tailWriter, tailReader, arg );
    }

    if ( ( head == 0 && ( headWriter == 0 || headReader == 0 ) ) || ( tail == 0 && ( tailWriter == 0 || tailReader == 0 ) ) )
    {
        delete headWriter;
        delete headReader;
        delete tailWriter;
        delete tailReader;
        delete head;
        delete tail;
        errno = ENOMEM;
        return -1;
    }

    mStreamHead = head;
    mStreamTail = tail;

    if ( pushModule( tail ) == -1 )
    {
        return -1;
    }
    else if ( pushModule( head, tail, head ) == -1 )
    {
        return -1;
    }
    return 0;
}

int Stream::close()
{
    const lock_guard<mutex> lock( mMutex );
    if ( mStreamHead != nullptr &&
         mStreamTail != nullptr )
    {
        // unlink();

        while ( mStreamHead->getNext() != mStreamTail )
        {
            if ( pop() == -1 )
            {
                return -1;
            }
        }

        if ( mStreamHead->close() == -1 )
        {
            return -1;
        }
        if ( mStreamTail->close() == -1 )
        {
            return -1;
        }

        delete mStreamHead;
        delete mStreamTail;

        mStreamHead = nullptr;
        mStreamTail = nullptr;

        mFinalClose.notify_all();
    }

    return 0;
}

int Stream::push( Module *module )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return pushModule( module, mStreamHead->getNext(), mStreamHead );
}

int Stream::pop()
{
    if ( mStreamHead->getNext() == mStreamTail )
    {
        return -1;
    }
    else
    {
        Module *topModule = mStreamHead->getNext();
        Module *newTop = topModule->getNext();

        mStreamHead->setNext( newTop );

        topModule->close();
        delete topModule;

        mStreamHead->getWriter()->setNext( newTop->getWriter() );
        newTop->getReader()->setNext( mStreamHead->getReader() );
    }
    return 0;
}

int Stream::pushModule( Module *newTop,
                        Module *currentTop,
                        Module *head )
{
    Task_2_0::Task *newTopReader = newTop->getReader();
    Task_2_0::Task *newTopWriter = newTop->getWriter();
    Task_2_0::Task *currentTopReader = nullptr;
    Task_2_0::Task *currentTopWriter = nullptr;

    if ( currentTop )
    {
        currentTopReader = currentTop->getReader();
        currentTopWriter = currentTop->getWriter();
        currentTopReader->setNext( newTopReader );
    }

    newTopWriter->setNext( currentTopWriter );
    if ( head )
    {
        if ( head != newTop )
        {
            head->link( newTop );
        }
    }
    else
    {
        newTopReader->setNext( nullptr );
    }
    newTop->setNext( currentTop );

    if ( newTopReader->open( newTop->getArg() ) == -1 )
    {
        return -1;
    }
    if ( newTopWriter->open( newTop->getArg() ) == -1 )
    {
        return -1;
    }
    return 0;
}

int Stream::put( string &message )
{
    return mStreamHead->getWriter()->put( message );
}
int Stream::get( string &message )
{
    return mStreamHead->getReader()->getQ( message );
}
} // namespace Stream_1_0