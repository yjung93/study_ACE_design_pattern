#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "Receiver.hpp"
#include "DataType.hpp"

#include <algorithm>

namespace example_stream
{

Receiver::Receiver()
    : mStopThread( false )
{
    mName = "Receiver";
    activate();
}
Receiver::~Receiver()
{
}

int Receiver::put( string &msg )
{
    putQ( msg );
    return 0;
}

int Receiver::close( u_long flags )
{
    LOG_INFO( "Receiver::" << __FUNCTION__ << "()" << " called" );
    mStopThread = true;
    requestStop();
    return 0;
}

int Receiver::svc()
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
                    LOG_INFO( "Receiver::"
                              << __FUNCTION__ << "() "
                              << "message: "
                              << data[PacketMessage] );

                    Packet packet;
                    packet.index = stoi( data[PacketSequenceIndex] );
                    packet.message = data[PacketMessage];

                    int packetSequenceSize = stoi( data[PacketSequenceSize] );

                    mPacketBuffer.push_back( packet );

                    if ( mPacketBuffer.size() == static_cast<size_t>( packetSequenceSize ) )
                    {
                        string completedMessage;
                        sort( mPacketBuffer.begin(),
                              mPacketBuffer.end(),
                              []( const Packet &a, const Packet &b )
                              { return a.index < b.index; } );

                        for ( auto itBuffer : mPacketBuffer )
                        {
                            // LOG_INFO( "Receiver::"
                            //           << __FUNCTION__ << "() "
                            //           << "itBuffer.index: "
                            //           << itBuffer.index
                            //           << "itBuffer.message: "
                            //           << itBuffer.message );

                            completedMessage.append( itBuffer.message );
                            completedMessage.append( " " );
                        }
                        mPacketBuffer.clear();

                        LOG_INFO( "Receiver::"
                                  << __FUNCTION__ << "() "
                                  << "completedMessage: "
                                  << completedMessage );

                        map<string, string> data;

                        data = {
                            { Stream_1_0::MessageType, Stream_1_0::VendorData },
                            { ReceivedMessage, completedMessage },
                        };

                        message = Framework_Common::Utils::formatKeyValue( data );
                        putNext( message );
                    }
                }
                else if ( data[Stream_1_0::MessageType] == Stream_1_0::Stop )
                {
                    LOG_INFO( "Receiver::" << __FUNCTION__ << "() " << Stream_1_0::Stop );
                    mStopThread = true;
                    putNext( message );
                }
            }
        }
    }
    return 0;
}

} /* namespace example_stream */