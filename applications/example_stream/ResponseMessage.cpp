#include "framework/common/Logger.hpp"
#include "framework/common/Utils.hpp"
#include "framework/stream/1_0/Stream.hpp"

#include "ResponseMessage.hpp"
#include "DataType.hpp"

namespace example_stream
{

ResponseMessage::ResponseMessage()
{
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

int ResponseMessage::svc()
{
    bool stop = false;
    while ( stop != true )
    {
        string message;
        int rc = getQ( message );
        if ( rc != -1 )
        {
            auto dataReceived = Framework_Common::Utils::parseKeyValueString( message );
            if ( dataReceived.find( Stream_1_0::MessageType ) != dataReceived.end() )
            {
                if ( dataReceived[Stream_1_0::MessageType] == Stream_1_0::VendorData )
                {
                    map<string, string> dataResponse;
                    dataResponse = {
                        { Stream_1_0::MessageType, Stream_1_0::VendorData },
                        { RecievedMessage, dataReceived[RecievedMessage] },
                        { ReplyMessage, "" }
                    };

                    stringstream ssResp;
                    ssResp << "Echo-" << dataReceived[RecievedMessage];
                    dataResponse[ReplyMessage] = ssResp.str();

                    message = Framework_Common::Utils::formatKeyValue( dataResponse );
                    LOG_INFO( "ResponseMessage::" << __FUNCTION__ << "() " << "new Message: " << message );
                }
                else if ( dataReceived[Stream_1_0::MessageType] == Stream_1_0::Stop )
                {
                    stop = true;
                }
            }
            Task *sibling = getSibling();
            if ( sibling != nullptr )
            {
                sibling->put( message );
            }
        }
    }
    return 0;
}

} /* namespace example_stream */