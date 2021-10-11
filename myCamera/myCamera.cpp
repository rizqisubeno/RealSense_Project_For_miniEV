#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace cv;
using namespace cv::ximgproc;

int main(int argc, char * argv[]) try
{
    static bool first = true;
    //create configuration
    rs2::config cfg;
    rs2_format myFormat=RS2_FORMAT_BGR8;
    cout << "use color format -> " << rs2_format_to_string(myFormat) <<endl;
    cfg.enable_stream(RS2_STREAM_COLOR,0,640,480,myFormat,30);
    
    // Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline pipe;

    // configure and start the pipeline
    rs2::pipeline_profile selection = pipe.start(cfg);

    rs2::device selected_dev = selection.get_device();

    const auto window_name="myCamera";
    namedWindow(window_name,WINDOW_AUTOSIZE);

    //image process line detector pointer
    Ptr<FastLineDetector> fld = createFastLineDetector(3,1.414213538f,5,200,3,false);

    while(waitKey(1)<0 && getWindowProperty(window_name,WND_PROP_AUTOSIZE)>=0)
    {

        //block program until frames arrive
        rs2::frameset frames = pipe.wait_for_frames();

        //try to get a video
        rs2::video_frame videoFrame= frames.get_color_frame();

        //query frame size 
        auto w = videoFrame.as<rs2::video_frame>().get_width();
        auto h = videoFrame.as<rs2::video_frame>().get_height();
        
        if(first)
        {
            cout << "image size w="<<to_string(w)<<" x h="<<to_string(h)<<endl;
        }
        //create OpenCV matrix of size (w,h) from colorized video
        Mat img(Size(w,h),CV_8UC3,(void*)videoFrame.get_data(), Mat::AUTO_STEP);
        Mat imgGray(Size(w,h),CV_8UC1);
        Mat imgThreshold=img.clone();
        cvtColor(img,imgGray,COLOR_BGR2GRAY);
        threshold(imgGray,imgThreshold,127,255,THRESH_BINARY);
        //adaptiveThreshold(imgGray,imgThreshold,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,15,6);
        // /*Fast Line Detector*/ 
         vector<Vec4f> lines;

        // Because of some CPU's power strategy, it seems that the first running of
        // an algorithm takes much longer. So here we run the algorithm 10 times
        // to see the algorithm's processing time with sufficiently warmed-up
        // CPU performance.
        if(first==true)
        {
            for (int run_count = 0; run_count < 5; run_count++) {
            double freq = getTickFrequency();
            lines.clear();
            int64 start = getTickCount();
            // Detect the lines with FLD
            fld->detect(imgThreshold, lines);
            double duration_ms = double(getTickCount() - start) * 1000 / freq;
            cout << "Elapsed time for FLD " << duration_ms << " ms" << endl;
            }
            first=false;
        }
        else
        {
            lines.clear();
            fld->detect(imgThreshold, lines);
        }
        //show found lines with FLD
        //Mat line_image_fld(maskImg);
        Mat line_image_fld=imgGray.clone();
        fld->drawSegments(line_image_fld,lines);
        //update window with new data
        imshow(window_name,line_image_fld);
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
