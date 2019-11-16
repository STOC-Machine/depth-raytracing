#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#include "example.hpp"          // Include short list of convenience functions for rendering

#include <algorithm>            // std::min, std::max

// Helper functions
void register_glfw_callbacks(window& app, glfw_state& app_state);

void draw_voxels(float width, float height, glfw_state& app_state, int num_voxels, float vertices[][3], float cubesize)
{
    // OpenGL commands that prep screen for the pointcloud
    glLoadIdentity();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glClearColor(153.f / 255, 153.f / 255, 153.f / 255, 1);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    gluPerspective(60, width / height, 0.01f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    gluLookAt(0, 0, 0, 0, 0, 1, 0, -1, 0);

    glTranslatef(0, 0, +0.5f + app_state.offset_y*0.05f);
    glRotated(app_state.pitch, 1, 0, 0);
    glRotated(app_state.yaw, 0, 1, 0);
    glTranslatef(0, 0, -0.5f);
    //gluLookAt(-1,-1,-1,0,0,0,1,0,0);
    glPointSize(width/ 640);
    glEnable(GL_DEPTH_TEST);

    glColor3f(1.0,0,0);
    /* this segment actually prints the pointcloud */
    
    for (int i = 0; i < num_voxels; i++)
    { 
           float toDraw[16][3] ={{cubesize*vertices[i][0],cubesize*vertices[i][1],cubesize*vertices[i][2]},
                 {cubesize*(vertices[i][0]+1),cubesize*vertices[i][1],cubesize*vertices[i][2]},
                 {cubesize*(vertices[i][0]+1),cubesize*(vertices[i][1]+1),cubesize*vertices[i][2]},
                 {cubesize*vertices[i][0],cubesize*(vertices[i][1]+1),cubesize*vertices[i][2]},
                 {cubesize*vertices[i][0],cubesize*(vertices[i][1]+1),cubesize*(vertices[i][2]+1)},
                 {cubesize*vertices[i][0],cubesize*vertices[i][1],cubesize*(vertices[i][2]+1)},
                 {cubesize*(vertices[i][0]+1),cubesize*vertices[i][1],cubesize*(vertices[i][2]+1)},
                 {cubesize*(vertices[i][0]+1),cubesize*vertices[i][1],cubesize*vertices[i][2]},
                 
                 {cubesize*(vertices[i][0]+1),cubesize*(vertices[i][1]+1),cubesize*(vertices[i][2]+1)},
                 {cubesize*(vertices[i][0]+1),cubesize*vertices[i][1],cubesize*vertices[i][2]},
                 {cubesize*(vertices[i][0]+1),cubesize*(vertices[i][1]+1),cubesize*vertices[i][2]},
                 {cubesize*vertices[i][0],cubesize*(vertices[i][1]+1),cubesize*vertices[i][2]},
                 {cubesize*vertices[i][0],cubesize*(vertices[i][1]+1),cubesize*(vertices[i][2]+1)},
                 {cubesize*vertices[i][0],cubesize*vertices[i][1],cubesize*(vertices[i][2]+1)},
                 {cubesize*(vertices[i][0]+1),cubesize*vertices[i][1],cubesize*(vertices[i][2]+1)},
                 {cubesize*(vertices[i][0]+1),cubesize*vertices[i][1],cubesize*vertices[i][2]}};
            glBegin(GL_TRIANGLE_FAN);
            for (int j=0; j < 8; j++){

            // upload the point and texture coordinates only for points we have depth data for
                glVertex3fv(toDraw[j]);
            }
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
            for (int j=8; j < 16; j++){

            // upload the point and texture coordinates only for points we have depth data for
                glVertex3fv(toDraw[j]);
            }
            glEnd();
            
    }

    // OpenGL cleanup

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

/*
int main(int argc, char * argv[]) try
{
    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "RealSense Pointcloud Example");
    // Construct an object to manage view state
    glfw_state app_state;
    // register callbacks to allow manipulation of the pointcloud
    register_glfw_callbacks(app, app_state);

    float vertices[4][3]={{0,0,0},{1,0,0},{0,1,0},{0,0,1}};

    while (app) // Application still alive?
    {
        draw_voxels(app.width(), app.height(), app_state,4,vertices,0.1);
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
*/

