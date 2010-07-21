#define BOOST_TEST_MODULE combined_tests

// boost.test
#include <boost/test/included/unit_test.hpp>

// mapnik
#include <mapnik/map.hpp>
#include <mapnik/svg_renderer.hpp>
#include <mapnik/color_factory.hpp>

// std
#include <string>
#include <sstream>

/**
 * This test case tests all the generators inside svg_renderer,
 * verifying the correctness of the whole SVG document.
 *
 * The test sets the svg_renderer object with a simple Map that
 * has only its dimensions specified and calls the apply()
 * method to produce the output.
 *
 * The output stream is a stringstream (the output is generated
 * into a stringstream).
 */
BOOST_AUTO_TEST_CASE(combined_test_case)
{
    using namespace mapnik;
    typedef svg_renderer<std::ostringstream> svg_ren;

    Map map(800, 600);
    map.set_background(color_factory::from_string("white"));

    std::ostringstream output_stream;
    svg_ren renderer(map, output_stream);
    renderer.apply();

    std::string expected_output = 
	svg_ren::XML_DECLARATION 
	+ "\n" 
	+ svg_ren::SVG_DTD 
	+ "\n" 
	+ "<svg width=\"800px\" height=\"600px\" version=\"1.1\" xmlns=\"" 
	+ svg_ren::SVG_NAMESPACE_URL 
	+ "\">"
	+"\n"
	+"<rect x=\"0\" y=\"0\" width=\"800px\" height=\"600px\" style=\"fill: #ffffff\"/>"
	+"\n"
	+"</svg>";

    std::string actual_output = renderer.get_output_stream().str();
    BOOST_CHECK_EQUAL(actual_output, expected_output);
}

