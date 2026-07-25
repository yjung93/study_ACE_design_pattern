#include "AsynchWriteStream.hpp"
#include "framework/common/Logger.hpp"

namespace Proactor_1_0
{

AsynchWriteStreamResult::AsynchWriteStreamResult( const Handler::ProxyPtr &handler_proxy,
                                                  int handle,
                                                  vector<uint8_t> &message,
                                                  size_t bytesToRead,
                                                  const void *act,
                                                  int priority,
                                                  int signal_number )
    : AsynchResult( handler_proxy,
                    act,
                    0,
                    0,
                    priority,
                    signal_number ),
      mMessage( message )
{
    this->aio_fildes = handle;
    this->aio_nbytes = message.size();

    if ( message.size() > 0 )
    {
        // Use the string buffer. Caller must ensure message is resized to hold data.
        this->aio_buf = message.data();
    }
    else
    {
        this->aio_buf = nullptr;
    }

    LOG_INFO( "aio_fildes=" << aio_fildes
              << " aio_nbytes=" << aio_nbytes
              << " aio_offset=" << aio_offset
              << " aio_sigevent.sigev_signo=" << aio_sigevent.sigev_signo
              << " aio_sigevent.sigev_notify=" << aio_sigevent.sigev_notify );
}

AsynchWriteStreamResult::~AsynchWriteStreamResult()
{
}

size_t AsynchWriteStreamResult::bytes_to_write() const
{
    return this->aio_nbytes;
}

/// Message block which contains the read data.
vector<uint8_t> &AsynchWriteStreamResult::message() const
{
    return this->mMessage;
}

/// I/O handle used for reading.
int AsynchWriteStreamResult::handle() const
{
    return this->aio_fildes;
}

void AsynchWriteStreamResult::complete( size_t bytes_transferred, int success, u_long error )
{
    this->mBytesTransferred = bytes_transferred;
    this->mSuccess = success;
    this->mError = error;

    // <errno> is available in the aiocb.
    (void) error;

    // Appropriately move the pointers in the message block.
    // this->message_block_.wr_ptr( bytes_transferred );

    // Create the interface result class.
    // ACE_Asynch_Read_Stream::Result result( this );

    // Call the application handler.
    Handler *handler = this->mhandlerProxy.get()->handler();
    if ( handler != 0 )
    {
        handler->handleWriteStream( *this );
    }
}

AsynchWriteStream::AsynchWriteStream()
{
}
AsynchWriteStream::~AsynchWriteStream()
{
}

int AsynchWriteStream::write( vector<uint8_t> &message,
                              size_t bytes_to_write,
                              const void *act,
                              int priority,
                              int signal_number )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );


    if ( bytes_to_write == 0 )
    {
        errno = ENOSPC;
        return -1;
    }

    // Create the Asynch_Result.

    Proactor *proactor = this->proactor();
    AsynchWriteStreamResult *result = new AsynchWriteStreamResult( this->mhandlerProxy,
                                                                   this->mHandle,
                                                                   message,
                                                                   bytes_to_write,
                                                                   act,
                                                                   priority,
                                                                   signal_number );

    int return_val = proactor->startAio( result, Proactor::OPCODE_WRITE );

    LOG_INFO( "return_val=" << return_val );

    if ( return_val == -1 )
    {
        delete result;
    }

    return return_val;
}

int AsynchWriteStream::open( Handler &handler,
                             int handle,
                             const void *completionKey,
                             Proactor *proactor )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    mProactor = proactor;
    mhandlerProxy = handler.proxy();
    mHandle = handle;
    return 0;
}

Proactor *AsynchWriteStream::proactor() const
{
    return mProactor;
}
} // namespace Proactor_1_0