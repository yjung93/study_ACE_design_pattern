#ifndef APPLICATIONS_EXAMPLE_ACTIVE_OBJECT_ACCEPTOR_HPP_
#define APPLICATIONS_EXAMPLE_ACTIVE_OBJECT_ACCEPTOR_HPP_

#include <arpa/inet.h>
#include <memory>
#include <unordered_map>

#include "framework/reactor/1_0/EventHandler.hpp"
#include "framework/reactor/1_0/Reactor.hpp"
#include "applications/example_active_object/ActObjMain.hpp"

namespace ExActiveObject
{

class ActObjAcceptor : public Reactor_1_0::EventHandler
{
  public:
    ActObjAcceptor( Reactor_1_0::Reactor *reactor = Reactor_1_0::Reactor::getInstance() );
    virtual ~ActObjAcceptor();

    void open();
    int handleInput( int fd = Reactor_1_0::INVALID_HANDLE ) override;
    void removeConnection( int fd );

  private:
    struct sockaddr_in mAddress;
    std::unordered_map<int, std::unique_ptr<ActObjMain>> mConnections;
};

} /* namespace  ExActiveObject */

#endif /* APPLICATIONS_EXAMPLE_REACTOR_ACCEPTOR_HPP_ */
