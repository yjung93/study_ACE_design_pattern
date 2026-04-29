
#include "applications/example_active_object/ActObjAcceptor.hpp"
#include <thread>

#include "framework/reactor/1_0/Reactor.hpp"
#include "framework/common/Logger.hpp"

void serverThreadFunction( Reactor_1_0::Reactor *reactor )
{
    LOG_INFO( "called" );

    reactor->runReactorEventLoop();
}

int main( int argc, char **argv )
{

    Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance();

    ExActiveObject::ActObjAcceptor acceptor( reactor );

    std::thread serverThread( serverThreadFunction, reactor );

    // Wait for the thread to finish execution
    if ( serverThread.joinable() )
    {
        serverThread.join();
    }

    return 0;
}
