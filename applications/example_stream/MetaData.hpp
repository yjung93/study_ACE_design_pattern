#ifndef APPLICATIONS_EXAMPLE_STREAM_METADATA_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_METADATA_HPP_

/*
 * Acceptor.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */

 #include "framework/task/2_0/Task.hpp"
#include "framework/stream/1_0/Module.hpp"
#include <atomic>

namespace example_stream
{

class MetaData : public Task_2_0::Task
{
  public:
    MetaData();
    ~MetaData();

    int put( string &msg ) override;
    int svc() override;
    int close( u_long flags = 0 ) override;
    
  private:
    std::atomic<bool> mStopThread;
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_METADATA_HPP_ */
