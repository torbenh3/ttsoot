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
#ifndef YC20_HH
#define YC20_HH

typedef Chain< BiQuadLP, BiQuadHP, Gain> FBank;

typedef VectorCascade< FlipFlop, FBank, FBank, FBank, FBank, FBank, FBank, FBank, FBank > FlipFlopFilterCascade;

template<int cc6, int cc5, int cc4, int cc3, int cc2, int cc1, int cc0>
class YC20FilterBank : public FlipFlopFilterCascade
{
    private:
	paramMap local_map;
	float passive_rc_frequency( float r, float c )
	{
	    return 1.0/(2.0 * M_PI * r * (c / 1000000.0) );
	}

    public:
	YC20FilterBank() {
	    FlipFlopFilterCascade::register_params( local_map, "" );
	    local_map.dump();

	    // TODO: This is still a guess. It could probably be looked up on the schematics
	    float next_stage_resistance = 180000.0 + 1.0/( 1.0/10000.0 + 1.0/100000.0 );

	    // next stage resistance affects the high pass cutoff point
	    float R2 = 1.0 / ( 1.0/next_stage_resistance + 1.0/180000.0 );
	    float R1 = 1.0 / ( 1.0/next_stage_resistance + 1.0/82000.0  );
	    float R0 = 1.0 / ( 1.0/next_stage_resistance + 1.0/56000.0  );
			
	    float C0 = (float)cc0 / 10000.0;
	    float C1 = (float)cc1 / 10000.0;
	    float C2 = (float)cc2 / 10000.0;
	    float C3 = (float)cc3 / 10000.0;
	    float C4 = (float)cc4 / 10000.0;
	    float C5 = (float)cc5 / 10000.0;
	    float C6 = (float)cc6 / 10000.0;

	    local_map["/0/0/bw"] = 3.0;
	    local_map["/0/0/cut"] = 15000;
	    local_map["/0/1/bw"] = 3.0;
	    local_map["/0/1/cut"] = passive_rc_frequency( next_stage_resistance , 0.039 );
	    local_map["/0/2/mod/gain"] = 1.0;
	    local_map["/1/0/0/bw"] = 3.0;
	    local_map["/1/0/0/cut"] = passive_rc_frequency( 15000, C6 );
	    local_map["/1/0/1/bw"] = 3.0;
	    local_map["/1/0/1/cut"] = passive_rc_frequency( next_stage_resistance + 15000, 0.039 );
	    local_map["/1/0/2/mod/gain"] = 1.0;
	    local_map["/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/0/0/cut"] = passive_rc_frequency( 15000, C5 );
	    local_map["/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/0/1/cut"] = passive_rc_frequency( next_stage_resistance + 15000, 0.039 );
	    local_map["/1/1/0/2/mod/gain"] = 1.0;
	    local_map["/1/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/1/0/0/cut"] = passive_rc_frequency( 15000, C4 );
	    local_map["/1/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/1/0/1/cut"] = passive_rc_frequency( next_stage_resistance + 15000, 0.039 );
	    local_map["/1/1/1/0/2/mod/gain"] = 1.0;
	    local_map["/1/1/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/1/1/0/0/cut"] = passive_rc_frequency( 15000, C3 );
	    local_map["/1/1/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/1/1/0/1/cut"] = passive_rc_frequency( next_stage_resistance + 15000, 0.039 );
	    local_map["/1/1/1/1/0/2/mod/gain"] = 1.0;
	    local_map["/1/1/1/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/1/1/1/0/0/cut"] = passive_rc_frequency( 15000, C2 );
	    local_map["/1/1/1/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/1/1/1/0/1/cut"] = passive_rc_frequency( R2 + 15000, 0.039 );
	    local_map["/1/1/1/1/1/0/2/mod/gain"] = 1.0;
	    local_map["/1/1/1/1/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/1/1/1/1/0/0/cut"] = passive_rc_frequency( 15000, C1 );
	    local_map["/1/1/1/1/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/1/1/1/1/0/1/cut"] = passive_rc_frequency( R1 + 15000, 0.039 );
	    local_map["/1/1/1/1/1/1/0/2/mod/gain"] = 1.0;
	    local_map["/1/1/1/1/1/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/1/1/1/1/1/0/0/cut"] = passive_rc_frequency( 15000, C0 );
	    local_map["/1/1/1/1/1/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/1/1/1/1/1/0/1/cut"] = passive_rc_frequency( R0 + 15000, 0.039 );
	    local_map["/1/1/1/1/1/1/1/0/2/mod/gain"] = 1.0;
	}
	virtual void register_params( paramMap &map, std::string prefix ) { }

};

