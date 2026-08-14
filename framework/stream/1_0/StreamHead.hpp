#ifndef STREAM_1_0_STREAM_HEAD_HPP_
#define STREAM_1_0_STREAM_HEAD_HPP_

#include "framework/task/2_0/Task.hpp"

namespace Stream_1_0
{

class StreamHead : public Task_2_0::Task
{
  public:
    StreamHead();
    virtual ~StreamHead();

    int open( void *arg = 0 ) override;
    int svc() override;

    int close( u_long flags = 0 ) override;
    int put( string &msg ) override;

  private:
};
} // namespace Stream_1_0

#endif // STREAM_1_0_STREAM_HEAD_HPP_