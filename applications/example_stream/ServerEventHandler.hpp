#ifndef APPLICATIONS_EXAMPLE_1_ServerEventHandler_HPP_
#define APPLICATIONS_EXAMPLE_1_ServerEventHandler_HPP_

/*
 * ServerEventHandler.h
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"
#include "framework/stream/1_0/Stream.hpp"

namespace example_stream
{

class ServerEventHandler : public Reactor_1_0::EventHandler
{
  public:
    ServerEventHandler( Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance() );
    virtual ~ServerEventHandler();

    void open();

  private:
    int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
    Stream_1_0::Stream mStream;
    Stream_1_0::Module *mBottomModule;
    string mRecvBuffer; // holds partial/leftover bytes across reads until a full '\n'-terminated message arrives
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_1_ServerEventHandler_HPP_ */
