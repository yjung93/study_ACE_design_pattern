#ifndef STREAM_1_0_STREAM_MODULE_HPP_
#define STREAM_1_0_STREAM_MODULE_HPP_

#include "framework/task/2_0/Task.hpp"
#include "Module.hpp"
#include "Stream.hpp"

namespace Stream_1_0
{
class SteamHead : public Task_2_0::Task
{
  public:
    SteamHead();
    ~SteamHead();

    int open( void *args = 0 ) override;
    int close( u_long flags = 0 ) override;
    int put( string &msg ) override;
    int svc() override;

  private:
};

class SteamTail : public Task_2_0::Task
{
  public:
    SteamTail();
    ~SteamTail();

    int open( void *args = 0 ) override;
    int close( u_long flags = 0 ) override;
    int put( string &msg ) override;
    int svc() override;

  private:
};

class ThruTask : public Task_2_0::Task
{
  public:
    ThruTask();
    ~ThruTask();

    int open( void *args = 0 ) override;
    int close( u_long flags = 0 ) override;
    int put( string &msg ) override;
    int svc() override;

  private:
};
} // namespace Stream_1_0

#endif //STREAM_1_0_STREAM_MODULE_HPP_