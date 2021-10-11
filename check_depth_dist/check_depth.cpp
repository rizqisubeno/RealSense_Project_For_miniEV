#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <iostream>

using namespace std;

int main(int argc, char * argv[]) try
{
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline p;

    // configure and start the pipeline
    p.start();

    while(true)
    {
        //block program until frames arrive
        rs2::frameset frames = p.wait_for_frames();

        //try to get a frame of a depth image
        rs2::depth_frame depth = frames.get_depth_frame();

        //get depth frame's dimensions
        auto width = depth.get_width();
        auto height = depth.get_height();

        // Query the distance from the camera to the object in the center of the image
        float distance_to_center = depth.get_distance(width/2,height/2);
        
        //print distance
        cout << "camera facing an object " << distance_to_center << " meters away" << endl;
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    cerr << e.what() << endl;
    return EXIT_FAILURE;
}
