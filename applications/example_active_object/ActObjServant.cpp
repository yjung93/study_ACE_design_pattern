
#include "ActObjServant.hpp"
#include <sys/socket.h>
#include <chrono>
#include <thread>
#include "framework/common/Logger.hpp"

namespace ExActiveObject
{
ActObjServant::ActObjServant()
{
    LOG_INFO( "called" );
}

string ActObjServant::RequestGetReturnMessage( const string &message )
{
    LOG_INFO( "message=" << message );

    string result = "Echo - " + message;

    this_thread::sleep_for( std::chrono::milliseconds( 1000 ) ); // simulate work load

    LOG_INFO( "process completed, result=" << result );

    return result;
}
} // namespace ExActiveObject
