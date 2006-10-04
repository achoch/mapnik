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

#include "shapefile.hpp"

shape_file::shape_file() {}

shape_file::shape_file(const std::string& file_name)
{
    //file_.rdbuf()->pubsetbuf(buff_,buffer_size);
    file_.open(file_name.c_str(),std::ios::in|std::ios::binary);
}

shape_file::~shape_file()
{
    if (file_ && file_.is_open())
        file_.close();
}


bool shape_file::open(const std::string& file_name)
{
    //file_.rdbuf()->pubsetbuf(buff_,buffer_size);
    file_.open(file_name.c_str(),std::ios::in | std::ios::binary);
    return file_?true:false;
}


bool shape_file::is_open()
{
    return file_.is_open();
}


void shape_file::close()
{
    if (file_ && file_.is_open())
        file_.close();
}



