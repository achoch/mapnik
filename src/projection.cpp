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

// mapnik
#include <mapnik/projection.hpp>
#include <mapnik/utils.hpp>
// proj4
#include <proj_api.h>

namespace mapnik {

#ifdef MAPNIK_THREADSAFE
boost::mutex projection::mutex_;
#endif
   
projection::projection(std::string const& params)
    : params_(params)
{ 
    init(); //
}
    
projection::projection(projection const& rhs)
    : params_(rhs.params_) 
{
    init(); //
}
        
projection& projection::operator=(projection const& rhs) 
{ 
    projection tmp(rhs);
    swap(tmp);
    return *this;
}
    
bool projection::operator==(const projection& other) const 
{
    return (params_ == other.params_);
}
    
bool projection::operator!=(const projection& other) const 
{
    return !(*this == other);
}
    
bool projection::is_initialized() const
{
    return proj_ ? true : false;
}
    
bool projection::is_geographic() const
{
    return is_geographic_;
}
    
std::string const& projection::params() const
{
    return params_;
}
    
void projection::forward(double & x, double &y ) const
{
#ifdef MAPNIK_THREADSAFE
    mutex::scoped_lock lock(mutex_);
#endif
    projUV p;
    p.u = x * DEG_TO_RAD;
    p.v = y * DEG_TO_RAD;
    p = pj_fwd(p,proj_);
    x = p.u;
    y = p.v;
    if (is_geographic_)
    {
        x *=RAD_TO_DEG;
        y *=RAD_TO_DEG;
    }           
}
    
void projection::inverse(double & x,double & y) const
{
#ifdef MAPNIK_THREADSAFE
    mutex::scoped_lock lock(mutex_);
#endif
    if (is_geographic_)
    {
        x *=DEG_TO_RAD;
        y *=DEG_TO_RAD;
    }  
    projUV p;
    p.u = x;
    p.v = y;
    p = pj_inv(p,proj_);
    x = RAD_TO_DEG * p.u;
    y = RAD_TO_DEG * p.v;
}
    
projection::~projection() 
{
#ifdef MAPNIK_THREADSAFE
    mutex::scoped_lock lock(mutex_);
#endif
    if (proj_) pj_free(proj_);
}
    
void projection::init()
{
// Based on http://trac.osgeo.org/proj/wiki/ThreadSafety
// you could think pj_init was threadsafe in version 4.7.0
// but its certainly not, and must only be in >= 4.7 with
// usage of projCtx per thread
//#if PJ_VERSION < 470 && MAPNIK_THREADSAFE
//    mutex::scoped_lock lock(mutex_);
//#endif
    mutex::scoped_lock lock(mutex_);
    proj_=pj_init_plus(params_.c_str());
    if (!proj_) throw proj_init_error(params_);
    is_geographic_ = pj_is_latlong(proj_) ? true : false;
}
    
void projection::swap (projection& rhs)
{
    std::swap(params_,rhs.params_);
    init ();
}
}
