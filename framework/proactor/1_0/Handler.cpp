#include "Handler.hpp"
#include "Proactor.hpp"
#include "framework/reactor/1_0/EventHandler.hpp"

namespace Proactor_1_0
{

Handler::Handler()
    : mProactor( 0 ), mHandle( Reactor_1_0::INVALID_HANDLE )
{
    Handler::Proxy *p = new Handler::Proxy( this );
    this->mProxy.reset( p );
}

Handler::Handler( Proactor *d )
    : mProactor( d ), mHandle( Reactor_1_0::INVALID_HANDLE )
{
    Handler::Proxy *p = new Handler::Proxy( this );
    this->mProxy.reset( p );
}

Handler::~Handler()
{
    Handler::Proxy *p = this->mProxy.get();
    if ( p )
        p->reset();
}

int Handler::handle()
{
    return mHandle;
}

Proactor *Handler::proactor()
{
    return this->mProactor;
}

void Handler::proactor( Proactor *p )
{
    this->mProactor = p;
}

Handler::ProxyPtr &Handler::proxy()
{
    return mProxy;
}
} // namespace Proactor_1_0