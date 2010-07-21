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
#include <mapnik/agg_renderer.hpp>
#include <mapnik/agg_rasterizer.hpp>

// boost
#include <boost/tuple/tuple.hpp>
#include <boost/scoped_ptr.hpp>

// agg
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgba.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_conv_stroke.h"

namespace mapnik 
{

typedef boost::tuple<double,double,double,double> segment_t;
bool y_order(segment_t const& first,segment_t const& second)
{
    double miny0 = std::min(first.get<1>(),first.get<3>());
    double miny1 = std::min(second.get<1>(),second.get<3>());
    return  miny0 > miny1;
}

template <typename T>
void agg_renderer<T>::process(building_symbolizer const& sym,
                              Feature const& feature,
                              proj_transform const& prj_trans)
{
    typedef  coord_transform2<CoordTransform,geometry2d> path_type;
    typedef  coord_transform3<CoordTransform,geometry2d> path_type_roof;
    typedef agg::renderer_base<agg::pixfmt_rgba32_plain> ren_base;
    typedef agg::renderer_scanline_aa_solid<ren_base> renderer;

    agg::rendering_buffer buf(pixmap_.raw_data(),width_,height_, width_ * 4);
    agg::pixfmt_rgba32_plain pixf(buf);
    ren_base renb(pixf);

    color const& fill_  = sym.get_fill();
    unsigned r=fill_.red();
    unsigned g=fill_.green();
    unsigned b=fill_.blue();
    unsigned a=fill_.alpha();
    renderer ren(renb);
    agg::scanline_u8 sl;

    ras_ptr->reset();
    ras_ptr->gamma(agg::gamma_linear());
    
    double height = sym.height() * scale_factor_;
    
    for (unsigned i=0;i<feature.num_geometries();++i)
    {
        geometry2d const& geom = feature.get_geometry(i);
        if (geom.num_points() > 2)
        {
            boost::scoped_ptr<geometry2d> frame(new line_string_impl);
            boost::scoped_ptr<geometry2d> roof(new polygon_impl);
            std::deque<segment_t> face_segments;
            double x0(0);
            double y0(0);
            unsigned cm = geom.vertex(&x0,&y0);
            for (unsigned j=1;j<geom.num_points();++j)
            {
                double x(0);
                double y(0);
                cm = geom.vertex(&x,&y);
                if (cm == SEG_MOVETO)
                {
                    frame->move_to(x,y);
                }
                else if (cm == SEG_LINETO)
                {
                    frame->line_to(x,y);
                    face_segments.push_back(segment_t(x0,y0,x,y));
                }
                
                x0 = x;
                y0 = y;
            }
            std::sort(face_segments.begin(),face_segments.end(), y_order);
            std::deque<segment_t>::const_iterator itr=face_segments.begin();
            for (;itr!=face_segments.end();++itr)
            {
                boost::scoped_ptr<geometry2d> faces(new polygon_impl);
                faces->move_to(itr->get<0>(),itr->get<1>());
                faces->line_to(itr->get<2>(),itr->get<3>());
                faces->line_to(itr->get<2>(),itr->get<3>() + height);
                faces->line_to(itr->get<0>(),itr->get<1>() + height);

                path_type faces_path (t_,*faces,prj_trans);
                ras_ptr->add_path(faces_path);
                ren.color(agg::rgba8(int(r*0.8), int(g*0.8), int(b*0.8), int(a * sym.get_opacity())));
                agg::render_scanlines(*ras_ptr, sl, ren);
                ras_ptr->reset();

                frame->move_to(itr->get<0>(),itr->get<1>());
                frame->line_to(itr->get<0>(),itr->get<1>()+height);
            }

            geom.rewind(0);
            for (unsigned j=0;j<geom.num_points();++j)
            {
                double x,y;
                unsigned cm = geom.vertex(&x,&y);
                if (cm == SEG_MOVETO)
                {
                    frame->move_to(x,y+height);
                    roof->move_to(x,y+height);
                }
                else if (cm == SEG_LINETO)
                {
                    frame->line_to(x,y+height);
                    roof->line_to(x,y+height);
                }
            }
            path_type path(t_,*frame,prj_trans);
            agg::conv_stroke<path_type> stroke(path);
            ras_ptr->add_path(stroke);
            ren.color(agg::rgba8(int(r*0.8), int(g*0.8), int(b*0.8), int(255 * sym.get_opacity())));
            agg::render_scanlines(*ras_ptr, sl, ren);
            ras_ptr->reset();

            path_type roof_path (t_,*roof,prj_trans);
            ras_ptr->add_path(roof_path);
            ren.color(agg::rgba8(r, g, b, int(a * sym.get_opacity())));
            agg::render_scanlines(*ras_ptr, sl, ren);
        }
    }
}

template void agg_renderer<image_32>::process(building_symbolizer const&,
                                              Feature const&,
                                              proj_transform const&);

}
