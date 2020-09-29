#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "TriangleMesh.hpp"
#include "IO.hpp"
#include "ExPolygon.hpp"
#include "BoundingBox.hpp"
#include "Point.hpp"

#include "boost/filesystem.hpp"
#include "boost/format.hpp"

int main()
{
	std::string path = boost::filesystem::current_path().string();
    std::string filename = "/../../test_object_2.stl";
	filename = path + filename;

	TriangleMesh tm;
    try
    {
		std::cout << filename << "\n";
        Slic3r::IO::STL::read(filename, &tm);
    }
    catch (...)
    {
        std::cout << "Could not read file\n";
        return 1;
    }

    BoundingBoxf3 tm_bb = tm.bounding_box();
    std::cout << "tm_bb3 start = (" << tm_bb.min.x << ", " << tm_bb.min.y << ", " << tm_bb.min.z << ")\n";
    std::cout << "tm_bb3 end   = (" << tm_bb.max.x << ", " << tm_bb.max.y << ", " << tm_bb.max.z << ")\n\n";

    coordf_t tm_min_z = tm_bb.min.z;
    coordf_t tm_max_z = tm_bb.max.z;

    double layer_thickness   = 0.4; // In mm
	double tm_height         = (tm_max_z - tm_min_z);
    unsigned int num_layers = std::floor(tm_height / layer_thickness);

	std::vector<double> layers_z_vals;
	layers_z_vals.push_back(0.0); // Seems that heights have to start at zero to avoid runtime error.
                                  // However, it does not seem to be consistant.

    //double lh = 0.5*layer_thickness + float(tm_min_z);
    //double lh = 0.5*layer_thickness;
    double lh = layer_thickness;
    layers_z_vals.push_back(lh);
    for (unsigned int i = 0; i < num_layers-1; i++)
    {
        lh += layer_thickness;
        layers_z_vals.push_back(lh);
    }

    for (unsigned int i = 0; i < layers_z_vals.size(); i++)
    {
        std::cout << "lh = " << layers_z_vals[i] << "\n";
    }

	std::cout << "There are " + std::to_string(layers_z_vals.size()) + " layers of " + std::to_string(layer_thickness) + "mm between "
		+ std::to_string(tm_min_z) + " and " + std::to_string(tm_max_z) + "\n";
	
	std::cout << "Slicing the model...";

    std::vector<ExPolygons> tm_layers = tm.slice(layers_z_vals);

	std::cout << "Slicing successful!" << std::endl;

    std::string outFilename = path + "/../../contour_points_2.csv";
    std::ofstream ofs;
	try
	{
		std::cout << outFilename << "\n";
		ofs.open(outFilename);
	}
	catch(...)
    {
        std::cout << "Failed to open output file" << std::endl;
        return 1;
    }

    ExPolygons* lay = nullptr;
    for (int64_t i = 0; i < layers_z_vals.size(); i++)
    {
        lay = &(tm_layers[i]);
        //coordf_t ptf_z = unscale(layers_z_vals[i]);
		coordf_t ptf_z = layers_z_vals[i];
		std::cout << "Printing layer " + std::to_string(i) + " to file. (" + std::to_string(ptf_z) + "mm)\n";

        for (ExPolygons::const_iterator iter = lay->begin(); iter != lay->end(); iter++)
        {
            const Polygon* ep = &(iter->contour);
            for (int j = 0; j < ep->points.size(); j++)
            {
                Pointf ptf_xy = Pointf::new_unscale(ep->points[j]);
                
                ofs << boost::format("%0.6f") % ptf_xy.x << ","
                    << boost::format("%0.6f") % ptf_xy.y << ","
                    << boost::format("%0.6f") % ptf_z    << "\n";
            }
            ofs << "\n"; // Each Polygon at a given layer is separated by one extra newline.
        }
        ofs << "\n"; // Each Layer is separated from the previous with one extra newline.
                     //     Resulting in two blank lines between layers and at the end of file.
    }

    ofs.close();
    
    return 0;
}
