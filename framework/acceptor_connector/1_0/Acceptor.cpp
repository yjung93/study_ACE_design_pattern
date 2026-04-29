/*
 * Acceptor.cpp
 *
 *  Created on: Jan 14, 2025
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ACCEPTOR_CPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ACCEPTOR_CPP_

#include "Acceptor.hpp"

namespace AcceptorConnector_1_0
{

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::Acceptor( const typename PEER_ACCEPTOR::PEER_ADDR &localAddr, Reactor_1_0::Reactor *reactor, int flags ) :
                Reactor_1_0::EventHandler( reactor )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;
    open( localAddr, reactor, flags );
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::Acceptor( Reactor_1_0::Reactor *reactor, int flags ) :
                Reactor_1_0::EventHandler( reactor )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::~Acceptor()
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::open( const typename PEER_ACCEPTOR::PEER_ADDR &localAddr, Reactor_1_0::Reactor *reactor, int flags )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;
    int result = 0;

    this->mPeerAcceptorAddr = localAddr;

    // Must supply a valid Reactor to Acceptor::open()...
    if ( result != -1 && reactor == 0 )
    {
        result = -1;
    }

    // Open the underlying PEER_ACCEPTOR.
    if ( result != -1 && this->mPeerAcceptor.open( mPeerAcceptorAddr ) == -1 )
    {
        result = -1;
    }

    if ( result != -1 )
    {
        setHandle( this->mPeerAcceptor.getHandle() );
        result = reactor->registerHandler( this, Reactor_1_0::EventHandler::ACCEPT_MASK );
    }
    return result;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::close()
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;

    return this->handleClose();
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::handleClose( int handle )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;
    return 0;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::handleInput( int fd )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << " fd="
    << fd
    << endl;

    SVC_HANDLER *svcHandler = 0;

    makeSvcHandler( svcHandler );

    acceptSvcHandler( svcHandler );

    activateSvcHandler( svcHandler ); 

    return 0;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::makeSvcHandler( SVC_HANDLER *&svcHandler )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;

    if ( svcHandler == nullptr )
    {
        svcHandler = new SVC_HANDLER;
        svcHandler->setReactor( this->getReactor() );
    }else
    {
        cout
        << __FUNCTION__
        << "svcHandler is not a null pointer, skip create svcHandler"
        << endl;
    }

    return 0;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::acceptSvcHandler( SVC_HANDLER *svcHandler )
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;

    int result = 0;

    if ( svcHandler == nullptr )
    {
        result = -1;
    }

    int svchandle = -1;
    if ( result != -1 )
    {
        svchandle = this->acceptor().accept( svcHandler->peer(),
                                             mPeerAcceptor.getHandle() );

        if ( svchandle == -1 )
        {
            // Close down handler to avoid memory leaks.

            cout
            << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::acceptSvcHandler"
            << ": "
            << "fail to accept"
            << endl;

            svcHandler->close();
            result = -1;
        }
    }

    if ( result != -1 )
    {
        cout
        << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::acceptSvcHandler"
        << ": "
        << "svchandle = "
        << svchandle
        << endl;
    }
    return result;
}

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
int Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::activateSvcHandler( SVC_HANDLER *svcHandler )
{

    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;

    int result = 0;
    if ( svcHandler == nullptr )
    {
        result = -1;
    }

    if ( result != -1 )
    {
        if ( svcHandler->open( (void*) this ) == -1 )
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

template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
PEER_ACCEPTOR& Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::acceptor() const
{
    cout
    << "Acceptor<SVC_HANDLER, PEER_ACCEPTOR>::"
    << __FUNCTION__
    << ": "
    << endl;
    return const_cast<PEER_ACCEPTOR&>( this->mPeerAcceptor );
}
} /* namespace AcceptorConnector_1_0 */

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ACCEPTOR_CPP_ */
