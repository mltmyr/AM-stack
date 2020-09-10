#include <string>
#include <iostream>

#include "TriangleMesh.hpp"
#include "IO.hpp"
#include "ExPolygon.hpp"
#include "BoundingBox.hpp"

int main()
{
    TriangleMesh tm;

    std::string filename = "test_file.stl";

    try
    {
        Slic3r::IO::STL::read(filename, &tm);
    }
    catch (...)
    {
        std::cout << "Could not read file\n";
        return 1;
    }

    Slic3r::TriangleMeshSlicer<Z> tm_slicer(&tm);

    std::vector<float> layers_z_vals{ 0.2, 0.4, 0.6, 0.8, 1.0 };
    std::vector<ExPolygons>* tm_layers;
    tm_slicer.slice(layers_z_vals, tm_layers);

    unsigned int inspect_layer_num = 2;

	ExPolygons* choosen_layer = &(tm_layers->at(inspect_layer_num - 1));
	for (ExPolygons::const_iterator iter = choosen_layer->begin(); iter != choosen_layer->end(); iter++)
	{
		BoundingBox bb_choosen_layer = iter->bounding_box();
		std::cout << "BoundingBox start = (" << bb_choosen_layer.min.x << ", " << bb_choosen_layer.min.y << ")\n";
		std::cout << "BoundingBox end   = (" << bb_choosen_layer.max.x << ", " << bb_choosen_layer.max.y << ")\n\n";
	}
    

    return 0;
}
