/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2007 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/
//$Id$

#ifndef GDAL_DATASOURCE_HPP
#define GDAL_DATASOURCE_HPP

#include <mapnik/datasource.hpp>
#include <boost/shared_ptr.hpp>
#include <gdal_priv.h>

class gdal_datasource : public mapnik::datasource 
{
public:
    gdal_datasource(mapnik::parameters const& params);
    virtual ~gdal_datasource ();
    int type() const;
    static std::string name();
    mapnik::featureset_ptr features( mapnik::query const& q) const;
    mapnik::featureset_ptr features_at_point(mapnik::coord2d const& pt) const;
    mapnik::box2d<double> envelope() const;
    mapnik::layer_descriptor get_descriptor() const;
private:
    mapnik::box2d<double> extent_;
    std::string dataset_name_;
    int band_;
    mapnik::layer_descriptor desc_;
    unsigned width_;
    unsigned height_;
    bool shared_dataset_;
    double filter_factor_;
    inline GDALDataset *open_dataset() const;
};


#endif // GDAL_DATASOURCE_HPP
