#!/usr/bin/env python

from nose.tools import *
from utilities import Todo

import mapnik2, pickle

# Tests that exercise the functionality of Mapnik classes.

# ShieldSymbolizer initialization
def test_shieldsymbolizer_init():
    s = mapnik2.ShieldSymbolizer(mapnik2.Expression('[Field Name]'), 'DejaVu Sans Bold', 6, mapnik2.Color('#000000'), mapnik2.PathExpression('../data/images/dummy.png'))

# ShieldSymbolizer missing image file
# images paths are now PathExpressions are evaluated at runtime
# so it does not make sense to throw...
#@raises(RuntimeError)
#def test_shieldsymbolizer_missing_image():
#    s = mapnik2.ShieldSymbolizer(mapnik2.Expression('[Field Name]'), 'DejaVu Sans Bold', 6, mapnik2.Color('#000000'), mapnik2.PathExpression('../#data/images/broken.png'))

# PointSymbolizer initialization
def test_pointsymbolizer_init():
    p = mapnik2.PointSymbolizer() 
    eq_(p.allow_overlap, False)
    eq_(p.opacity,1)
    eq_(p.filename,'')

    p = mapnik2.PointSymbolizer(mapnik2.PathExpression("../data/images/dummy.png"))
    eq_(p.allow_overlap, False)
    eq_(p.opacity, 1)
    eq_(p.filename,'../data/images/dummy.png')

# PointSymbolizer missing image file
# images paths are now PathExpressions are evaluated at runtime
# so it does not make sense to throw...
#@raises(RuntimeError)
#def test_pointsymbolizer_missing_image():
 #   p = mapnik2.PointSymbolizer(mapnik2.PathExpression("../data/images/broken.png"))

# PointSymbolizer pickling
def test_pointsymbolizer_pickle():
    raise Todo("point_symbolizer pickling currently disabled")
    p = mapnik2.PointSymbolizer(mapnik2.PathExpression("../data/images/dummy.png"))
    p2 = pickle.loads(pickle.dumps(p,pickle.HIGHEST_PROTOCOL))
    # image type, width, and height only used in contructor...
    eq_(p.filename, p2.filename)
    eq_(p.allow_overlap, p2.allow_overlap)
    eq_(p.opacity, p2.opacity)

# PolygonSymbolizer initialization
def test_polygonsymbolizer_init():
    p = mapnik2.PolygonSymbolizer()

    eq_(p.fill, mapnik2.Color('gray'))
    eq_(p.fill_opacity, 1)

    p = mapnik2.PolygonSymbolizer(mapnik2.Color('blue'))

    eq_(p.fill, mapnik2.Color('blue'))
    eq_(p.fill_opacity, 1)

# PolygonSymbolizer pickling
def test_polygonsymbolizer_pickle():
    p = mapnik2.PolygonSymbolizer(mapnik2.Color('black'))
    p.fill_opacity = .5
    # does not work for some reason...
    #eq_(pickle.loads(pickle.dumps(p)), p)
    p2 = pickle.loads(pickle.dumps(p,pickle.HIGHEST_PROTOCOL))
    eq_(p.fill, p2.fill)
    eq_(p.fill_opacity, p2.fill_opacity)


# Stroke initialization
def test_stroke_init():
    s = mapnik2.Stroke()

    eq_(s.width, 1)
    eq_(s.opacity, 1)
    eq_(s.color, mapnik2.Color('black'))
    eq_(s.line_cap, mapnik2.line_cap.BUTT_CAP)
    eq_(s.line_join, mapnik2.line_join.MITER_JOIN)

    s = mapnik2.Stroke(mapnik2.Color('blue'), 5.0)

    eq_(s.width, 5)
    eq_(s.opacity, 1)
    eq_(s.color, mapnik2.Color('blue'))
    eq_(s.line_cap, mapnik2.line_cap.BUTT_CAP)
    eq_(s.line_join, mapnik2.line_join.MITER_JOIN)

