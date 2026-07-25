
#include "framework/common/Logger.hpp"
#include "StreamHead.hpp"

namespace Stream_1_0
{

StreamHead::StreamHead()
{
}
StreamHead::~StreamHead()
{
}

int StreamHead::open( void *arg )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return 0;
}
int StreamHead::close()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return 0;
}
int StreamHead::put( string &msg )
{
    if ( isWriter() )
    {
        putNext( msg );
    }
    else
    {
        putQ( msg );
    }
    return 0;
}
int StreamHead::svc()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return 0;
}
} // namespace Stream_1_0