#include "framework/common/Logger.hpp"
#include "Module.hpp"

using namespace std;

namespace Stream_1_0
{

Module::Module()
    : mNext( nullptr ),
      mArg( nullptr )
{
    mPipePaire[PIPE_READ_SIDE] = nullptr;
    mPipePaire[PIPE_WRITE_SIDE] = nullptr;
}

Module::Module( const string &name,
                Task_2_0::Task *writer,
                Task_2_0::Task *reader,
                void *arg )
    : mNext( nullptr )
{
    mPipePaire[PIPE_READ_SIDE] = nullptr;
    mPipePaire[PIPE_WRITE_SIDE] = nullptr;
    if ( open( name, writer, reader, arg ) == -1 )
    {
        LOG_INFO( "fail to open, name=" << name );
    }
}

Module::~Module()
{
}

int Module::open( const string &name,
                  Task_2_0::Task *writer,
                  Task_2_0::Task *reader,
                  void *arg )
{
    mName = name;
    mArg = arg;

    closeImpl( PIPE_READ_SIDE );
    closeImpl( PIPE_WRITE_SIDE );

    if ( writer == nullptr )
    {
        writer = new Task_2_0::Task;
    }

    if ( reader == nullptr )
    {
        reader = new Task_2_0::Task;
    }

    if ( writer == nullptr || reader == nullptr )
    {
        closeImpl( PIPE_READ_SIDE );
        closeImpl( PIPE_WRITE_SIDE );
        return -1;
    }

    setReader( reader );
    setWriter( writer );

    reader->mModule = this;
    writer->mModule = this;

    return 0;
}

Task_2_0::Task *Module::getReader()
{
    return mPipePaire[PIPE_READ_SIDE];
}

Task_2_0::Task *Module::getWriter()
{
    return mPipePaire[PIPE_WRITE_SIDE];
}

void Module::setReader( Task_2_0::Task *reader )
{
    closeImpl( PIPE_READ_SIDE );
    mPipePaire[PIPE_READ_SIDE] = reader;

    if ( reader != nullptr )
    {
        reader->mModule = this;
        reader->mFlags |= Task_2_0::READER; // add the flag
    }
}
void Module::setWriter( Task_2_0::Task *writer )
{
    closeImpl( PIPE_WRITE_SIDE );
    mPipePaire[PIPE_WRITE_SIDE] = writer;

    if ( writer != nullptr )
    {
        writer->mModule = this;
        writer->mFlags &= ~Task_2_0::READER; // remove the flat
    }
}

void Module::setName( const string &name )
{
    mName = name;
}
const string &Module::getName()
{
    return mName;
}

void Module::setArg( void *arg )
{
    mArg = arg;
}
void *Module::getArg()
{
    return mArg;
}

void Module::setNext( Module *module )
{
    mNext = module;
}

Module *Module::getNext()
{
    return mNext;
}

Task_2_0::Task *Module::getSibling( Task_2_0::Task *org )
{
    LOG_INFO( "Module::" << __FUNCTION__ << "() " << "called" );
    if ( mPipePaire[PIPE_READ_SIDE] == org )
    {
        return mPipePaire[PIPE_WRITE_SIDE];
    }
    else if ( mPipePaire[PIPE_WRITE_SIDE] == org )
    {
        return mPipePaire[PIPE_READ_SIDE];
    }
    else
    {
        return nullptr;
    }
}

int Module::close()
{
    closeImpl( PIPE_READ_SIDE );
    closeImpl( PIPE_WRITE_SIDE );

    return 0;
}

int Module::closeImpl( PipePare which )
{
    return 0;
}

void Module::link( Module *module )
{
    setNext( module );
    getWriter()->setNext( module->getWriter() );
    module->getReader()->setNext( getReader() );
}

} // namespace Stream_1_0