# Stroke dashes
def test_stroke_dash_arrays():
    s = mapnik2.Stroke()
    s.add_dash(1,2)
    s.add_dash(3,4)
    s.add_dash(5,6)

    eq_(s.get_dashes(), [(1,2),(3,4),(5,6)])

# Stroke pickling
def test_stroke_pickle():
    s = mapnik2.Stroke(mapnik2.Color('black'),4.5)

    eq_(s.width, 4.5)
    eq_(s.color, mapnik2.Color('black'))

    s.add_dash(1,2)
    s.add_dash(3,4)
    s.add_dash(5,6)

    s2 = pickle.loads(pickle.dumps(s,pickle.HIGHEST_PROTOCOL))
    eq_(s.color, s2.color)
    eq_(s.width, s2.width)
    eq_(s.opacity, s2.opacity)
    eq_(s.get_dashes(), s2.get_dashes())
    eq_(s.line_cap, s2.line_cap)
    eq_(s.line_join, s2.line_join)

    
# LineSymbolizer initialization
def test_linesymbolizer_init():
    l = mapnik2.LineSymbolizer()
   
    eq_(l.stroke.width, 1)
    eq_(l.stroke.opacity, 1)
    eq_(l.stroke.color, mapnik2.Color('black'))
    eq_(l.stroke.line_cap, mapnik2.line_cap.BUTT_CAP)
    eq_(l.stroke.line_join, mapnik2.line_join.MITER_JOIN)

    l = mapnik2.LineSymbolizer(mapnik2.Color('blue'), 5.0)

    eq_(l.stroke.width, 5)
    eq_(l.stroke.opacity, 1)
    eq_(l.stroke.color, mapnik2.Color('blue'))
    eq_(l.stroke.line_cap, mapnik2.line_cap.BUTT_CAP)
    eq_(l.stroke.line_join, mapnik2.line_join.MITER_JOIN)
    
    s = mapnik2.Stroke(mapnik2.Color('blue'), 5.0)
    l = mapnik2.LineSymbolizer(s)
    
    eq_(l.stroke.width, 5)
    eq_(l.stroke.opacity, 1)
    eq_(l.stroke.color, mapnik2.Color('blue'))
    eq_(l.stroke.line_cap, mapnik2.line_cap.BUTT_CAP)
    eq_(l.stroke.line_join, mapnik2.line_join.MITER_JOIN)

# LineSymbolizer pickling
def test_linesymbolizer_pickle():
    p = mapnik2.LineSymbolizer()
    p2 = pickle.loads(pickle.dumps(p,pickle.HIGHEST_PROTOCOL))
    # line and stroke eq fails, so we compare attributes for now..
    s,s2 = p.stroke, p2.stroke
    eq_(s.color, s2.color)
    eq_(s.opacity, s2.opacity)
    eq_(s.width, s2.width)
    eq_(s.get_dashes(), s2.get_dashes())
    eq_(s.line_cap, s2.line_cap)
    eq_(s.line_join, s2.line_join)

# Shapefile initialization
def test_shapefile_init():
    s = mapnik2.Shapefile(file='../../demo/data/boundaries')

    e = s.envelope()
   
    assert_almost_equal(e.minx, -11121.6896651, places=7)
    assert_almost_equal(e.miny, -724724.216526, places=6)
    assert_almost_equal(e.maxx, 2463000.67866, places=5)
    assert_almost_equal(e.maxy, 1649661.267, places=3)

