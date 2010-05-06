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

#ifndef LV2URIMAP_HH
#define LV2URIMAP_HH

class LV2Feature
{
    protected:
        void *data;

        LV2Feature( const LV2_Feature * const * features, const std::string uri )
        {
            int i;
            for (i=0; features[i]; i++)
                if (features[i]->uri == uri)
                    break;

            assert( features[i] );

            data = features[i]->data;
        }
};


class LV2URIMap : public LV2Feature
{
    public:
        LV2URIMap( const LV2_Feature * const * features )
            : LV2Feature( feature, LV2_URI_MAP_URI )
        {}

        uint32_t uri_to_id const ( const char *map, const char *uri )
        {
            LV2_URI_Map_Feature *f = (LV2_URI_Map_Feature *) data;
            return f->uri_to_id( f->callback_data, map, uri );
        }

        uint32_t operator[] const (const char *uri)
        {
            return uri_to_id( 0, uri );
        }

        uint32_t operator[] const (const std::string &uri)
        {
            return uri_to_id( 0, uri.c_str() );
        }
};

#endif
