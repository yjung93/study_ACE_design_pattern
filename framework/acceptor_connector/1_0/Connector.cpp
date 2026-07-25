/*
 * Connector.cpp
 *
 *  Created on: Jan 14, 2025
 *      Author: yjung93
 */
#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_CONNECTOR_CPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_CONNECTOR_CPP_

#include <arpa/inet.h>
#include <cerrno>
#include "framework/common/Logger.hpp"

#include "Connector.hpp"

namespace AcceptorConnector_1_0
{
template <typename SVC_HANDLER, typename PEER_CONNECTOR>
Connector<SVC_HANDLER, PEER_CONNECTOR>::Connector( Reactor_1_0::Reactor *reactor, int flags )
    : Reactor_1_0::EventHandler( reactor )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    this->open( reactor, flags );
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
Connector<SVC_HANDLER, PEER_CONNECTOR>::~Connector()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}
template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::open( Reactor_1_0::Reactor *reactor, int flags )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    this->setReactor( reactor );
    this->mFlags = flags;
    return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
bool Connector<SVC_HANDLER, PEER_CONNECTOR>::close( SVC_HANDLER *&sh )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    return true;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::connect( SVC_HANDLER *&svcHandler, const typename PEER_CONNECTOR::PEER_ADDR &remoteAddr, const timeval *timeOut, int flags )
{
    int result = 0;

    result = makeSvcHandler( svcHandler );

    if ( result != -1 )
    {
        result = connectSvcHandler( svcHandler, remoteAddr, timeOut, flags );
    }

    if ( result != -1 )
    {
        result = activateSvcHandler( svcHandler );
    }

    if ( result == -1 )
    {
        if ( errno == EINPROGRESS || errno == EWOULDBLOCK )
        {
            LOG_INFO( "errno=" << ( ( errno == EINPROGRESS ) ? "EINPROGRESS" : "EWOULDBLOCK" ) );

            nonblockingConnect( svcHandler );
        }
    }
    return result;
}
template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::handleInput( int fd )
{
    return 0;
}
template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::handleClose( int handle )
{
    return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::makeSvcHandler( SVC_HANDLER *&svcHandler )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    if ( svcHandler == nullptr )
    {
        svcHandler = new SVC_HANDLER;
        svcHandler->setReactor( this->getReactor() );
    }
    else
    {
        LOG_INFO( "svcHandler is not a null pointer, skip create svcHandler" );
    }

    return 0;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::connectSvcHandler( SVC_HANDLER *svcHandler, const typename PEER_CONNECTOR::PEER_ADDR remoteAddr, const timeval *timeOut, int flags )
{
    LOG_INFO( "flag=0x" << std::hex << flags );

    int result = 0;

    if ( svcHandler == nullptr )
    {
        result = -1;
    }

    if ( result != -1 )
    {
        result = mConnector.connect( svcHandler->peer(), remoteAddr, timeOut, 0, flags );
    }

    return result;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::activateSvcHandler( SVC_HANDLER *svcHandler )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    int result = 0;
    if ( svcHandler == nullptr )
    {
        result = -1;
    }

    if ( result != -1 )
    {
        if ( svcHandler->open( (void *) this ) == -1 )
        {
            result = -1;
        }
    }

    if ( result == -1 )
    {
        svcHandler->close();
    }

    return result;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
int Connector<SVC_HANDLER, PEER_CONNECTOR>::nonblockingConnect( SVC_HANDLER *svcHandler )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    int result = -1;
    NBCH *nbch = 0;
    if ( svcHandler != nullptr )
    {
        nbch = new NBCH( *this, svcHandler );
        result = getReactor()->registerHandler( nbch,
                                                Reactor_1_0::EventHandler::CONNECT_MASK );
    }

    // TBD : register timer, once it is available

    return result;
}

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
void Connector<SVC_HANDLER, PEER_CONNECTOR>::initializeSvcHandler( int handle, SVC_HANDLER *svc_handler )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    bool result = true;

    if ( svc_handler == nullptr )
    {
        result = false;
    }

    if ( result == true )
    {
        svc_handler->setHandle( handle );

        typename PEER_CONNECTOR::PEER_ADDR raddr;

        // Check to see if we're connected.
        if ( svc_handler->peer().getRemoteAddr( raddr ) != -1 )
        {
            activateSvcHandler( svc_handler );
        }
        else // Somethings gone wrong, so close down...
        {
            svc_handler->close();
        }
    }
}

template <typename SVC_HANDLER>
NonBlockingConnectHandler<SVC_HANDLER>::NonBlockingConnectHandler( ConnectorBase<
                                                                       SVC_HANDLER> &connector,
                                                                   SVC_HANDLER *sh )
    : mConnector( connector ),
      mSvcHandler( sh )
{
    if ( mSvcHandler != nullptr )
    {
        setHandle( mSvcHandler->getHandle() );
    }
}

template <typename SVC_HANDLER>
NonBlockingConnectHandler<SVC_HANDLER>::~NonBlockingConnectHandler()
{
}
template <typename SVC_HANDLER>
int NonBlockingConnectHandler<SVC_HANDLER>::handleOutput( int handle )
{
    // Called when a connection is establishment asynchronous.
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    // Grab the connector ref before smashing ourselves in close().
    ConnectorBase<SVC_HANDLER> &connector = this->mConnector;
    SVC_HANDLER *svc_handler = 0;
    int const retval = this->close( svc_handler ) ? 0 : -1;

    if ( svc_handler != 0 )
    {
        connector.initializeSvcHandler( handle, svc_handler );
    }

    return retval;
}

template <typename SVC_HANDLER>
int NonBlockingConnectHandler<SVC_HANDLER>::handleInput( int handle )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    return 0;
}

template <typename SVC_HANDLER>
int NonBlockingConnectHandler<SVC_HANDLER>::handleClose( int handle )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    return 0;
}

template <typename SVC_HANDLER>
int NonBlockingConnectHandler<SVC_HANDLER>::handleException( int handle )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    return 0;
}

template <typename SVC_HANDLER>
SVC_HANDLER *NonBlockingConnectHandler<SVC_HANDLER>::svcHandler()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    return mSvcHandler;
}

template <typename SVC_HANDLER>
bool NonBlockingConnectHandler<SVC_HANDLER>::close( SVC_HANDLER *&sh )
{
    bool result = true;

    // Make sure that we haven't already initialized the Svc_Handler.
    if ( this->mSvcHandler == nullptr )
    {
        result = false;
    }

    if ( result == true )
    {
        // Exclusive access to the Reactor.
        if ( result == true )
        {
            sh = this->mSvcHandler;
            this->mSvcHandler = 0;

            // Remove from Reactor.
            if ( this->getReactor()->removeHandler( sh, ALL_EVENTS_MASK ) == -1 )
            {
                result = false;
            }
            this->mSvcHandler = 0;
        }
    }
    return result;
}

} /* namespace AcceptorConnector_1_0 */

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_CONNECTOR_CPP_ */
