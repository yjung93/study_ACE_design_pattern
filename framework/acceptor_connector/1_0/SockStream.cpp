/*
 * SockStream.cpp
 *
 *  Created on: May 9, 2025
 *      Author: yjung93
 */

#include "Config.hpp"
#include "SockStream.hpp"
#include "framework/common/Logger.hpp"

namespace AcceptorConnector_1_0
{

SockStream::SockStream()
    : mHandle( INVALID_HANDLE )
{
}

SockStream::~SockStream()
{
}

void SockStream::setHandle( int handle )
{
    mHandle = handle;
}
int SockStream::getHandle() const
{
    return mHandle;
}

ssize_t SockStream::recv_n( void *buf, size_t size, int flags )
{
    return ::recv( getHandle(), buf, size, flags );
}

ssize_t SockStream::send_n( const void *buf, size_t size, int flags )
{
    return ::send( getHandle(), buf, size, flags );
}

int SockStream::close_reader()
{
    return ::shutdown( getHandle(), SHUT_RD );
}
int SockStream::close_writer()
{
    return ::shutdown( getHandle(), SHUT_WR );
}

int SockStream::close()
{
    close_writer();
    return ::close( this->getHandle() );
}

int SockStream::open( int type, int protocolFamily, int protocol, int reuseAddr )
{
    int result = 0;

    setHandle( ::socket( protocolFamily, type, protocol ) );

    if ( this->getHandle() == INVALID_HANDLE )
    {
        result = -1;
    }

    LOG_INFO( "handle: " << getHandle() );

    return result;
}

int SockStream::enable( int value )
{
    int result = 0;
    if ( value == O_NONBLOCK )
    {
        int flags = ::fcntl( this->getHandle(), F_GETFL, 0 );
        if ( flags < 0 )
        {
            std::perror( "fcntl(F_GETFL)" );
            ::close( this->getHandle() );
            result = -1;
        }

        flags |= O_NONBLOCK;

        if ( ( result != -1 && ::fcntl( this->getHandle(), F_SETFL, flags ) < 0 ) )
        {
            std::perror( "fcntl(F_SETFL)" );
            ::close( this->getHandle() );
            result = -1;
        }
    }

    LOG_INFO( "value=0x"
              << std::hex << value
              << ", fcntl-value=0x"
              << std::hex
              << ::fcntl( this->getHandle(), F_GETFL, 0 ) );

    return result;
}

int SockStream::disable( int value )
{
    int result = 0;
    if ( value == O_NONBLOCK )
    {
        int flags = ::fcntl( this->getHandle(), F_GETFL, 0 );
        if ( flags < 0 )
        {
            std::perror( "fcntl(F_GETFL)" );
            ::close( this->getHandle() );
            result = -1;
        }

        flags &= ~( O_NONBLOCK );

        if ( ( result != -1 && fcntl( this->getHandle(), F_SETFL, flags ) < 0 ) )
        {
            std::perror( "fcntl(F_SETFL)" );
            ::close( this->getHandle() );
            result = -1;
        }
    }

    LOG_INFO( "value=0x"
              << std::hex
              << value
              << ", fcntl-value=0x"
              << std::hex
              << ::fcntl( this->getHandle(), F_GETFL, 0 ) );

    return result;
}

int SockStream::getRemoteAddr( sockaddr_in &socketAddr ) const
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    int result = 0;
    socklen_t peerLen = sizeof( socketAddr );
    sockaddr *addr = reinterpret_cast<sockaddr *>( &socketAddr );

    if ( ::getpeername( this->getHandle(), addr, &peerLen ) == -1 )
    {
        result = -1;
    }

    return result;
}

} /* namespace AcceptorConnector_1_0 */
