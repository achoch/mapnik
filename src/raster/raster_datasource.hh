/* This file is part of Mapnik (c++ mapping toolkit)
 * Copyright (C) 2005 Artem Pavlenko
 *
 * Mapnik is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

//$Id: raster_datasource.hh 68 2004-11-23 22:39:58Z artem $

#ifndef RASTER_DATASOURCE_HH
#define RASTER_DATASOURCE_HH

#include "mapnik.hh"
#include "image_reader.hh"

using namespace mapnik;

class raster_datasource : public datasource
{
private:
    std::string                  filename_;
    std::string                  format_;
    mapnik::Envelope<double>     extent_;
public:
    raster_datasource(const Parameters& params);
    virtual            ~raster_datasource();
    int                type() const;
    static std::string name();
    FeaturesetPtr      featuresAll(const CoordTransform& t) const;
    FeaturesetPtr      featuresInBox(const CoordTransform& t,const mapnik::Envelope<double>& box) const;
    FeaturesetPtr      featuresAtPoint(const CoordTransform& t,const coord2d& pt) const;
    const mapnik::Envelope<double>& envelope() const;
private:
    //no copying
    raster_datasource(const raster_datasource&);
    raster_datasource& operator=(const raster_datasource&);
    //
    bool parseEnvelope(const std::string& str,Envelope<double>& envelope);
};

#endif                                            //RASTER_DATASOURCE_H
