/*
 * main.cpp
 *
 *  Created on: Jan 22, 2024
 *      Author: yjung93
 */

#include <applications/example_acceptor_connector/AcceptorImpl.hpp>
#include <iostream>
#include <thread>

#include "framework/reactor/1_0/Reactor.hpp"

using namespace std;
using namespace ex_acceptor_connector;

void serverThreadFuncion( Reactor_1_0::Reactor *reactor )
{
    cout << "serverThreadFuncion"
         << endl;

    reactor->runReactorEventLoop();
}

int main( int argc, char **argv )
{

    AcceptorImpl acceptor( Reactor_1_0::Reactor::getInstance() );

    std::thread serverThread( serverThreadFuncion,
                              Reactor_1_0::Reactor::getInstance() );

    // Wait for the thread to finish execution
    if ( serverThread.joinable() )
    {
        serverThread.join();
    }

    return 0;
}
