#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>

#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "DataType.hpp"

constexpr int PORT = 8080;

vector<string> convertToPacket( const string input )
{
    stringstream ssInput( input );
    string word;
    vector<string> vecWord;
    vector<string> vecPacket;

    while ( ssInput >> word )
    {
        vecWord.push_back( word );
    }
    map<string, string> data;

    data = {
        { Stream_1_0::MessageType, Stream_1_0::VendorData },
        { example_stream::PacketSequenceSize, "" },
        { example_stream::PacketSequenceIndex, "" },
        { example_stream::PacketMessage, "" }
    };

    int index = 0;
    for ( auto itWord : vecWord )
    {
        data[example_stream::PacketSequenceSize] = to_string( vecWord.size() );
        data[example_stream::PacketSequenceIndex] = to_string( index++ );
        data[example_stream::PacketMessage] = itWord;

        // format to map to string and push to list
        vecPacket.push_back( Framework_Common::Utils::formatKeyValue( data ) );
    }

    return vecPacket;
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    const int bufferLength = 1024;

    if ( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        LOG_ERROR( "socket creation error" );
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( PORT );

    // Convert IPv4 and IPv6 addresses from text to binary form
    if ( inet_pton( AF_INET, "127.0.0.1", &serv_addr.sin_addr ) <= 0 )
    {
        LOG_ERROR( "invalid address / address not supported" );
        return -1;
    }

    if ( connect( sock, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 )
    {
        LOG_ERROR( "connection failed" );
        return -1;
    }

    bool loop = true;
    while ( loop )
    {
        string input;
        char buffer[bufferLength] = { 0 };

        LOG_INFO( "Enter Message : " );
        getline( cin, input );

        if ( input.length() > 0 )
        {
            if ( input == "exit" )
            {
                loop = false;
            }
            else
            {
                vector<string> packets = convertToPacket( input );

                // shuffle the sequence of packet to simulate the multi-path of network
                random_device rd;
                mt19937 rng( rd() );
                shuffle( packets.begin(), packets.end(), rng );

                // send packets
                for ( auto itPacket : packets )
                {
                    // '\n' delimits messages so the receiver can reassemble packets split/merged by TCP
                    string framedPacket = itPacket + "\n";
                    send( sock, framedPacket.c_str(), framedPacket.size(), 0 );
                    LOG_INFO( "sent: " << itPacket );
                }

                // receive message server replies
                int valRead = read( sock, buffer, bufferLength );
                if ( valRead == -1 || valRead == 0 )
                {
                    LOG_ERROR( "read returned: " << valRead );
                    loop = false;
                }
                else
                {
                    LOG_INFO( "received: " << buffer );
                }
            }
        }
    }

    close( sock );

    return 0;
}
