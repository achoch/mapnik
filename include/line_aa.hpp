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

//$Id: line_aa.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef LINE_AA_HPP
#define LINE_AA_HPP

#include "geometry.hpp"
#include "graphics.hpp"
#include "style.hpp"

namespace mapnik
{
    template <typename PixBuffer> class LineRasterizerAA
    {
    private:
	PixBuffer* pixbuf_;
    public:
	LineRasterizerAA(PixBuffer& pixbuf)
                :pixbuf_(&pixbuf) {}
	
	template <typename Transform>
	void render(const geometry_type& geom,const Color& c);
    private:
	LineRasterizerAA(const LineRasterizerAA&);
	LineRasterizerAA& operator=(const LineRasterizerAA&);
	void render_line(int x0,int y0,int x1,int y1,unsigned rgba);
    };
}
#endif                                            //LINE_AA_HPP
