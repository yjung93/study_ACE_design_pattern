#ifndef APPLICATIONS_EXAMPLE_STREAM_RECEIVER_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_RECEIVER_HPP_

/*
 * Receiver.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */
#include <atomic>
#include "framework/task/2_0/Task.hpp"

namespace example_stream
{

class Receiver : public Task_2_0::Task
{
  public:
    struct Packet
    {
        int index;
        string message;
    };

    Receiver();
    ~Receiver();

    int put( string &msg ) override;
    int svc() override;
    int close( u_long flags = 0 ) override;

  private:
    vector<Packet> mPacketBuffer;
    atomic<bool> mStopThread;
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_RECEIVER_HPP_ */
