/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006 Artem Pavlenko
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

#ifndef QUERY_HPP
#define QUERY_HPP

//mapnik
#include <mapnik/box2d.hpp>
#include <mapnik/feature.hpp>

// boost
#include <boost/tuple/tuple.hpp>

// stl
#include <set>
#include <limits>

namespace mapnik {

class query 
{
public:
    typedef boost::tuple<double,double> resolution_type;
private:
    box2d<double> bbox_;
    resolution_type resolution_;
    double scale_denominator_;
    double filter_factor_;
    std::set<std::string> names_;
public:
         
    query(box2d<double> const& bbox, resolution_type const& resolution, double scale_denominator = 1.0)
        : bbox_(bbox),
          resolution_(resolution),
          scale_denominator_(scale_denominator),
          filter_factor_(1.0)
    {}
    
    query(query const& other)
        : bbox_(other.bbox_),
          resolution_(other.resolution_),
          scale_denominator_(other.scale_denominator_),
          filter_factor_(other.filter_factor_),
          names_(other.names_)
    {}
         
    query& operator=(query const& other)
    {
        if (this == &other) return *this;
        bbox_=other.bbox_;
        resolution_=other.resolution_;
        scale_denominator_=other.scale_denominator_;
        filter_factor_=other.filter_factor_;
        names_=other.names_;
        return *this;
    }
         
    query::resolution_type const& resolution() const
    {
        return resolution_;
    }
    
    double scale_denominator() const
    {
        return scale_denominator_;
    }
         
    box2d<double> const& get_bbox() const
    {
        return bbox_;
    }

    double get_filter_factor() const
    {
        return filter_factor_;
    }
    
    void filter_factor(double factor)
    {
        filter_factor_ = factor;
    }
             
    void add_property_name(std::string const& name)
    {
        names_.insert(name);
    } 
         
    std::set<std::string> const& property_names() const
    {
        return names_;
    }
};
}


#endif //QUERY_HPP
