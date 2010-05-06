/*
 *  ttsoot - templated DSP graph composition library
 *
 *  Copyright (C) 2010  Torben Hohn <torbenh@gmx.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LV2ATOM_HH
#define LV2ATOM_HH

#include "lv2plugin/lv2events.h"
#include "lv2/atom.h"

#include <initializer_list>

class LV2Atom
{
    public:
	uint8_t *data;
	uint32_t size;
	uint32_t type;

        bool type_valid;

	LV2Atom( uint8_t *d, uint32_t s, uint32_t t )
	    : data(d)
	    , size(s)
            , type(t)
            , type_valid(true)
	{}

        LV2Atom()
            : type_valid(false)
        {}
};

template <typename T>
class LV2AtomSimple : public LV2Atom
{
    private:
        T val;
        static int cache_id;

    public:
	LV2AtomSimple( uint8_t *d, uint32_t s, uint32_t t )
            : LV2Atom(d,s,t)
        {}

        LV2AtomSimple( T v )
            : LV2Atom()
            , val(v)
        {}
        
        operator T ()
        {
            if (type_valid)
                return * (T *) data;
            else
                return val;
        }

        LV2AtomSimple & operator = ( T v )
        {
            if (type_valid)
                * (T *) data = v;
            else
                val = v;

            return *this;
        }

        size_t write_body( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            * (T *) dst = val;
            return sizeof(T);
        }

        size_t write( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2_Atom *atom = (LV2_Atom *) dst;
            atom->type = uri_cache[cache_id];
            atom->size = write_body( atom->body, uri_cache );
            
            return ((atom->size+3)&(~3)) + 2 * sizeof(uint16_t);
        }
};

typedef LV2AtomSimple<float> LV2AtomFloat32;

struct LV2Triple
{
    uint32_t subject;
    uint32_t predicate;
    LV2_Atom object;
};

template<typename T>
class LV2AtomTriple : public LV2Atom
{
    private:
        uint32_t subject;
        uint32_t predicate;
        T object;

    public:
        static int cache_id;

        LV2AtomTriple( uint8_t *d, uint32_t s, uint32_t t )
            : LV2Atom(d,s,t)
        {}

        LV2AtomTriple( uint32_t s, uint32_t p, T o )
            : LV2Atom()
            , subject(s)
            , predicate(p)
            , object(o)
        {}

        uint32_t get_subject()
        {
            if (!type_valid)
                return subject;
            else 
            {
                LV2Triple *triple = (LV2Triple *) data;
                return triple->subject;
            }
        }

        uint32_t get_predicate()
        {
            if (!type_valid)
                return predicate;
            else 
            {
                LV2Triple *triple = (LV2Triple *) data;
                return triple->predicate;
            }
        }

        T get_object()
        {
            if (!type_valid)
                return object;
            else 
            {
                LV2Triple *triple = (LV2Triple *) data;
                return T( triple->object.body, triple->object.size, triple->object.type );
            }
        }

        size_t write_body( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2Triple *triple = (LV2Triple *) dst;
            triple->subject = subject;
            triple->predicate = predicate;
            int sz = object.write( &triple->object, uri_cache );

            sz += 2 * sizeof(uint32_t);
            
            return sz;
        }

        size_t write( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2_Atom *atom = (LV2_Atom *) dst;
            atom->type = uri_cache[cache_id];

            int sz = write_body( atom->body, uri_cache );

            atom->size = 2 * sizeof(uint32_t) + sz;
            
            return ((atom->size+3)&(~3)) + 2 * sizeof(uint16_t);
        }

};

template <typename ... Args>
class LV2Triples;

template <typename T, typename ... Args>
class LV2Triples<T, Args...>
{
    private:
        T head;
        LV2Triples<Args...> tail;

    public:
        static int cache_id;

        LV2Triples(T h, Args & ... t)
            : head(h)
            , tail(t...)
        {}

        size_t write_body( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            int sz = head.write_body( dst, uri_cache );
            sz += tail.write_body( dst + sz, uri_cache );

            return sz;
        }

        size_t write( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2_Atom *atom = (LV2_Atom *) dst;
            atom->type = uri_cache[cache_id];

            int sz = write_body( atom->body, uri_cache );
            atom->size = sz;

            return ((atom->size+3)&(~3)) + 2 * sizeof(uint16_t);
        }

};

template <typename T>
class LV2Triples<T>
{
    private:
        T head;

    public:
        static int cache_id;

        LV2Triples(T h)
            : head(h)
        {}

        size_t write_body( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            int sz = head.write_body( dst, uri_cache );

            return sz;
        }

        size_t write( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2_Atom *atom = (LV2_Atom *) dst;
            atom->type = uri_cache[cache_id];

            int sz = write_body( atom->body, uri_cache );
            atom->size = sz;

            return ((atom->size+3)&(~3)) + 2 * sizeof(uint16_t);
        }

};

class LV2TriplesIter
{
    private:
	uint8_t *data;
    public:
	LV2TriplesIter( uint8_t *d )
	   : data(d)
	{}

	bool operator != ( const LV2TriplesIter &other ) {
	    return data != other.data;
	}

	LV2TriplesIter & operator++() {
	    LV2Triple * triple = (LV2Triple *) data;
	    uint32_t offset = sizeof(LV2Triple) + triple->object.size;
	    offset = offset+3 & (~3);

	    data  += offset;

	    return *this;
	}

	LV2AtomTriple<LV2Atom> operator*() {
	    return LV2AtomTriple<LV2Atom>(data, 0, 0);
	}
};

template <>
class LV2Triples<> : public LV2Atom
{
    public:
	LV2Triples( uint8_t *d, uint32_t s, uint32_t t )
            : LV2Atom(d,s,t)
        {}

        LV2TriplesIter begin() { return LV2TriplesIter( data ); }
        LV2TriplesIter end() { return LV2TriplesIter( data+size ); }
};

template <typename T>
class LV2AtomVector : public LV2Atom
{
    private:
        std::initializer_list<T> init_list;
        static int cache_id;

    public:
        LV2AtomVector( uint8_t *d, uint32_t s, uint32_t t )
            : LV2Atom(d,s,t)
        {}

        LV2AtomVector( std::initializer_list<T> il )
            : LV2Atom()
            , init_list(il)
        {}

        size_t esize()
        {
            LV2_Vector_Body *vec = (LV2_Vector_Body *) data;
            return size - 2*sizeof(uint16_t) / vec->elem_count;
        }

        size_t etype()
        {
            LV2_Vector_Body *vec = (LV2_Vector_Body *) data;
            return vec->elem_type;
        }

        size_t vsize()
        {
            LV2_Vector_Body *vec = (LV2_Vector_Body *) data;
            return vec->elem_count;
        }

        T get_elem( size_t index )
        {
            LV2_Vector_Body *vec = (LV2_Vector_Body *) data;
            uint8_t *elem = vec->elems + index * esize();
            return T(elem, esize(), etype());
        }


        size_t write_body( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2_Vector_Body *vec = (LV2Triple *) dst;
            vec->elem_count = init_list.size();
            vec->elem_type  = uri_cache[T::cache_id];
            
            size_t sz = sizeof(LV2_Vector_Body);
            uint8_t * write_ptr = vec->elems;

            for (T * e = init_list.begin(); e != init_list.end(); e++)
            {
                size_t add_size = e->write_body (write_ptr, uri_cache);
                sz += add_size;
                write_ptr += add_size;
            }

            return sz;
        }

        size_t write( uint8_t *dst, const std::vector<uint32_t> & uri_cache )
        {
            LV2_Atom *atom = (LV2_Atom *) dst;

            atom->type = uri_cache[cache_id];
            atom->size = write_body( atom->body, uri_cache );
            
            return ((atom->size+3)&(~3)) + 2 * sizeof(uint16_t);
        }
        
};


#endif
