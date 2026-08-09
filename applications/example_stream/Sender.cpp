#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "Sender.hpp"
#include "DataType.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace example_stream
{

Sender::Sender( ServerEventHandler *owner, int handle )
    : mFd( handle ),
      mStopThread( false ),
      mOwner( owner )
{
    mName = "Sender";
    activate();
}
Sender::~Sender()
{
}

int Sender::put( string &msg )
{
    putQ( msg );
    return 0;
}

int Sender::close( u_long flags )
{
    LOG_INFO( "Sender::" << __FUNCTION__ << "()" << " called" );
    mStopThread = true;
    requestStop();
    return 0;
}

int Sender::svc()
{
    while ( mStopThread != true )
    {
        string message;
        int rc = getQ( message );
        if ( rc != -1 )
        {
            auto data = Framework_Common::Utils::parseKeyValueString( message );
            if ( data.find( Stream_1_0::MessageType ) != data.end() )
            {
                if ( data[Stream_1_0::MessageType] == Stream_1_0::VendorData )
                {
                    LOG_INFO( "Sender::"
                              << __FUNCTION__ << "() "
                              << "message: "
                              << data[ReplyMessage] );

                    send( mFd, data[ReplyMessage].c_str(), data[ReplyMessage].size(), 0 );
                    
                }
                else if ( data[Stream_1_0::MessageType] == Stream_1_0::Stop )
                {
                    LOG_INFO( "Sender::" << __FUNCTION__ << "() " << Stream_1_0::Stop );
                    mStopThread = true;  
                   
                }
                putNext( message );
            }
        }
    }
    return 0;
}

} /* namespace example_stream */