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

//$Id: image_symbolizer.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef IMAGE_SYMBOLIZER_HPP
#define IMAGE_SYMBOLIZER_HPP

#include "symbolizer.hpp"
#include "image_data.hpp"

namespace mapnik 
{   
    struct image_symbolizer : public symbolizer
    {
    private:
	ImageData32 symbol_;
    public:
	image_symbolizer(std::string const& file,
			 std::string const& type,
			 unsigned width,unsigned height) 
	    : symbolizer(),
	      symbol_(width,height)
	{
	    try 
	    {
		std::auto_ptr<ImageReader> reader(get_image_reader(type,file));
		std::cout<<"image width="<<reader->width()<<std::endl;
		std::cout<<"image height="<<reader->height()<<std::endl;
		reader->read(0,0,symbol_);		
	    } 
	    catch (...) 
	    {
		std::cerr<<"exception caught..."<<std::endl;
	    }
	}

	virtual ~image_symbolizer() {}

	void render(geometry_type& geom,Image32& image) const 
	{
	    int w=symbol_.width();
	    int h=symbol_.height();
		
	    geometry_type::path_iterator<SHIFT0> itr=geom.begin<SHIFT0>();
	    while (itr!=geom.end<SHIFT0>())
	    {	 
		int x=itr->x;
		int y=itr->y;
		int px=int(x-0.5*w);
		int py=int(y-0.5*h);
		image.set_rectangle(px,py,symbol_);
	 	++itr;
	    }
	}
	
    private:
	image_symbolizer(const image_symbolizer&);
	image_symbolizer& operator=(const image_symbolizer&);	
    };
}

#endif // IMAGE_SYMBOLIZER_HPP
