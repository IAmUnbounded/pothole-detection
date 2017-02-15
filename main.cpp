#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
using namespace std;

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold=100;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";
int erosion_elem = 0;
int erosion_size = 1;
int dilation_elem = 0;
int dilation_size = 12;
int const max_elem = 2;
int const max_kernel_size = 21;
Mat erosion_dst,dilation_dst;

void Erosion( int, void* )
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( dilation_dst, erosion_dst, element );
  imshow( "Erosion Demo", erosion_dst );
}

void Dilation( int, void* )
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation


  Mat dst1;
  dilate( detected_edges, dilation_dst, element );
  imshow( "Dilation Demo", dilation_dst );
Erosion(0,0);
  vector<Point> Largest_contour;
          double max_area=0;
  vector<vector<Point> > contours;
  findContours(erosion_dst,contours,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
  for(unsigned int i=0; i < contours.size(); i++)
   {
       double area=contourArea(contours[i]);
       if(area>300&&area<1000)
       {
           vector<vector<Point> > tcontour;

           tcontour.push_back(contours[i]);
                       drawContours(erosion_dst,tcontour,-1,Scalar(255,0,0),2);

       }
   }

        cv::namedWindow("check");
        blur( erosion_dst, erosion_dst, Size(3,3) );
               imshow("check",erosion_dst);
               imwrite( "/home/sukhad/Desktop/result.jpg", erosion_dst );
}

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );
//blur( detected_edges, detected_edges, Size(3,3) );
/*blur( detected_edges, detected_edges, Size(3,3) );
blur( detected_edges, detected_edges, Size(3,3) );
blur( detected_edges, detected_edges, Size(3,3) );
blur( detected_edges, detected_edges, Size(3,3) );
blur( detected_edges, detected_edges, Size(3,3) );
blur( src_gray, detected_edges, Size(3,3) );*/
  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);

        Dilation(0,0);

  imshow( window_name,dst);
 }


/** @function main */
int main()
{


    VideoCapture cap(0);
    if(!cap.isOpened())
        cout<<"pappu";
    while(cap.isOpened()){

        cv::waitKey(30);
        cap.read(src);
    //Mat src=imread("/home/sukhad/Desktop/pothole.jpg",1);
        cv::namedWindow("ab");
    imshow("ab",src);
        if(!src.data){
            cout<<"No input";
            return -1;
           }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );
  //  cv::inRange(src,cv::Scalar(105, 105, 105), cv::Scalar(105, 105, 105),src_gray);
  /// Convert the image to grayscale
 cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create a window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  namedWindow("Dilation Demo", CV_WINDOW_AUTOSIZE);

namedWindow( "Erosion Demo", CV_WINDOW_AUTOSIZE );
/*/// Create Erosion Trackbar
 createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
                 &erosion_elem, max_elem,
                 Erosion );

 createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
                 &erosion_size, max_kernel_size,
                 Erosion );
 /// Create Dilation Trackbar
   createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
                   &dilation_elem, max_elem,
                   Dilation );

   createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo",
                   &dilation_size, max_kernel_size,
                   Dilation );*/
  /// Show the image
  CannyThreshold(0, 0);

  /// Wait until user exit program by pressing a key
  waitKey(0);
}
  return 0;
  }

