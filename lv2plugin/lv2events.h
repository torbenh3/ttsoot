
#ifndef LV2EVENTS_HH
#define LV2EVENTS_HH

#include "lv2/event.h"

template<typename T>
class LV2Event : public LV2_Event
{
    private:
    public:
	T get_data() {
	    char *c_ptr = (char *) this;
	    c_ptr += sizeof( LV2_Event );
	    return T( c_ptr, size );
	}
};

template<typename T>
class LV2EventIter
{
    private:
	char *data;
	uint32_t   num;
    public:
	LV2EventIter( char *d, uint32_t n )
	   : data(d)
	   , num(n)
	{}

	bool operator != ( const LV2EventIter &other ) {
	    return num != other.num;
	}

	LV2EventIter & operator++() {
	    LV2_Event * event = (LV2_Event *) data;
	    uint32_t offset = sizeof(LV2_Event) + event->size;
	    offset = offset+3 & (~3);

	    data  += offset;
	    num += 1;

	    return *this;
	}

	LV2Event<T> & operator*() {
	    return * static_cast< LV2Event<T> >(data);
	}
};

template<typename T>
class LV2EventBuf : public LV2_Event_Buffer
{
    private:
    public:
	LV2EventIter<T> begin() {
	    return LV2EventIter<T>( data, 0 );
	}

	LV2EventIter<T> end() {
	    return LV2EventIter<T>( 0, event_count );
	}
};

#endif
