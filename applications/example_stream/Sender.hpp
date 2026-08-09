#ifndef APPLICATIONS_EXAMPLE_STREAM_SENDER_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_SENDER_HPP_

/*
 * Acceptor.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

#include "framework/task/2_0/Task.hpp"
#include "framework/stream/1_0/Module.hpp"
#include "ServerEventHandler.hpp"
#include <atomic>

namespace example_stream
{

class Sender : public Task_2_0::Task
{
  public:
    Sender( ServerEventHandler* owner, int handle );
    ~Sender();

    int put( string &msg ) override;
    int svc() override;
    int close( u_long flags = 0 ) override;
    
  private:
    int mFd;
    std::atomic<bool> mStopThread;
    ServerEventHandler *mOwner;
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_SENDER_HPP_ */
