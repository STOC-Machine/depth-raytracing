// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"          // Include short list of convenience functions for rendering
#include "ray.h"
#include "voxel.cpp"
#include <algorithm>            // std::min, std::max

#define X_BOXES 100
#define Y_BOXES 100
#define Z_BOXES 100

// Helper functions
void register_glfw_callbacks(window& app, glfw_state& app_state);

int main(int argc, char * argv[]) try
{
    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "RealSense Pointcloud Example");
    // Construct an object to manage view state
    glfw_state app_state;
    // register callbacks to allow manipulation of the pointcloud
    register_glfw_callbacks(app, app_state);

    // Declare pointcloud object, for calculating pointclouds and texture mappings
    rs2::pointcloud pc;
    // We want the points object to be persistent so we can display the last cloud when a frame drops
    rs2::points points;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();
    
    
    int voxelGrid[X_BOXES][Y_BOXES][Z_BOXES];
    double point[3];
    //double cam[3];
    //vec3Set(0.0, 0.0, 0.0, cam);
    
    
    while (app) // Application still alive?
    {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        // Wait for the next set of frames from the camera
        auto frames = pipe.wait_for_frames();

        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Tell pointcloud object to map to this color frame
        pc.map_to(color);

        auto depth = frames.get_depth_frame();

        // Generate the pointcloud and texture mappings
        points = pc.calculate(depth);
        

        initializeVoxelGrid(voxelGrid, X_BOXES, Y_BOXES, Z_BOXES);


        // Camera defined to be at origin, pointed in some direction

        
        // The position of an arbitrary point in camera space, coordinates in meters

        
        auto vertices=points.get_vertices();
        for (int i=0; i<points.size();i+=10){
            if(vertices[i].z){
                point[0]=vertices[i].x;
                point[1]=vertices[i].y;
                point[2]=vertices[i].z;
                castPositiveDelta(point, voxelGrid);
                castNegativeDelta(point,voxelGrid);
            }
        }
        
        for (int i=0; i<X_BOXES; i++){
            for (int j=0; j<Y_BOXES;j++){
                for (int k=0; k<Z_BOXES;k++){
                    if(voxelGrid[i][j][k]<0){
                        //printf("%d,%d,%d\n",i,j,k);
                        float voxels[1][3]={{i,j,k}};
                        draw_voxels(app.width(), app.height(), app_state, 1, voxels, 0.1);
                    
                    }
                }
            }
        }
        
        //printf("%ld\n",points.size());

        // Upload the color frame to OpenGL
        app_state.tex.upload(color);

        // Draw the pointcloud
        //draw_pointcloud(app.width(), app.height(), app_state, points);
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
