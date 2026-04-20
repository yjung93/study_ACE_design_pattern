#ifndef APPLICATIONS_EXAMPLE_ACTIVE_OBJECT_ActObjMain_HPP_
#define APPLICATIONS_EXAMPLE_ACTIVE_OBJECT_ActObjMain_HPP_

#include <memory>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"
#include "ActObjClient.hpp"

namespace  ExActiveObject {

class ActObjAcceptor;

class ActObjMain: public Reactor_1_0::EventHandler {
public:
	ActObjMain(Reactor_1_0::Reactor *reactor, ActObjAcceptor &owner);
	virtual ~ActObjMain();

	void open();

private:
	  int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;

	  ActObjAcceptor *mOwner;
	  std::unique_ptr<ActObjClient> mActObjClient;
};

} /* namespace  ExActiveObject */

#endif /* APPLICATIONS_EXAMPLE_1_ActObjMain_HPP_ */
