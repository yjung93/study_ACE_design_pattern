/*
 * Acceptor.hpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#ifndef APPLICATIONS_EXAMPLE_REACTOR_ACCEPTOR_HPP_
#define APPLICATIONS_EXAMPLE_REACTOR_ACCEPTOR_HPP_

#include <arpa/inet.h>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"

namespace example_reactor
{

class Acceptor: public Reactor_1_0::EventHandler
{
public:
    Acceptor( Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance() );
    virtual ~Acceptor();

    void open();
    int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;

private:
    struct sockaddr_in mAddress;
};

} /* namespace example_reactor */

#endif /* APPLICATIONS_EXAMPLE_REACTOR_ACCEPTOR_HPP_ */
