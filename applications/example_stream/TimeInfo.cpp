#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "TimeInfo.hpp"
#include "DataType.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace example_stream
{

TimeInfo::TimeInfo()
{
    activate();
}
TimeInfo::~TimeInfo()
{
}

int TimeInfo::put( string &msg )
{
    putQ( msg );
    return 0;
}

int TimeInfo::svc()
{
    bool stop = false;
    while ( stop != true )
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
                    // Get current time
                    auto now = chrono::system_clock::now();
                    auto time_t = chrono::system_clock::to_time_t( now );
                    stringstream ssTime;
                    ssTime << put_time( std::localtime( &time_t ), "%Y-%m-%d %H:%M:%S" );

                    stringstream ssReply;
                    ssReply << data[ReplyMessage] << ", Received Time: " << ssTime.str();
                    data[ReplyMessage] = ssReply.str();

                    string newMessage = Framework_Common::Utils::formatKeyValue( data );
                    LOG_INFO( "TimeInfo::" << __FUNCTION__ << "() " << "newMessage: " << newMessage );

                    putNext( newMessage );
                }
            }
        }
    }
    return 0;
}

} /* namespace example_stream */