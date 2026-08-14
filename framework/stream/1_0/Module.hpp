#ifndef STREAM_1_0_MODULE_HPP_
#define STREAM_1_0_MODULE_HPP_

#include "framework/task/2_0/Task.hpp"

namespace Stream_1_0
{

class Module
{
  public:
    enum PipePair
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
    
    Task_2_0::Task *getSibling( Task_2_0::Task *org );

    int close();
    int closeImpl( PipePair which );

    void link( Module *module );

  private:
    string mName;
    Task_2_0::Task *mPipePair[PIPE_MAX];
    Module *mNext;
    void *mArg;
};

} // namespace Stream_1_0
#endif //STREAM_1_0_MODULE_HPP_