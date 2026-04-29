/*
 * ServiceHandler.cpp
 *
 *  Created on: Jan 14, 2025
 *      Author: yjung93
 */

#ifndef FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ServiceHandler_CPP_
#define FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ServiceHandler_CPP_

#include "ServiceHandler.hpp"
#include "framework/common/Logger.hpp"
namespace AcceptorConnector_1_0
{

template<typename PEER_STREAM>
ServiceHandler<PEER_STREAM>::ServiceHandler( Reactor_1_0::Reactor *reactor ) :
                Reactor_1_0::EventHandler( reactor )
{
    LOG_INFO( "called" );
}
template<typename PEER_STREAM>
ServiceHandler<PEER_STREAM>::~ServiceHandler()
{
    LOG_INFO( "called" );
}

template<typename PEER_STREAM>
PEER_STREAM& ServiceHandler<PEER_STREAM>::peer() const
{
    LOG_INFO( "called" );
    return (PEER_STREAM&) this->mPeer;
}
template<typename PEER_STREAM>
int ServiceHandler<PEER_STREAM>::getHandle() const
{
    return mPeer.getHandle();
}

template<typename PEER_STREAM>
int ServiceHandler<PEER_STREAM>::open( void* )
{
    LOG_INFO( "called" );
    int result = 0;
    if ( getReactor() != nullptr )
    {
        LOG_INFO( "register Handler" );
        int rc = getReactor()->registerHandler( this, Reactor_1_0::EventHandler::READ_MASK );
        if ( rc == -1 )
        {
            result = -1;
        }
    }
    return result;
}

} // namespace AcceptorConnector_1_0


#endif /* FRAMEWORK_ACCEPTOR_CONNECTOR_1_0_ServiceHandler_HPP_ */
