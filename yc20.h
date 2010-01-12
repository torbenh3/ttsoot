#ifndef YC20_HH
#define YC20_HH

typedef Chain< BiQuadLP, BiQuadHP, Gain> FBank;

typedef Cascade< FlipFlop, FBank, FBank, FBank, FBank, FBank, FBank, FBank, FBank, FBank > FlipFlopFilterCascade;

template<int c6, int c5, int c4, int c3, int c2, int c1, int c0>
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
			
	    float C0 = (float)c0 / 10000.0;
	    float C1 = (float)c1 / 10000.0;
	    float C2 = (float)c2 / 10000.0;
	    float C3 = (float)c3 / 10000.0;
	    float C4 = (float)c4 / 10000.0;
	    float C5 = (float)c5 / 10000.0;
	    float C6 = (float)c6 / 10000.0;

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
	    local_map["/1/1/1/1/1/1/1/1/0/0/bw"] = 3.0;
	    local_map["/1/1/1/1/1/1/1/1/0/0/cut"] = passive_rc_frequency( 15000, C6 );
	    local_map["/1/1/1/1/1/1/1/1/0/1/bw"] = 3.0;
	    local_map["/1/1/1/1/1/1/1/1/0/1/cut"] = 1.0;
	    local_map["/1/1/1/1/1/1/1/1/0/2/mod/gain"] = 0.0;
	}
	virtual void register_params( paramMap &map, std::string prefix ) { }

};

template<int c6, int c5, int c4, int c3, int c2, int c1, int c0>
class yc20voice : public Sequence< SlowVarBLSaw, YC20FilterBank<c6, c5, c4, c3, c2, c1, c0>, Gain > {};

typedef yc20voice<47, 100, 220, 470, 820, 1200, 1500> yc20voice_I1;
typedef yc20voice<39, 82,  180, 390, 680, 1000, 1500> yc20voice_I2;
typedef yc20voice<27, 56,  120, 270, 560,  820, 1200> yc20voice_I3;

typedef Mixer< yc20voice_I1, yc20voice_I1, yc20voice_I1, yc20voice_I1, yc20voice_I2, yc20voice_I2, yc20voice_I2, yc20voice_I2, yc20voice_I3, yc20voice_I3, yc20voice_I3, yc20voice_I3 > yc20_t; 

#endif
