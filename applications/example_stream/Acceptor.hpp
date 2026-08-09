/*
 * Acceptor.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

#ifndef APPLICATIONS_EXAMPLE_STREAM_ACCEPTOR_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_ACCEPTOR_HPP_

#include <arpa/inet.h>
#include <memory>
#include <unordered_map>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"

#include "ServerEventHandler.hpp"

using namespace std;

namespace example_stream
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
    unordered_map<int, unique_ptr<ServerEventHandler>> mConnections;
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_ACCEPTOR_HPP_ */
