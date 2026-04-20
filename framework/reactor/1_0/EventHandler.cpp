/*
 * EventHandler.cpp
 *
 *  Created on: Jan 12, 2024
 *      Author: yjung93
 */
#include <iostream>
#include "EventHandler.hpp"

using namespace std;

namespace Reactor_1_0
{

EventHandler::EventHandler( Reactor *reactor )
    : mReactor( reactor ),
      mFd( INVALID_HANDLE )
{
    cout << "EventHandler::"
         << __FUNCTION__
         << ": "
         << endl;
}

EventHandler::~EventHandler()
{
    cout << "EventHandler::"
         << __FUNCTION__
         << ": "
         << endl;
}

int EventHandler::handleInput( int fd )
{
    cout << "EventHandler::"
         << __FUNCTION__
         << ": "
         << endl;
    return -1;
}

int EventHandler::handleOutput( int fd )
{
    cout << "EventHandler::"
         << __FUNCTION__
         << ": "
         << endl;
    return -1;
}

int EventHandler::handleClose( int fd )
{
    cout << "EventHandler::"
         << __FUNCTION__
         << ": "
         << endl;
    return 0;
}

Reactor *EventHandler::getReactor() const
{
    return mReactor;
}

void EventHandler::setReactor( Reactor *reactor )
{
    mReactor = reactor;
}

int EventHandler::getHandle() const
{
    return mFd;
}

void EventHandler::setHandle( int fd )
{
    cout << "EventHandler::"
         << __FUNCTION__
         << ": "
         << "fd: "
         << fd
         << endl;
    mFd = fd;
}

} /* namespace Reactor_1_0 */
