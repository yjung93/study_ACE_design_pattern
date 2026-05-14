#include <errno.h>
#include "ErrnoGuard.hpp"
namespace Framework_Common
{
ErrnoGuard::ErrnoGuard()
    : mSaved( errno )
{
}
ErrnoGuard::~ErrnoGuard()
{
    errno = mSaved;
}
ErrnoGuard &ErrnoGuard::operator=( int v )
{
    mSaved = v;
    return *this;
}
ErrnoGuard::operator int() const
{
    return mSaved;
}
} // namespace Framework_Common