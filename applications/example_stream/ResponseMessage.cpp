#include <sstream>
#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "ResponseMessage.hpp"
#include "DataType.hpp"

namespace example_stream
{

ResponseMessage::ResponseMessage()
    : mStopThread( false )
{
    stringstream ss;
    static int cnt = 1;
    ss << "ResponseMessage cnt=" << cnt++; // debug
    mName = ss.str();
    activate();
}
ResponseMessage::~ResponseMessage()
{
}

int ResponseMessage::put( string &msg )
{
    putQ( msg );
    return 0;
}

int ResponseMessage::close( u_long flags )
{
    LOG_INFO( "ResponseMessage::" << __FUNCTION__ << "()" << " called" );
    mStopThread = true;
    requestStop();
    return 0;
}

int ResponseMessage::svc()
{
    while ( mStopThread != true )
    {
        string message;
        int rc = getQ( message );
        LOG_INFO( "ResponseMessage::" << __FUNCTION__ << "() getQ message = " << message );
        if ( rc != -1 )
        {
            auto dataReceived = Framework_Common::Utils::parseKeyValueString( message );
            if ( dataReceived.find( Stream_1_0::MessageType ) != dataReceived.end() )
            {
                if ( dataReceived[Stream_1_0::MessageType] == Stream_1_0::VendorData )
                {
                    map<string, string> mapResponse;
                    mapResponse = {
                        { Stream_1_0::MessageType, Stream_1_0::VendorData },
                        { ReceivedMessage, dataReceived[ReceivedMessage] },
                        { ReplyMessage, "" }
                    };

                    stringstream ssResp;
                    ssResp << "Echo-" << dataReceived[ReceivedMessage];
                    mapResponse[ReplyMessage] = ssResp.str();

                    string strResponse = Framework_Common::Utils::formatKeyValue( mapResponse );
                    LOG_INFO( "ResponseMessage::" << __FUNCTION__ << "() " << "new Message: " << strResponse );

                    Task *sibling = getSibling();
                    if ( sibling != nullptr )
                    {
                        sibling->put( strResponse );
                    }
                }
                else if ( dataReceived[Stream_1_0::MessageType] == Stream_1_0::Stop )
                {
                    LOG_INFO( "ResponseMessage::" << __FUNCTION__ << "() " << Stream_1_0::Stop );
                    mStopThread = true;
                }

                putNext( message );
            }
        }
    }
    return 0;
}

} /* namespace example_stream */