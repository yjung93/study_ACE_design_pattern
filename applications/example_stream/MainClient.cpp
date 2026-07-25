#include "framework/common/Logger.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

constexpr int PORT = 8080;

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    const int bufferLength = 1024;

    if ( (sock = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
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

    if ( connect( sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr) ) < 0 )
    {
        LOG_ERROR( "connection failed" );
        return -1;
    }

    bool loop = true;
    while ( loop )
    {
        std::string input;
        char buffer[bufferLength] =
        { 0 };

        LOG_INFO( "Enter Message : " );
        std::getline( std::cin, input );

        if ( input.length() > 0 )
        {

            if ( input == "exit" )
            {
                loop = false;
            }else
            {

                send( sock, input.c_str(), input.size(), 0 );
                LOG_INFO( "sent: " << input );

                int valRead = read( sock, buffer, bufferLength );
                if ( valRead == -1 || valRead == 0 )
                {
                    LOG_ERROR( "read returned: " << valRead );
                    loop = false;
                }else
                {
                    LOG_INFO( "received: " << buffer );
                }

            }
        }

    }

    close( sock );

    return 0;
}
