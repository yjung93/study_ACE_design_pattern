/*
 * main.cpp
 *
 *  Created on: Jan 22, 2024
 *      Author: yjung93
 */

#include "applications/example_proactor/Acceptor.hpp"
#include <thread>

#include "framework/reactor/1_0/Reactor.hpp"
#include "framework/common/Logger.hpp"

void reactorThreadFuncion( Reactor_1_0::Reactor *reactor )
{
    LOG_INFO( "called" );

    reactor->runReactorEventLoop();
}

void proactorThreadFuncion( Proactor_1_0::Proactor *proactor )
{
    LOG_INFO( "called" );

    proactor->proactorRunEventLoop();
}

int main( int argc, char **argv )
{
    Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance();

    example_proactor::Acceptor acceptor( reactor );

    std::thread reactorThread( reactorThreadFuncion, reactor );
    std::thread proactorThread( proactorThreadFuncion, Proactor_1_0::Proactor::getInstance() );

    // Wait for the thread to finish execution
    if ( reactorThread.joinable() )
    {
        reactorThread.join();
    }
    
    if ( proactorThread.joinable() )
    {
        proactorThread.join();
    }

    return 0;
}