# Shapefile properties
def test_shapefile_properties():
    s = mapnik2.Shapefile(file='../../demo/data/boundaries', encoding='latin1')
    f = s.features_at_point(s.envelope().center()).features[0]

    eq_(f['CGNS_FID'], u'6f733341ba2011d892e2080020a0f4c9')
    eq_(f['COUNTRY'], u'CAN')
    eq_(f['F_CODE'], u'FA001')
    eq_(f['NAME_EN'], u'Quebec')
    eq_(f['NOM_FR'], u'Qu\xe9bec')
    eq_(f['Shape_Area'], 1512185733150.0)
    eq_(f['Shape_Leng'], 19218883.724300001)

    # Check that the deprecated interface still works,
    # remove me once the deprecated code is cleaned up
    eq_(f.properties['Shape_Leng'], 19218883.724300001)

# TextSymbolizer initialization
def test_textsymbolizer_init():
    ts = mapnik2.TextSymbolizer(mapnik2.Expression('[Field_Name]'), 'Font Name', 8, mapnik2.Color('black'))

    eq_(str(ts.name), str(mapnik2.Expression('[Field_Name]')))
    eq_(ts.face_name, 'Font Name')
    eq_(ts.text_size, 8)
    eq_(ts.fill, mapnik2.Color('black'))

# TextSymbolizer pickling
def test_textsymbolizer_pickle():
    ts = mapnik2.TextSymbolizer(mapnik2.Expression('[Field_Name]'), 'Font Name', 8, mapnik2.Color('black'))

    eq_(str(ts.name), str(mapnik2.Expression('[Field_Name]')))
    eq_(ts.face_name, 'Font Name')
    eq_(ts.text_size, 8)
    eq_(ts.fill, mapnik2.Color('black'))
    
    raise Todo("text_symbolizer pickling currently disabled")

    ts2 = pickle.loads(pickle.dumps(ts,pickle.HIGHEST_PROTOCOL))
    eq_(ts.name, ts2.name)
    eq_(ts.face_name, ts2.face_name)
    eq_(ts.allow_overlap, ts2.allow_overlap)
    eq_(ts.get_displacement(), ts2.get_displacement())
    eq_(ts.get_anchor(), ts2.get_anchor())
    eq_(ts.fill, ts2.fill)
    eq_(ts.force_odd_labels, ts2.force_odd_labels)
    eq_(ts.halo_fill, ts2.halo_fill)
    eq_(ts.halo_radius, ts2.halo_radius)
    eq_(ts.label_placement, ts2.label_placement)
    eq_(ts.minimum_distance, ts2.minimum_distance)
    eq_(ts.text_ratio, ts2.text_ratio)
    eq_(ts.text_size, ts2.text_size)
    eq_(ts.wrap_width, ts2.wrap_width)
    eq_(ts.vertical_alignment, ts2.vertical_alignment)
    eq_(ts.label_spacing, ts2.label_spacing)
    eq_(ts.label_position_tolerance, ts2.label_position_tolerance)
    
    eq_(ts.wrap_character, ts2.wrap_character)
    eq_(ts.text_convert, ts2.text_convert)
    eq_(ts.line_spacing, ts2.line_spacing)
    eq_(ts.character_spacing, ts2.character_spacing)
    
    # r1341
    eq_(ts.wrap_before, ts2.wrap_before)
    eq_(ts.horizontal_alignment, ts2.horizontal_alignment)
    eq_(ts.justify_alignment, ts2.justify_alignment)
    eq_(ts.opacity, ts2.opacity)
        
    raise Todo("FontSet pickling support needed: http://trac.mapnik2.org/ticket/348")
    eq_(ts.fontset, ts2.fontset)


# Map initialization
def test_map_init():
    m = mapnik2.Map(256, 256)
   
    eq_(m.width, 256)
    eq_(m.height, 256)
    eq_(m.srs, '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs')

    m = mapnik2.Map(256, 256, '+proj=latlong')
    
    eq_(m.width, 256)
    eq_(m.height, 256)
    eq_(m.srs, '+proj=latlong')

