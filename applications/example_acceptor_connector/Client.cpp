/*
 * Client.cpp
 *
 *  Created on: Jun 8, 2025
 *      Author: yjung93
 */

#include <sys/socket.h>
#include <arpa/inet.h>

#include "Client.hpp"
#include "framework/common/Logger.hpp"

namespace ex_acceptor_connector
{

constexpr int PORT = 8080;

Client::Client()
{
    // TODO Auto-generated constructor stub
}

Client::~Client()
{
    // TODO Auto-generated destructor stub
}

void Client::initialize()
{
    AcceptorConnector_1_0::SockConnector::PEER_ADDR peerAddr;
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_addr.s_addr = INADDR_ANY;
    peerAddr.sin_port = htons( PORT );

    // Convert IPv4 and IPv6 addresses from text to binary form
    if ( ::inet_pton( AF_INET, "127.0.0.1", &peerAddr.sin_addr ) <= 0 )
    {
        LOG_ERROR( "invalid address / address not supported" );
    }

    OutputHandler *pOutputHandler = &mOutputHandler;

    // nonbock mode
    int result = mConnector.connect( pOutputHandler, peerAddr, 0, O_NONBLOCK );

    /*
    // block mode 
    int timeout_ms = 3000;
    
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = ( timeout_ms % 1000 ) * 1000;

    int result = mConnector.connect( pOutputHandler, peerAddr, &tv, 0 );
    */

    if ( result == -1 )
    {
        LOG_INFO( "connect result="
                  << result
                  << ", errno="
                  << errno );
    }
}

void Client::finish()
{
}

void Client::sendMessage( string message )
{
    mOutputHandler.sendMessage( message );
}
} // namespace ex_acceptor_connector
/* namespace v_1_1 */
