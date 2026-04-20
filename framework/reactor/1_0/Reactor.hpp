/*
 * Reactor.hpp
 *
 *  Created on: Jan 12, 2024
 *      Author: yjung93
 */

#ifndef REACTOR_1_0_REACTOR_HPP_
#define REACTOR_1_0_REACTOR_HPP_

#include <string>
#include <cstdio>
#include <unordered_map>
#include <mutex>
#include <fcntl.h>
#include <unistd.h>

#include "EventHandler.hpp"

namespace Reactor_1_0
{

class Reactor
{
  public:
    struct EvHandlerInfo
    {
        ReactorMask mask;
        EventHandler *evHandler;
    };

    typedef std::unordered_map<int, EvHandlerInfo> EventHandlerRepository;

    static Reactor *getInstance();
    int runReactorEventLoop();
    int handleEvents();
    int registerHandler( EventHandler *event_handler, ReactorMask mask );
    int removeHandler( EventHandler *event_handler, ReactorMask mask );
    void dbgRepository( std::string title );

  protected:
    static Reactor *mInstance;
    EventHandlerRepository mEventHandlerRepository;

  private:
    Reactor();
    virtual ~Reactor();
    void notifySelectLoop();

    std::recursive_mutex mMutexRepository;
    int mWakeupPipe[2]; // 0: read end, 1: write end
};

} /* namespace Reactor_1_0 */

#endif /* REACTOR_1_0_REACTOR_HPP_ */