# Map initialization from string
def test_map_init_from_string():
    map_string = '''<Map bgcolor="steelblue" srs="+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs">
     <Style name="My Style">
      <Rule>
       <PolygonSymbolizer>
        <CssParameter name="fill">#f2eff9</CssParameter>
       </PolygonSymbolizer>
       <LineSymbolizer>
        <CssParameter name="stroke">rgb(50%,50%,50%)</CssParameter>
        <CssParameter name="stroke-width">0.1</CssParameter>
       </LineSymbolizer>
      </Rule>
     </Style>
     <Layer name="boundaries">
      <StyleName>My Style</StyleName>
       <Datasource>
        <Parameter name="type">shape</Parameter>
        <Parameter name="file">../../demo/data/boundaries</Parameter>
       </Datasource>
      </Layer>
    </Map>'''

    m = mapnik2.Map(600, 300)
    
    mapnik2.load_map_from_string(m, map_string)
    mapnik2.load_map_from_string(m, map_string, False, "")
    mapnik2.load_map_from_string(m, map_string, True, "")
    raise(Todo("Need to write more map property tests in 'object_test.py'..."))

# Map pickling
def test_map_pickle():
    # Fails due to scale() not matching, possibly other things
    raise(Todo("Map does not support pickling yet (Tickets #345)."))

    m = mapnik2.Map(256, 256)

    eq_(pickle.loads(pickle.dumps(m)), m)

    m = mapnik2.Map(256, 256, '+proj=latlong')

    eq_(pickle.loads(pickle.dumps(m)), m)

# Color initialization
def test_color_init():
    c = mapnik2.Color('blue')

    eq_(c.a, 255)
    eq_(c.r, 0)
    eq_(c.g, 0)
    eq_(c.b, 255)

    eq_(c.to_hex_string(), '#0000ff')

    c = mapnik2.Color('#f2eff9')
    
    eq_(c.a, 255)
    eq_(c.r, 242)
    eq_(c.g, 239)
    eq_(c.b, 249)

    eq_(c.to_hex_string(), '#f2eff9')

    c = mapnik2.Color('rgb(50%,50%,50%)')

    eq_(c.a, 255)
    eq_(c.r, 128)
    eq_(c.g, 128)
    eq_(c.b, 128)

    eq_(c.to_hex_string(), '#808080')

    c = mapnik2.Color(0, 64, 128)

    eq_(c.a, 255)
    eq_(c.r, 0)
    eq_(c.g, 64)
    eq_(c.b, 128)

    eq_(c.to_hex_string(), '#004080')
    
    c = mapnik2.Color(0, 64, 128, 192)

    eq_(c.a, 192)
    eq_(c.r, 0)
    eq_(c.g, 64)
    eq_(c.b, 128)

    eq_(c.to_hex_string(), '#004080')

# Color equality
def test_color_equality():
    c1 = mapnik2.Color('blue')
    c2 = mapnik2.Color('blue')
    c3 = mapnik2.Color('black')

    c3.r = 0
    c3.g = 0
    c3.b = 255
    c3.a = 255

    eq_(c1, c2)
    eq_(c1, c3)

    c1 = mapnik2.Color(0, 64, 128)
    c2 = mapnik2.Color(0, 64, 128)
    c3 = mapnik2.Color(0, 0, 0)

    c3.r = 0
    c3.g = 64
    c3.b = 128

    eq_(c1, c2)
    eq_(c1, c3)

    c1 = mapnik2.Color(0, 64, 128, 192)
    c2 = mapnik2.Color(0, 64, 128, 192)
    c3 = mapnik2.Color(0, 0, 0, 255)

    c3.r = 0
    c3.g = 64
    c3.b = 128
    c3.a = 192

    eq_(c1, c2)
    eq_(c1, c3)
    
    c1 = mapnik2.Color('rgb(50%,50%,50%)')
    c2 = mapnik2.Color(128, 128, 128, 255)
    c3 = mapnik2.Color('#808080')
    c4 = mapnik2.Color('gray')

    eq_(c1, c2)
    eq_(c1, c3)
    eq_(c1, c4)

