/*
 * ServerEventHandler.h
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#ifndef APPLICATIONS_EXAMPLE_1_ServerEventHandler_HPP_
#define APPLICATIONS_EXAMPLE_1_ServerEventHandler_HPP_

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"


namespace example_reactor {

class ServerEventHandler: public Reactor_1_0::EventHandler {
public:
	ServerEventHandler(Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance());
	virtual ~ServerEventHandler();

	void open();

private:
	  int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
};

} /* namespace example_reactor */

#endif /* APPLICATIONS_EXAMPLE_1_ServerEventHandler_HPP_ */
