#ifndef TASK_1_0_EVENTHANDLER_HPP_
#define TASK_1_0_EVENTHANDLER_HPP_

/*
 * Task.hpp
 *
 *  Created on: Nov 04, 2025
 *      Author: yjung93
 */
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

namespace Task_1_0
{

class Task
{
  public:
    Task();
    ~Task();
    virtual int open( void *args = 0 );
    virtual int svc();
    
    int putQ( const string &message );
    int getQ( string &message );
    int activate();
    int svcRun();


  private:
    vector<string> mMessageQueue;
    mutex mQueueMutex;
    condition_variable mQueueCondition;
    thread mWorkerThread;
    bool mStopRequested;
};

} // namespace Task_1_0

#endif /* TASK_1_0_EVENTHANDLER_HPP_ */
