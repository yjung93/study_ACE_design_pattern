#ifndef FRAMEWORK_COMMON_ERRNO_GUARD_HPP_
#define FRAMEWORK_COMMON_ERRNO_GUARD_HPP_

// RAII guard: saves errno on construction, restores on destruction.
// Prevents cleanup calls (close, etc.) from clobbering the original error.

namespace Framework_Common
{
class ErrnoGuard
{
  public:
    explicit ErrnoGuard();
    ~ErrnoGuard();

    ErrnoGuard( const ErrnoGuard & ) = delete;
    ErrnoGuard &operator=( const ErrnoGuard & ) = delete;

    ErrnoGuard &operator=( int v );
    operator int() const;

  private:
    int mSaved;
};
} // namespace Framework_Common

#endif //FRAMEWORK_COMMON_ERRNO_GUARD_HPP_

