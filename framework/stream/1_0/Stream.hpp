#ifndef STREAM_1_0_STREAM_HPP_
#define STREAM_1_0_STREAM_HPP_

#include <condition_variable>
#include "Module.hpp"

namespace Stream_1_0
{

const string MessageType = "message-type";
const string VendorData = "vendor-data";
const string Stop = "stop";

class Stream
{
  public:
    Stream( void *arg = nullptr, Module *head = nullptr, Module *tail = nullptr );
    ~Stream(); 
    int open( void *arg = 0,
              Module *head = 0,
              Module *tail = 0 );

    int close();

    int push( Module *module );
    int pop();
    int put( string &message );
    int get( string &message );

  protected:
    Module *mStreamHead;
    Module *mStreamTail;
    Stream *mLinkedUs;
    mutex mMutex;

  private:
    int pushModule( Module *newTop,
                    Module *currentTop = 0,
                    Module *head = 0 );

    condition_variable mFinalClose;
};

} // namespace Stream_1_0
#endif //STREAM_1_0_STREAM_HPP_