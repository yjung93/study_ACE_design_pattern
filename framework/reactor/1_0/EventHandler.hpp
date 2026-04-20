/*
 * EventHandler.hpp
 *
 *  Created on: Jan 12, 2024
 *      Author: yjung93
 */

#ifndef REACTOR_1_0_EVENTHANDLER_HPP_
#define REACTOR_1_0_EVENTHANDLER_HPP_

#include <cstdio>

namespace Reactor_1_0
{

// Forward declaration.
class Reactor;

static constexpr int INVALID_HANDLE = -1;

using ReactorMask = unsigned long;

class EventHandler
{
  public:
    enum
    {
        NULL_MASK = 0,
        READ_MASK = ( 1 << 0 ),
        ACCEPT_MASK = ( 1 << 3 ),
        CONNECT_MASK = ( 1 << 4 ),
        ALL_EVENTS_MASK = READ_MASK | ACCEPT_MASK | CONNECT_MASK
    };

    EventHandler( Reactor *reactor = nullptr );
    virtual ~EventHandler();

    virtual int handleInput( int fd = INVALID_HANDLE );
    virtual int handleOutput( int fd = INVALID_HANDLE );
    virtual int handleClose( int handle );

    Reactor *getReactor() const;
    void setReactor( Reactor *reactor );

    virtual int getHandle() const;
    void setHandle( int fd );

  private:
    Reactor *mReactor;
    int mFd;
};

} /* namespace Reactor_1_0 */

#endif /* REACTOR_1_0_EVENTHANDLER_HPP_ */
