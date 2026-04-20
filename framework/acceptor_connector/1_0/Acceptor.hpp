/*
 * Acceptor.hpp
 *
 *  Created on: Jan 14, 2025
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ACCEPTOR_HPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ACCEPTOR_HPP_

#include <arpa/inet.h>
#include <iostream>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"

using namespace std;

namespace AcceptorConnector_1_0
{
template<typename SVC_HANDLER, typename PEER_ACCEPTOR>
class Acceptor: public Reactor_1_0::EventHandler

{
public:

    Acceptor( Reactor_1_0::Reactor *reactor = 0, int flags = 0 );
    Acceptor( const typename PEER_ACCEPTOR::PEER_ADDR &localAddr, Reactor_1_0::Reactor *reactor =
                    0, int flags = 0 );
    virtual ~Acceptor();

    virtual int open( const typename PEER_ACCEPTOR::PEER_ADDR &localAddr, Reactor_1_0::Reactor *reactor =
                    Reactor_1_0::Reactor::getInstance(), int flags = 0 );
    virtual int close();

    virtual PEER_ACCEPTOR& acceptor() const;

protected:

    virtual int makeSvcHandler( SVC_HANDLER *&svcHandler );
    virtual int acceptSvcHandler( SVC_HANDLER *svcHandler );
    virtual int activateSvcHandler( SVC_HANDLER *svcHandler );

    int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
    int handleClose( int handle = Reactor_1_0::INVALID_HANDLE ) override;

private:
    // Concrete factory for accepting connections from clients...
    PEER_ACCEPTOR mPeerAcceptor;

    // Needed to reopen the socket if {accept} fails.
    typename PEER_ACCEPTOR::PEER_ADDR mPeerAcceptorAddr;

};

} /* namespace AcceptorConnector_1_0 */

// for splitting the template class definition into .cpp
#include "framework/acceptor_connector/1_0/Acceptor.cpp"

#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ACCEPTOR_HPP_ */
