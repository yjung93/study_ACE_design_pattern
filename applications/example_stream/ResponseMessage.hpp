#ifndef APPLICATIONS_EXAMPLE_STREAM_RESPONSE_MESSAGE_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_RESPONSE_MESSAGE_HPP_

/*
 * Acceptor.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

#include "framework/task/2_0/Task.hpp"
#include "framework/stream/1_0/Module.hpp"

namespace example_stream
{

class ResponseMessage : public Task_2_0::Task
{
  public:
    ResponseMessage();
    ~ResponseMessage();

    int put( string &msg ) override;
    int svc() override;

  private:
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_RESPONSE_MESSAGE_HPP_ */
