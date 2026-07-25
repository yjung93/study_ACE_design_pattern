#include <applications/example_acceptor_connector/ConnectorImpl.hpp>
#include <thread>

#include "framework/reactor/1_0/Reactor.hpp"
#include "applications/example_acceptor_connector/Client.hpp"
#include "framework/common/Logger.hpp"

using namespace ex_acceptor_connector;

void clientThreadFuncion( Reactor_1_0::Reactor *reactor )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );

    reactor->runReactorEventLoop();
}

int main()
{
    std::thread clientThread( clientThreadFuncion,
                              Reactor_1_0::Reactor::getInstance() );

    Client client;
    client.initialize();

    bool loop = true;
    while ( loop )
    {
        std::string input;

        std::cout << "Enter Message : ";
        std::getline( std::cin, input );

        if ( input.length() > 0 )
        {

            if ( input == "exit" )
            {
                loop = false;
            }else
            {
                client.sendMessage( input );
            }
        }

    }
    client.finish();

    // Wait for the thread to finish execution
    if ( clientThread.joinable() )
    {
        clientThread.join();
    }

    return 0;
}

