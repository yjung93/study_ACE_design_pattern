#ifndef STREAM_1_0_MODULE_HPP_
#define STREAM_1_0_MODULE_HPP_

#include "framework/task/2_0/Task.hpp"

namespace Stream_1_0
{

class Module
{
  public:
    enum PipePare
    {
        PIPE_READ_SIDE,
        PIPE_WRITE_SIDE,
        PIPE_MAX
    };
    Module();
    virtual ~Module();

    Module( const string &name,
            Task_2_0::Task *writer = nullptr,
            Task_2_0::Task *reader = nullptr,
            void *a = nullptr );

    Task_2_0::Task *getReader();
    Task_2_0::Task *getWriter();

    int open( const string &name,
              Task_2_0::Task *writer = nullptr,
              Task_2_0::Task *reader = nullptr,
              void *a = nullptr );

    void setReader( Task_2_0::Task *reader );
    void setWriter( Task_2_0::Task *writer );

    void setName( const string &name );
    const string &getName();

    void setArg( void *arg );
    void *getArg();

    void setNext( Module *module );
    Module *getNext();

    int close();
    int closeImpl( PipePare which );

    void link( Module *module );

  private:
    string mName;
    Task_2_0::Task *mPipePaire[PIPE_MAX];
    Module *mNext;
    void *mArg;
};

} // namespace Stream_1_0
#endif //STREAM_1_0_MODULE_HPP_