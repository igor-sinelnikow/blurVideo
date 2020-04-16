#include <iostream>
#include <optional>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <opencv2/opencv.hpp>

#define KEY_ESC 27

namespace pt = boost::property_tree;

cv::Size getKernelSize( const pt::ptree& settings )
{
    const int size = settings.get( "ksize", 23 );
    return cv::Size( size, size );
}

cv::VideoCapture getVideoSource( const pt::ptree& settings )
{
    const auto mbIndex = settings.get_optional<int>( "videoSource" );
    if ( mbIndex.has_value() )
    {
        return cv::VideoCapture( *mbIndex );
    }

    const auto mbFilename = settings.get_optional<std::string>( "videoSource" );
    if ( mbFilename.has_value() )
    {
        std::cout << *mbFilename << std::endl;
        return cv::VideoCapture( *mbFilename );
    }

    return cv::VideoCapture( 0 );
}

int main( int argc, char const *argv[] )
{
    pt::ptree settings;
    pt::read_ini( "settings.ini", settings );
    const int radius = settings.get( "areaRadius", 35 );
    const cv::Size ksize = getKernelSize( settings );
    cv::VideoCapture cap = getVideoSource( settings );

    if ( !cap.isOpened() )
    {
        std::cerr << "Cannot open the video or camera" << std::endl;
        return -1;
    }

    const double fps = cap.get( cv::CAP_PROP_FPS );
    const int delay = cvFloor( 1000 / fps ); // in milliseconds

    std::cout << "Press Esc to quit" << std::endl;

    const cv::String winname = "Blur round area";
    cv::namedWindow( winname );

    std::optional<cv::Point> coords = {};
    cv::setMouseCallback( winname, []( int event, int x, int y, int flags, void* coords )
    {
        if ( event == cv::EVENT_MOUSEMOVE )
        {
            *((std::optional<cv::Point>*)coords) = std::make_optional<cv::Point>( x, y );
        }
    }, &coords );

    cv::Mat frame;
    while ( cap.read( frame ) )
    {
        if ( coords.has_value() )
        {
            const auto& center = *coords;

            cv::Mat maskToBlur = cv::Mat::zeros( frame.size(), frame.type() );
            cv::Mat maskToAdd = cv::Mat::zeros( frame.size(), frame.type() );

            cv::circle( maskToBlur, center, 1.2*radius, CV_RGB( 255, 255, 255 ), -1, cv::LINE_AA );
            cv::circle( maskToAdd, center, radius, CV_RGB( 255, 255, 255 ), -1, cv::LINE_AA );

            cv::Mat roi;
            cv::GaussianBlur( frame & maskToBlur, roi, ksize, 0 );

            frame = ( frame & ( ~maskToAdd ) ) + ( roi & maskToAdd );
        }
        cv::imshow( winname, frame );

        if ( cv::waitKey( delay ) == KEY_ESC )
        {
            break;
        }
    }

    return 0;
}
