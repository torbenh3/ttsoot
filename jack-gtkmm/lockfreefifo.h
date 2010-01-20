#ifndef LOCKFREEFIFO_H
#define LOCKFREEFIFO_H

#include "stdlib.h" 

template<typename T>
class LockFreeFIFO
{
    private:
	T **_data;
	volatile size_t	_write_ptr;
	volatile size_t _read_ptr;
	size_t _size;
	size_t _size_mask;
    public:
	LockFreeFIFO( size_t size = 128 )
	{
	    unsigned int power_of_two;

	    for (power_of_two = 1; (1U << power_of_two) < size; power_of_two++);
	    _size = 1 << power_of_two;
	    _size_mask = _size - 1;

	    _data = new T* [size];
	    _write_ptr = 0;
	    _read_ptr  = 0;
	}

	~LockFreeFIFO()
	{
	    delete _data;
	}

	T *pop()
	{
	    size_t r = _read_ptr;
	    size_t w = _write_ptr;
	    T *retval;

	    if (r == w)
		return 0;

	    retval= _data[r];
	    r += 1;
	    r &= _size_mask;
	    _read_ptr = r;

	    return retval;
	}

	bool push( T *item )
	{
	    size_t r = _read_ptr;
	    size_t w = _write_ptr;

	    if (item == 0)
		return 0;

	    if (w<r)
		w += _size;

	    if ((w-r) > (_size-2))
		return 0;
	    
	    _data[w&_size_mask] = item;

	    w += 1;
	    w &= _size_mask;

	    _write_ptr = w;
	    return 1;
	}
};


#endif
