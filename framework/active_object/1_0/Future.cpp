
#ifndef FRAMEWORK_ACTIVE_OBJECT_1_0_FUTURE_CPP_
#define FRAMEWORK_ACTIVE_OBJECT_1_0_FUTURE_CPP_

#include "Future.hpp"
#include "framework/common/Logger.hpp"

namespace ActiveObject_1_0
{

template <typename T>
FutureObserver<T>::FutureObserver()
{
}

template <typename T>
FutureObserver<T>::~FutureObserver()
{
}

template <typename T>
FutureRep<T>::FutureRep()
    : mValue( nullptr )
{
}
template <typename T>
FutureRep<T>::~FutureRep()
{
    if ( this->mValue != nullptr )
    {
        delete this->mValue;
    }
}

template <typename T>
int FutureRep<T>::set( const T &r, Future<T> &caller )
{
    if ( this->mValue == nullptr )
    {
        unique_lock<std::mutex> lock( mValueReadyMutex );
        this->mValue = new T( r );

        for ( auto observer : mObserverCollection )
        {
            if ( observer != nullptr )
            {
                observer->update( caller );
            }
        }

        this->mValueReady.notify_all();
    }
    return 0;
}

template <typename T>
int FutureRep<T>::get( T &value, unsigned long int timeout )
{
    int result = 0;
    if ( this->mValue == nullptr )
    {
        unique_lock<mutex> lock( this->mValueReadyMutex );
        bool rc = mValueReady.wait_for( lock,
                                        chrono::milliseconds( timeout ),
                                        [this]
                                        { return ( this->mValue != nullptr ); } );

        if ( rc == false )
        {
            LOG_INFO( "time out" );
            result = -1;
        }
    }

    if ( this->mValue != nullptr )
    {
        value = *this->mValue;
    }
    return result;
}

template <typename T>
int FutureRep<T>::attach( FutureObserver<T> *observer,
                          Future<T> &caller )
{
    lock_guard<std::mutex> lock( mValueReadyMutex );

    int result = 0;

    // If the value is already produced, then notify observer
    if ( this->mValue == nullptr )
    {
        auto rc = this->mObserverCollection.insert( observer );
        result = rc.second ? 0 : -1;
    }
    else
    {
        observer->update( caller );
    }
    return result;
}

template <typename T>
int FutureRep<T>::detach( FutureObserver<T> *observer )
{
    lock_guard<std::mutex> lock( mValueReadyMutex );

    return this->mObserverCollection.remove( observer );
}

template <typename T>
int FutureRep<T>::ready() const
{
    return ( this->mValue != nullptr ) ? 0 : -1;
}

template <typename T>
Future<T>::Future()
    : mRep( make_shared<FutureRep<T>>() )
{
}

template <typename T>
Future<T>::~Future()
{
}

template <typename T>
int Future<T>::cancle()
{
    this->mRep = make_shared<FutureRep<T>>();
    return 0;
}

template <typename T>
int Future<T>::set( const T &r )
{
    return this->mRep->set( r, *this );
}

template <typename T>
int Future<T>::get( T &value, unsigned long int timeout ) const
{
    return this->mRep->get( value, timeout );
}

template <typename T>
int Future<T>::ready() const
{
    return this->mRep->ready();
}

template <class T>
int Future<T>::attach( FutureObserver<T> *observer )
{
    return this->mRep->attach( observer, *this );
}

template <class T>
int Future<T>::detach( FutureObserver<T> *observer )
{
    return this->mRep->detach( observer );
}

} //namespace ActiveObject_1_0

#endif //FRAMEWORK_ACTIVE_OBJECT_1_0_FUTURE_CPP_