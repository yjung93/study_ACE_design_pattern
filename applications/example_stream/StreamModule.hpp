#ifndef APPLICATIONS_EXAMPLE_STREAM_STREAM_MODULE_HPP_
#define APPLICATIONS_EXAMPLE_STREAM_STREAM_MODULE_HPP_

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
template <class TASK>
class StreamModule : public Stream_1_0::Module
{
  public:
    StreamModule( const string &name, TASK *task )
    {
        open( name,
              task,
              nullptr,
              nullptr );
    }

    ~StreamModule() = default;
};

} /* namespace example_stream */

#endif /* APPLICATIONS_EXAMPLE_STREAM_STREAM_MODULE_HPP_ */
