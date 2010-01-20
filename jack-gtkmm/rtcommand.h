#ifndef RTCOMMAND_H
#define RTCOMMAND_H

#include "lockfreefifo.h"
#include <unistd.h>
#include <list>

class RTCommand
{
    public:
	virtual void execute() = 0;
	virtual void cleanup() = 0;
	virtual ~RTCommand() {}
};

class RTCommandEngine 
{
    private:
	LockFreeFIFO<RTCommand> in_fifo;
	LockFreeFIFO<RTCommand> out_fifo;

    public:
	RTCommandEngine() {}

	void push_command( RTCommand *cmd )
	{
	    while(1)
	    {
		RTCommand *out_cmd = out_fifo.pop();
		if( out_cmd == 0 )
		    break;

		out_cmd->cleanup();
		delete out_cmd;
	    }

	    while (!in_fifo.push( cmd )) usleep(1000);
	}

	void wait_command( RTCommand *cmd )
	{
            bool end_loop = false;
            while(!end_loop)
            {
		RTCommand *out_cmd = out_fifo.pop();
		if( out_cmd == 0 ) {
                    usleep(50);
		    continue;
                }
                if( out_cmd == cmd ) 
                    end_loop = true;

		out_cmd->cleanup();
		delete out_cmd;
            }
	}

        void push_command_and_wait( RTCommand *cmd )
        {
            push_command( cmd );
	    wait_command( cmd );
        }

	void process_rt_commands()
	{
	    while(1)
	    {
		RTCommand *the_cmd = in_fifo.pop();
		if( the_cmd == 0 )
		    break;

		the_cmd->execute();
		out_fifo.push( the_cmd );
	    }
	}
};

template<typename T>
class RTListAddCommand : public RTCommand
{
    private:
	std::list<T *> _splice_list;
	std::list<T *> *_dst_list;
    public: 
	RTListAddCommand( std::list<T *> *dst_list, T *element )
	{
	    _dst_list = dst_list;
	    _splice_list.push_back( element );
	}
	void execute()
	{
	    _dst_list->splice( _dst_list->end(), _splice_list );
	}
	void cleanup()
	{
	    // no cleanup necessary. 
	}
};

template<typename T>
class RTListRemCommand : public RTCommand
{
    private:
	std::list<T *> _splice_list;
	std::list<T *> *_dst_list;
	T *_element_to_remove;

    public: 
	RTListRemCommand( std::list<T *> *dst_list, T *element )
	{
	    _dst_list = dst_list;
	    _element_to_remove =  element;
	}

	void execute()
	{
	    for (typename std::list<T *>::iterator i = _dst_list->begin(); i != _dst_list->end(); i++ ) {
		if( *i == _element_to_remove ) {
		    _splice_list.splice( _splice_list.begin(), *_dst_list, i );
		    break;
		}
	    }
	}

	void cleanup()
	{
	    for (typename std::list<T *>::iterator i = _splice_list.begin(); i != _splice_list.end(); i++ ) {
		delete *i;
	    }
	}
};

template<typename T>
class RTList
{
    private:
	RTCommandEngine &_engine;
	std::list<T *> the_actual_list;
	std::list<T *> shadow_list;
    public:
	typedef typename std::list<T *>::iterator iterator;
	RTList( RTCommandEngine &engine )
	    : _engine( engine ) 
	{
	}

	void add( T *to_add )
	{
	    RTListAddCommand<T> *cmd = new RTListAddCommand<T>( &the_actual_list, to_add );
	    _engine.push_command( cmd );
	    shadow_list.push_back( to_add );
	}

	void rem( T *to_rem )
	{
	    shadow_list.remove( to_rem );

	    RTListRemCommand<T> *cmd = new RTListRemCommand<T>( &the_actual_list, to_rem );
	    _engine.push_command_and_wait( cmd );
	}

	void clear()
	{
	    if( shadow_list.empty() )
		return;

	    RTListRemCommand<T> *cmd;
	    while( ! shadow_list.empty() )
	    {
		typename std::list<T *>::iterator it = shadow_list.begin();
		cmd = new RTListRemCommand<T>( &the_actual_list, *it );
		shadow_list.erase( it );
		_engine.push_command( cmd );
	    }
	    _engine.wait_command( cmd );
	}

	iterator rt_begin() { return the_actual_list.begin(); }
	iterator rt_end() { return the_actual_list.end(); }
        int rt_size() { return the_actual_list.size(); }

	iterator begin() { return shadow_list.begin(); }
	iterator end() { return shadow_list.end(); }
        int size() { return shadow_list.size(); }
};

#endif
