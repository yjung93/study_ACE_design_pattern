#ifndef APPLICATIONS_EXAMPLE_STREAM_RESPONSE_MESSAGE_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_RESPONSE_MESSAGE_HPP_

/*
 * ResponseMessage.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

#include "framework/task/2_0/Task.hpp"
#include <atomic>

namespace example_stream
{

class ResponseMessage : public Task_2_0::Task
{
  public:
    ResponseMessage();
    ~ResponseMessage();

    int put( string &msg ) override;
    int svc() override;
    int close( u_long flags = 0 ) override;

  private:
    std::atomic<bool> mStopThread;
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_RESPONSE_MESSAGE_HPP_ */
