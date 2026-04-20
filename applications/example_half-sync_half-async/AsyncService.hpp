/*
 * AsyncService.h
 *
 *  Created on: Jan 23, 2024
 *      Author: yjung93
 */

#ifndef APPLICATIONS_EXAMPLE_1_AsyncService_HPP_
#define APPLICATIONS_EXAMPLE_1_AsyncService_HPP_

#include <memory>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"
#include "SyncService.hpp"

namespace  ExHalfSyncAsync {

class Acceptor;

class AsyncService: public Reactor_1_0::EventHandler {
public:
	AsyncService(Reactor_1_0::Reactor *reactor, Acceptor &owner);
	virtual ~AsyncService();

	void open();

private:
	  int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;

	  Acceptor *mOwner;
	  std::unique_ptr<SyncService> mSyncService;
};

} /* namespace  ExHalfSyncAsync */

#endif /* APPLICATIONS_EXAMPLE_1_AsyncService_HPP_ */
