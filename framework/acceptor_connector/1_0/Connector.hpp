/*
 * Connector.hpp
 *
 *  Created on: Jan 14, 2025
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_CONNECTOR_HPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_CONNECTOR_HPP_

#include "framework/acceptor_connector/1_0/Config.hpp"
#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"

using namespace std;

namespace AcceptorConnector_1_0
{

template <class SVC_HANDLER>
class ConnectorBase
{
  public:
    virtual ~ConnectorBase() = default;

    /// Initialize the Svc_Handler.
    virtual void initializeSvcHandler( int handle, SVC_HANDLER *svc_handler ) = 0;
};

template <class SVC_HANDLER>
class NonBlockingConnectHandler : public Reactor_1_0::EventHandler
{
  public:
    /// Constructor.
    NonBlockingConnectHandler( ConnectorBase<SVC_HANDLER> &connector, SVC_HANDLER *sh );

    /// Destructor.
    ~NonBlockingConnectHandler();

    bool close( SVC_HANDLER *&sh );

    /// Get SVC_HANDLER.
    SVC_HANDLER *svcHandler();

    /// Called by ACE_Reactor when asynchronous connections fail.
    virtual int handleInput( int handle );

    /// Called by ACE_Dev_Poll_Reactor when asynchronous connections fail.
    virtual int handleClose( int handle );

    /// Called by ACE_Reactor when asynchronous connections succeed.
    virtual int handleOutput( int handle );

    /// Called by ACE_Reactor when asynchronous connections suceeds (on
    /// some platforms only).
    virtual int handleException( int handle );

  private:
    ConnectorBase<SVC_HANDLER> &mConnector;

    /// Associated SVC_HANDLER.
    SVC_HANDLER *mSvcHandler;
};

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
class Connector : public ConnectorBase<SVC_HANDLER>, public Reactor_1_0::EventHandler
{
  public:
    typedef typename SVC_HANDLER::AddrType AddrType;
    typedef PEER_CONNECTOR ConnectorType;
    typedef SVC_HANDLER HandlerType;
    typedef typename SVC_HANDLER::StreamType StreamType;
    typedef typename PEER_CONNECTOR::PEER_ADDR PeerAddrType;
    typedef NonBlockingConnectHandler<SVC_HANDLER> NBCH;

    Connector( Reactor_1_0::Reactor *reactor = 0, int flags = 0 );
    virtual ~Connector();

    virtual int open( Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance(), int flags = 0 );
    bool close( SVC_HANDLER *&sh );

    virtual int connect( SVC_HANDLER *&svcHandler, const typename PEER_CONNECTOR::PEER_ADDR &remoteAddr, const timeval *timeOut = 0, int flags = 0 );

    void initializeSvcHandler( int handle, SVC_HANDLER *svc_handler ) override;

  protected:
    virtual int makeSvcHandler( SVC_HANDLER *&svcHandler );
    virtual int connectSvcHandler( SVC_HANDLER *svcHandler, const typename PEER_CONNECTOR::PEER_ADDR remoteAddr, const timeval *timeOut = 0, int flags = 0 );
    virtual int activateSvcHandler( SVC_HANDLER *svcHandler );

    int nonblockingConnect( SVC_HANDLER * );

    int handleInput( int fd = INVALID_HANDLE ) override;
    int handleClose( int handle = INVALID_HANDLE ) override;

  private:
    PEER_CONNECTOR mConnector;

    int mFlags;
};

} /* namespace AcceptorConnector_1_0 */

// for splitting the template class definition into .cpp
#include "framework/acceptor_connector/1_0/Connector.cpp"

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_CONNECTOR_HPP_ */
