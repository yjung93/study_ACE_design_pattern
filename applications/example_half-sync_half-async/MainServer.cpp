/*
 * main.cpp
 *
 *  Created on: Jan 22, 2024
 *      Author: yjung93
 */

#include "applications/example_half-sync_half-async/Acceptor.hpp"
#include <thread>

#include "framework/reactor/1_0/Reactor.hpp"
#include "framework/common/Logger.hpp"

void serverThreadFuncion( Reactor_1_0::Reactor *reactor )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    reactor->runReactorEventLoop();
}

int main( int argc, char **argv )
{

    Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance();

    ExHalfSyncAsync::Acceptor acceptor( reactor );

    std::thread serverThread( serverThreadFuncion, reactor );

    // Wait for the thread to finish execution
    if ( serverThread.joinable() )
    {
        serverThread.join();
    }

    return 0;
}
