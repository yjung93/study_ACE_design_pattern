/*
 * Acceptor.hpp
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#ifndef APPLICATIONS_example_proactor_ACCEPTOR_HPP_
#define APPLICATIONS_example_proactor_ACCEPTOR_HPP_

#include <arpa/inet.h>
#include <memory>
#include <unordered_map>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"

#include "ServerEventHandler.hpp"

namespace example_proactor
{

class Acceptor : public Reactor_1_0::EventHandler
{
  public:
    Acceptor( Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance() );
    virtual ~Acceptor();

    void open();
    int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
    void removeConnection( int fd );

  private:
    struct sockaddr_in mAddress;
    std::unordered_map<int, std::unique_ptr<ServerEventHandler>> mConnections;
};

} /* namespace example_proactor */

#endif /* APPLICATIONS_example_proactor_ACCEPTOR_HPP_ */
