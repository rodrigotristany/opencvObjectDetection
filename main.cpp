#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );
void applyBorderDetection( Mat fram );
void denoinsingImage(Mat frame);
void thresh_callback(int, void* );

/** Global variables */
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
Mat src_gray;
int thresh = 100;
RNG rng(12345);

/** @function main */
int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade|data/haarcascades/haarcascade_frontalface_alt.xml|Path to face cascade.}"
                             "{eyes_cascade|data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|Path to eyes cascade.}"
                             "{camera|0|Camera device number.}");

    parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face + eyes) in a video stream.\n"
                  "You can use Haar or LBP features.\n\n" );
    parser.printMessage();

    String face_cascade_name = samples::findFile( parser.get<String>("face_cascade") );
    String eyes_cascade_name = samples::findFile( parser.get<String>("eyes_cascade") );

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if( !eyes_cascade.load( eyes_cascade_name ) )
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };

    int camera_device = parser.get<int>("camera");
    VideoCapture capture;
    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }

    Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }

        applyBorderDetection( frame );

        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}

/***
 * Parte X: quitamos el ruido de la imagen.
***/

void denoinsingImage(Mat frame)
{
    Mat channels[3], denoised;
    split(frame, channels);
    merge(channels, 3, frame);

    imshow("Normal", frame);
    
    fastNlMeansDenoisingColored(frame,denoised,10,10,7,21);

    split(denoised, channels);
    merge(channels, 3, frame);

    imshow("Denoised", frame);
}

/***
 * FIN PARTE X
***/

/***
 * Parte Y: aplicamos en el stream de cámara el algoritmo de canny para detectar los bordes.
 ***/

void applyBorderDetection(Mat src)
{
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    blur( src_gray, src, Size(3,3) );
   
    const char* source_window = "Source";
    namedWindow( source_window );
    imshow( source_window, src );

    const int max_thresh = 255;
    createTrackbar( "Canny thresh:", source_window, &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );
}

void thresh_callback(int, void* )
{
    Mat canny_output;
    Canny( src_gray, canny_output, thresh, thresh*2 );

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        //Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        drawContours( drawing, contours, (int)i, Scalar(255,255,255), 2, LINE_8, hierarchy, 0 );
    }
    imshow( "Contours", drawing );
}

/***
 * FIN PARTE Y
***/