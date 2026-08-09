#ifndef TASK_2_0_HPP_
#define TASK_2_0_HPP_

/*
 * Task.hpp
 *
 *  Created on: June 30, 2026
 *      Author: yjung93
 */
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

namespace Stream_1_0
{
class Module;
} // namespace Stream_1_0

namespace Task_2_0
{

enum
{
    // Identifies a Task as being the "reader" in a Module.
    READER = 01,
    // Just flush data messages in the queue.
    FLUSHDATA = 02,
    // Flush all messages in the Queue.
    FLUSHALL = 04,
    // Flush read queue
    FLUSHR = 010,
    // Flush write queue
    FLUSHW = 020,
    // Flush both queues
    FLUSHRW = 030
};

class Task
{
  public:
    Task();
    virtual ~Task();

    int putQ( const string &message );
    int getQ( string &message );
    int activate();
    int svcRun();

    void wait();
    void requestStop();
    
    virtual int open( void *args = 0 );
    virtual int close( u_long flags = 0 );
    virtual int put( string &msg );
    virtual int svc();

    int moduleClosed();
    Task *getNext();
    void setNext( Task *task );

    Task *getSibling();

    int putNext( string &msg );
    bool isWriter();


  protected:
    u_long mFlags;
    string mName;  

  private:
    friend Stream_1_0::Module;

    vector<string> mMessageQueue;
    mutex mQueueMutex;
    condition_variable mQueueCondition;
    thread mWorkerThread;
    bool mStopRequested;

    Stream_1_0::Module *mModule;
    Task *mNext;
};

} // namespace Task_2_0

#endif /* TASK_2_0_EVENTHANDLER_HPP_ */