# Color pickling
def test_color_pickle():
    c = mapnik2.Color('blue')

    eq_(pickle.loads(pickle.dumps(c)), c)

    c = mapnik2.Color(0, 64, 128)

    eq_(pickle.loads(pickle.dumps(c)), c)

    c = mapnik2.Color(0, 64, 128, 192)

    eq_(pickle.loads(pickle.dumps(c)), c)

# Rule initialization
def test_rule_init():
    min_scale = 5
    max_scale = 10
    
    r = mapnik2.Rule()
   
    eq_(r.name, '')
    eq_(r.title, '')
    eq_(r.min_scale, 0)
    eq_(r.max_scale, float('inf'))
    
    r = mapnik2.Rule("Name")
    
    eq_(r.name, 'Name')
    eq_(r.title, '')
    eq_(r.min_scale, 0)
    eq_(r.max_scale, float('inf'))
    
    r = mapnik2.Rule("Name", "Title")
    
    eq_(r.name, 'Name')
    eq_(r.title, 'Title')
    eq_(r.min_scale, 0)
    eq_(r.max_scale, float('inf'))
    
    r = mapnik2.Rule("Name", "Title", min_scale)
    
    eq_(r.name, 'Name')
    eq_(r.title, 'Title')
    eq_(r.min_scale, min_scale)
    eq_(r.max_scale, float('inf'))
    
    r = mapnik2.Rule("Name", "Title", min_scale, max_scale)
    
    eq_(r.name, 'Name')
    eq_(r.title, 'Title')
    eq_(r.min_scale, min_scale)
    eq_(r.max_scale, max_scale)
    
# Coordinate initialization
def test_coord_init():
    c = mapnik2.Coord(100, 100)

    eq_(c.x, 100)
    eq_(c.y, 100)

# Coordinate multiplication
def test_coord_multiplication():
    c = mapnik2.Coord(100, 100)
    c *= 2

    eq_(c.x, 200)
    eq_(c.y, 200)

# Box2d initialization
def test_envelope_init():
    e = mapnik2.Box2d(100, 100, 200, 200)

    assert_true(e.contains(100, 100))
    assert_true(e.contains(100, 200))
    assert_true(e.contains(200, 200))
    assert_true(e.contains(200, 100))

    assert_true(e.contains(e.center()))
    
    assert_false(e.contains(99.9, 99.9))
    assert_false(e.contains(99.9, 200.1))
    assert_false(e.contains(200.1, 200.1))
    assert_false(e.contains(200.1, 99.9))

    eq_(e.width(), 100)
    eq_(e.height(), 100)

    eq_(e.minx, 100)
    eq_(e.miny, 100)

    eq_(e.maxx, 200)
    eq_(e.maxy, 200)

    c = e.center()

    eq_(c.x, 150)
    eq_(c.y, 150)

# Box2d pickling
def test_envelope_pickle():
    e = mapnik2.Box2d(100, 100, 200, 200)

    eq_(pickle.loads(pickle.dumps(e)), e)

# Box2d multiplication
def test_envelope_multiplication():
    e = mapnik2.Box2d(100, 100, 200, 200)
    e *= 2
    
    assert_true(e.contains(50, 50))
    assert_true(e.contains(50, 250))
    assert_true(e.contains(250, 250))
    assert_true(e.contains(250, 50))

    assert_false(e.contains(49.9, 49.9))
    assert_false(e.contains(49.9, 250.1))
    assert_false(e.contains(250.1, 250.1))
    assert_false(e.contains(250.1, 49.9))

    assert_true(e.contains(e.center()))
    
    eq_(e.width(), 200)
    eq_(e.height(), 200)

    eq_(e.minx, 50)
    eq_(e.miny, 50)

    eq_(e.maxx, 250)
    eq_(e.maxy, 250)

    c = e.center()

    eq_(c.x, 150)
    eq_(c.y, 150)
