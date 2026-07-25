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
            auto data = Framework_Common::Utils::parseKeyValueString( message );
            if ( data.find( Stream_1_0::MessageType ) != data.end() )
            {
                if ( data[Stream_1_0::MessageType] == Stream_1_0::VendorData )
                {
                    stringstream ssRply;
                    ssRply << "Echo-" << data[RecievedMessage];
                    data[ReplyMessage] = ssRply.str();

                    message = Framework_Common::Utils::formatKeyValue( data );
                    LOG_INFO( "ResponseMessage::" << __FUNCTION__ << "() " << "new Message: " << message );
                }
                if ( data[Stream_1_0::MessageType] == Stream_1_0::Stop )
                {
                }
            }
            putNext( message );
        }
    }
    return 0;
}

} /* namespace example_stream */