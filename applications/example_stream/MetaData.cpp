#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "MetaData.hpp"
#include "DataType.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace example_stream
{

MetaData::MetaData()
{
    activate();
}
MetaData::~MetaData()
{
}

int MetaData::put( string &msg )
{
    putQ( msg );
    return 0;
}

int MetaData::svc()
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
                    stringstream ssReply;
                    ssReply << data[ReplyMessage];

                    //-----------------------------------
                    // Time info
                    //-----------------------------------

                    // Get current time
                    auto now = chrono::system_clock::now();
                    auto time_t = chrono::system_clock::to_time_t( now );
                    stringstream ssTime;
                    ssTime << put_time( std::localtime( &time_t ), "%Y-%m-%d %H:%M:%S" );

                    ssReply << ", Received Time: " << ssTime.str();

                    //-----------------------------------
                    // Server info
                    //-----------------------------------

                    // Get server information
                    char hostname[256];
                    gethostname( hostname, sizeof( hostname ) );

                    // Get IP address from hostname
                    struct addrinfo hints = {}, *result = nullptr;
                    hints.ai_family = AF_INET;
                    hints.ai_socktype = SOCK_STREAM;

                    string serverIp = "127.0.0.1";
                    if ( getaddrinfo( hostname, nullptr, &hints, &result ) == 0 && result != nullptr )
                    {
                        struct sockaddr_in *addr = (struct sockaddr_in *) result->ai_addr;
                        serverIp = inet_ntoa( addr->sin_addr );
                        freeaddrinfo( result );
                    }

                    //-----------------------------------
                    // Add metadata to replay-message
                    //-----------------------------------E

                    // server ip to server info
                    stringstream ssServerinfo;
                    ssServerinfo << "Stream Framework example server, IP: " << serverIp;

                    // server info to reply-message
                    ssReply << ", Server info: " << ssServerinfo.str();

                    // reply-message to map data
                    data[ReplyMessage] = ssReply.str();

                    message = Framework_Common::Utils::formatKeyValue( data );
                    LOG_INFO( "MetaData::" << __FUNCTION__ << "() " << "message: " << message );
                }
                else if ( data[Stream_1_0::MessageType] == Stream_1_0::Stop )
                {
                    stop = true;
                }
                putNext( message );
            }
        }
    }
    return 0;
}

} /* namespace example_stream */