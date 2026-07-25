/*
 * EventHandler.cpp
 *
 *  Created on: Jan 12, 2024
 *      Author: yjung93
 */
#include "EventHandler.hpp"
#include "framework/common/Logger.hpp"

namespace Reactor_1_0
{

EventHandler::EventHandler( Reactor *reactor )
    : mReactor( reactor ),
      mFd( INVALID_HANDLE )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

EventHandler::~EventHandler()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
}

int EventHandler::handleInput( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return -1;
}

int EventHandler::handleOutput( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return -1;
}

int EventHandler::handleClose( int fd )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
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
    LOG_INFO( "fd=" << fd );
    mFd = fd;
}

} /* namespace Reactor_1_0 */
