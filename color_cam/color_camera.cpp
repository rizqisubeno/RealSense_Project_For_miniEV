#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char * argv[]) try
{
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline p;

    // configure and start the pipeline
    p.start();

    const auto window_name="RGB Camera";
    namedWindow(window_name,WINDOW_AUTOSIZE);

    while(waitKey(1)<0 && getWindowProperty(window_name,WND_PROP_AUTOSIZE)>=0)
    {
        //block program until frames arrive
        rs2::frameset frames = p.wait_for_frames();

        //try to get a video
        rs2::video_frame videoColor = frames.get_color_frame();

        //query frame size 
        auto w = videoColor.as<rs2::video_frame>().get_width();
        auto h = videoColor.as<rs2::video_frame>().get_height();

        //create OpenCV matrix of size (w,h) from colorized video
        Mat img(Size(w,h),CV_8UC3,(void*)videoColor.get_data(), Mat::AUTO_STEP);
        Mat rgbImg(Size(w,h),CV_8UC3);
        cvtColor(img,rgbImg,COLOR_BGR2RGB);
        // update window with new data
        imshow(window_name,rgbImg);
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
