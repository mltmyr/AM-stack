#include "CLI11.hpp"
#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "TriangleMesh.hpp"
#include "IO.hpp"
#include "ExPolygon.hpp"
#include "BoundingBox.hpp"
#include "Point.hpp"

int main(int argc, char **argv)
{
    /*=============[ Argument Parsing ]=============*/
    // Variables
    std::string outputfilename {"slice.json"};
    std::string inputfilename {"model.stl"};
    bool include_holes {false};
    bool only_points {false};
    double layer_height {0.4}; // mm
    // Setup CLI
    CLI::App app;
    app.add_option("-i", inputfilename, "Input file name.", true)->check(CLI::ExistingFile);
    app.add_option("-o", outputfilename, "Output file name.", true)->check(CLI::NonexistentPath);
    app.add_option("-z", layer_height, "Layer height in z direction (mm).", true);
    app.add_flag("--only-points", only_points, "Save points to csv");
    app.add_flag("--include-holes", include_holes, "Whether to include the points of the interior holes in the part.");
    CLI11_PARSE(app, argc, argv);
 
    /*=============[ STL parsing ]=============*/
    TriangleMesh tm;
    // Open STL
    try
    {
        Slic3r::IO::STL::read(inputfilename, &tm);
    } 
    catch (...) 
    {
        std::cout << "Error opening file: " << inputfilename << std::endl;
        return 1;
    }
    // Preliminary
    BoundingBoxf3 tm_bb = tm.bounding_box();
    double tm_height = (tm_bb.max.z - tm_bb.min.z);
    unsigned int num_layers = std::floor(tm_height / layer_height);
    
    // Set layer heights
    std::vector<double> layers_z_vals;
    layers_z_vals.push_back(0.0);
    double lh = 0.0;
    for (int i = 0; i < num_layers; i++)
    {
        lh += layer_height;
        layers_z_vals.push_back(lh);
    }
    
    // Slice the model
    std::cout << "Slicing the model." << std::endl;
    std::vector<ExPolygons> tm_layers = tm.slice(layers_z_vals);
    std::cout << "Slicing complete." << std::endl;
    
    // Prepare file
    std::ofstream ofs(outputfilename);
    json js;
    js["layers"] = json::array();
    // Process the layers
    ExPolygons* lay = nullptr;
    for (int64_t i = 0; i < layers_z_vals.size(); i++)
    {
        lay = &(tm_layers[i]);
        json jslayer;
        for (ExPolygons::const_iterator iter = lay->begin(); iter != lay->end(); iter++)
        {
            // The contour
            const Polygon* p = &(iter->contour);
            json jscontour = json::array();
            for (int j = 0; j < p->points.size(); j++)
            {
                Pointf ptf_xy = Pointf::new_unscale(p->points[j]);
                if (only_points)
                {
                    ofs << std::to_string(ptf_xy.x) << ", " 
                        << std::to_string(ptf_xy.y) << ", " 
                        << std::to_string(layers_z_vals[i]) << "\n"; 
                } else 
                {
                    jscontour.push_back({ptf_xy.x, ptf_xy.y, layers_z_vals[i]});
                }
            }
            js["layers"][i]["contour"] = jscontour;
            // The inner holes
            if  (include_holes)
            {
                json jsholes = json::array();
                const Polygons* holes = &(iter->holes);
                for (Polygons::const_iterator piter = holes->begin(); piter != holes->end(); piter++)
                {
                    json jshole = json::array();
                    for (int j = 0; j < piter->points.size(); j++)
                    {
                        Pointf ptf_xy = Pointf::new_unscale(piter->points[j]);
                        if (only_points)
                        {
                            ofs << std::to_string(ptf_xy.x) << ", "
                                << std::to_string(ptf_xy.y) << ", "
                                << std::to_string(layers_z_vals[i]) << "\n";
                        } else 
                        {
                            jshole.push_back({ptf_xy.x, ptf_xy.y, layers_z_vals[i]});
                        }
                    }
                    jsholes.push_back(jshole);
                }
                js["layers"][i]["holes"] = jsholes;
            }
        }
    }
    if (!only_points)
    {
        ofs << js;
    }
}