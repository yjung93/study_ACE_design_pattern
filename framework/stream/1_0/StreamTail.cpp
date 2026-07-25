
#include "framework/common/Logger.hpp"
#include "StreamTail.hpp"

namespace Stream_1_0
{

StreamTail::StreamTail()
{
}
StreamTail::~StreamTail()
{
}

int StreamTail::open( void *arg )
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return 0;
}
int StreamTail::close()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return 0;
}
int StreamTail::put( string &msg )
{
    return 0;
}
int StreamTail::svc()
{
    LOG_INFO( __FUNCTION__ << "() " << "called" );
    return 0;
}
} // namespace Stream_1_0