template<int c6, int c5, int c4, int c3, int c2, int c1, int c0>
class yc20voice : public Sequence< SlowVarBLSaw, YC20FilterBank<c6, c5, c4, c3, c2, c1, c0> > {};

typedef yc20voice<47, 100, 220, 470, 820, 1200, 1500> yc20voice_I1;
typedef yc20voice<39, 82,  180, 390, 680, 1000, 1500> yc20voice_I2;
typedef yc20voice<27, 56,  120, 270, 560,  820, 1200> yc20voice_I3;

typedef VVectorGen< yc20voice_I1, yc20voice_I1, yc20voice_I1, yc20voice_I1, yc20voice_I2, yc20voice_I2, yc20voice_I2, yc20voice_I2, yc20voice_I3, yc20voice_I3, yc20voice_I3, yc20voice_I3 > voice_vector; 

class yc20busbar : public Block
{
    public:
	typedef fvec<8*12> input_t;
	typedef fvec<7> output_t;

	fvec<61> keys;

	inline fvec<7> process( fvec<8*12> s )
	{
	    fvec<7> retval;
	    float acc=0.0f;
	    // XXX:
	    for( int i=0; i<12; i++ )
		acc += (s[i*8] * keys[i]);

	    retval[0] = acc;
	    acc = 0.0f;
	    for( int i=0; i<12; i++ )
		acc += (s[i*8+1] * keys[i+12]);
	    retval[1] = acc;
	    retval[2] = acc;
	    retval[3] = acc;
	    return retval;

	    // XXX:
	    for( int oct=0; oct < 4;  oct++ )
		for( int key=0; key < 12; key++ )
		    acc += (s[(3-oct)+key*8] * keys[key+12*oct]);

	    for( int key=0; key < 12; key++ )
		acc += (s[key*8] * keys[key+12*4]);

	    retval[0] = acc;

	    acc=0.0f;
	    for( int oct=0; oct < 4;  oct++ ) {
		for( int key=0; key < 4; key++ )
		    acc += (s[(3-oct)+(key+7)*8] * keys[key+12*oct]);
		for( int key=4; key < 12; key++ )
		    acc += (s[(4-oct)+(key-4)*8] * keys[key+12*oct]);
	    }

	    retval[1] = acc;

	    acc=0.0f;
	    for( int oct=0; oct < 5;  oct++ )
		for( int key=0; key < 12; key++ )
		    acc += (s[(4-oct)+key*8] * keys[key+12*oct]);

	    retval[2] = acc;

	    acc=0.0f;
	    for( int oct=0; oct < 5;  oct++ ) {
		for( int key=0; key < 9; key++ )
		    acc += s[(4-oct)+(key+3)*8] * keys[key+12*oct];
		for( int key=9; key < 12; key++ )
		    acc += (s[(5-oct)+(key-9)*8] * keys[key+12*oct]);
	    }

	    retval[3] = acc;

	    acc=0.0f;
	    for( int oct=0; oct < 5;  oct++ )
		for( int key=0; key < 12; key++ )
		    acc += (s[(5-oct)+key*8] * keys[key+12*oct]);

	    retval[4] = acc;

	    acc=0.0f;
	    for( int oct=0; oct < 5;  oct++ )
		for( int key=0; key < 12; key++ )
		    acc += (s[(6-oct)+key*8] * keys[key+12*oct]);

	    retval[5] = acc;

	    acc=0.0f;
	    for( int oct=0; oct < 5;  oct++ )
		for( int key=0; key < 12; key++ )
		    acc += (s[(7-oct)+key*8] * keys[key+12*oct]);

	    retval[6] = acc;

	    return retval;
	}

	virtual void register_params( paramMap &map, std::string prefix ) {
	    map.add_this( "busbar", this );
	}
};

typedef Sequence< voice_vector, yc20busbar, VGain<7>, VectorSum<7> > yc20_t;
#endif


