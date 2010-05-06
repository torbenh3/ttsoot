
#ifndef LV2EVENTS_HH
#define LV2EVENTS_HH

#include "lv2/event.h"

class LV2Event : public LV2_Event
{
    public:
        template<typename T>
	T get() {
	    char *c_ptr = (char *) this;
	    c_ptr += sizeof( LV2_Event );
	    return T( c_ptr, size, type );
	}
};

class LV2EventIter
{
    private:
	uint8_t *data;
	uint32_t   num;
    public:
	LV2EventIter( uint8_t *d, uint32_t n )
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

	LV2Event & operator*() {
	    return * reinterpret_cast< LV2Event *>(data);
	}

	LV2Event * operator->() {
	    return reinterpret_cast< LV2Event *>(data);
	}
};

class LV2EventBuf : public LV2_Event_Buffer
{
    private:
    public:
	LV2EventIter begin() {
	    return LV2EventIter( data, 0 );
	}

	LV2EventIter end() {
	    return LV2EventIter( 0, event_count );
	}
};

#endif
