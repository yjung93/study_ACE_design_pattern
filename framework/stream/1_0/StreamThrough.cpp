#include "framework/common/Logger.hpp"
#include "StreamThrough.hpp"

namespace Stream_1_0
{

StreamThrough::StreamThrough()
{
}
StreamThrough::~StreamThrough()
{
}

int StreamThrough::open( void *arg )
{
    LOG_INFO( "StreamThrough::" << __FUNCTION__ << "() " << "called" );
    return 0;
}
int StreamThrough::close( u_long flags )
{
    LOG_INFO( "StreamThrough::" << __FUNCTION__ << "() " << "called" );
    return 0;
}
int StreamThrough::put( string &msg )
{
    LOG_INFO( "StreamThrough::" << __FUNCTION__ << "() " << "message: " << msg );
    return putNext( msg );
}
int StreamThrough::svc()
{
    LOG_INFO( "StreamThrough::" << __FUNCTION__ << "() " << "called" );
    return 0;
}
} // namespace Stream_1_0