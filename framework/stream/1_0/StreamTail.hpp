#ifndef STREAM_1_0_STREAM_TAIL_HPP_
#define STREAM_1_0_STREAM_TAIL_HPP_

#include "framework/task/2_0/Task.hpp"

namespace Stream_1_0
{

class StreamTail : public Task_2_0::Task
{
  public:
    StreamTail();
    virtual ~StreamTail();

    int open( void *arg = 0 ) override;
    int svc() override;

    virtual int close();
    virtual int put( string &msg );

  private:
};
} // namespace Stream_1_0

#endif // STREAM_1_0_STREAM_TAIL_HPP_