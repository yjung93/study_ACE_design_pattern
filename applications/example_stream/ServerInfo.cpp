#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "ServerInfo.hpp"
#include "DataType.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace example_stream
{

ServerInfo::ServerInfo()
{
    activate();
}
ServerInfo::~ServerInfo()
{
}

int ServerInfo::put( string &msg )
{
    putQ( msg );
    return 0;
}

int ServerInfo::svc()
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

                    
                    stringstream ssServerinfo;
                    ssServerinfo << "Stream Framework example server, IP: " << serverIp;

                    stringstream ssReply;
                    ssReply << data[ReplyMessage] << ", Server info: " << ssServerinfo.str();
                    data[ReplyMessage] = ssReply.str();

                    string newMessage = Framework_Common::Utils::formatKeyValue( data );
                    LOG_INFO( "ServerInfo::" << __FUNCTION__ << "() " << "newMessage: " << newMessage );

                    putNext( newMessage );
                }
            }
        }
    }
    return 0;
}

} /* namespace example_stream */