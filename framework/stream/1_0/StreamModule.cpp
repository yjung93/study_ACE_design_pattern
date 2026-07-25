#include "framework/common/Logger.hpp"
#include "Module.hpp"
#include "StreamModule.hpp"

namespace Stream_1_0
{

SteamHead::SteamHead()
{
}

SteamHead::~SteamHead()
{
}

int SteamHead::open( void *args )
{
    LOG_INFO( "is  called" );
    return 0;
}
int SteamHead::close( u_long flags )
{
    LOG_INFO( "is  called" );
    return 0;
}
int SteamHead::put( string &msg )
{
    LOG_INFO( "is  called" );
    return 0;
}
int SteamHead::svc()
{
    LOG_INFO( "is  called" );
    return 0;
}



SteamTail::SteamTail()
{
}

SteamTail::~SteamTail()
{
}

int SteamTail::open( void *args )
{
    LOG_INFO( "is  called" );
    return 0;
}
int SteamTail::close( u_long flags )
{
    LOG_INFO( "is  called" );
    return 0;
}
int SteamTail::put( string &msg )
{
    LOG_INFO( "is  called" );
    return 0;
}
int SteamTail::svc()
{
    LOG_INFO( "is  called" );
    return 0;
}



ThruTask::ThruTask()
{
}

ThruTask::~ThruTask()
{
}

int ThruTask::open( void *args )
{
    LOG_INFO( "is  called" );
    return 0;
}
int ThruTask::close( u_long flags )
{
    LOG_INFO( "is  called" );
    return 0;
}
int ThruTask::put( string &msg )
{
    LOG_INFO( "is  called" );
    return 0;
}
int ThruTask::svc()
{
    LOG_INFO( "is  called" );
    return 0;
}

} // namespace Stream_1_0