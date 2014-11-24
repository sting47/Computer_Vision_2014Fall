#include <cv.h>
#include <highgui.h>
 
using namespace cv;
 
int main()
{
 char* imageName = "lena.bmp";
 
 Mat image;
 image = imread( imageName, 1 );
 
 Mat gray_image;
 cvtColor( image, gray_image, CV_RGB2GRAY );
 
 imwrite( "testcv.bmp", gray_image );
 
 return 0;
}