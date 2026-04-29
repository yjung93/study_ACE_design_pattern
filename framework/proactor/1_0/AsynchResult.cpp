#include <cstring> 

#include "AsynchResult.hpp"
#include "Proactor.hpp"
#include "Handler.hpp"
#include "framework/common/Logger.hpp"

#include "framework/reactor/1_0/EventHandler.hpp"

namespace Proactor_1_0
{

size_t AsynchResult::bytes_transferred() const
{
    return this->mBytesTransferred;
}

void AsynchResult::setBytesTransferred( size_t nbytes )
{
    this->mBytesTransferred = nbytes;
}

const void *AsynchResult::act() const
{
    return this->mAct;
}

int AsynchResult::success() const
{
    return this->mSuccess;
}

u_long AsynchResult::error() const
{
    return this->mError;
}

void AsynchResult::setError( u_long errcode )
{
    this->mError = errcode;
}

int AsynchResult::event() const
{
    return Reactor_1_0::INVALID_HANDLE;
}

u_long AsynchResult::offset() const
{
    return this->aio_offset;
}

u_long AsynchResult::offsetHigh() const
{
    // errno = ENOTSUP;
    return 0;
}

int AsynchResult::priority() const
{
    return this->aio_reqprio;
}

int AsynchResult::signalNumber() const
{
    return this->aio_sigevent.sigev_signo;
}

int AsynchResult::postCompletion( Proactor *proactor )
{
    // Get to the platform specific implementation.

    if ( proactor == 0 )
    {
        LOG_ERROR( "proactor null pointer" );
        return -1;
    }

    // Post myself.
    return proactor->postCompletion( this );
}

AsynchResult::~AsynchResult()
{
}

AsynchResult::AsynchResult( const Handler::ProxyPtr &handler_proxy,
                            const void *act,
                            u_long offset,
                            u_long offset_high,
                            int priority,
                            int signal_number )

    : mhandlerProxy( handler_proxy ),
      mAct( act ),
      mBytesTransferred( 0 ),
      mSuccess( 0 ),
      mError( 0 )
{
    ::memset(static_cast<aiocb*>(this), 0, sizeof(aiocb));

    aio_offset = offset;
    aio_reqprio = priority;
    aio_sigevent.sigev_signo = signal_number;
    aio_sigevent.sigev_notify = SIGEV_NONE; 
    //
    // @@ Support offset_high with aiocb64.
    //
    (void) offset_high;

    // Other fields in the <aiocb> will be initialized by the
    // subclasses.
}

} // namespace Proactor_1_0