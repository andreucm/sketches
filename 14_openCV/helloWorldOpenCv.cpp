////////////////////////////////////////////////////////////////////////
//
// helloWorldOpenCv.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file, inverts it, and displays the result. 
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char *argv[])
{
	IplImage* img = 0; 
	IplImage* img2 = 0;
	int height,width,step,channels;
	uchar *data;
	int i,j,k;
	
	if(argc<2){
		printf("Usage: main <image-file-name>\n\7");
		exit(0);}
	
	// load an image  
	img=cvLoadImage(argv[1]);
	if(!img){
	printf("Could not load image file: %s\n",argv[1]);
	exit(0);
	}
	
	// get the image data
	height    = img->height;
	width     = img->width;
	step      = img->widthStep;
	channels  = img->nChannels;
	data      = (uchar *)img->imageData;
	printf("Processing a %dx%d image with %d channels and widthStep=%d\n",height,width,channels,step); 
	
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); // create a window
	cvMoveWindow("mainWin", 100, 100);//offset from the upper-left corner of the screen
	
	// invert the image
	for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
	data[i*step+j*channels+k]=255-data[i*step+j*channels+k];	
	cvShowImage("mainWin1", img );	// show the image
	cvWaitKey(0); // wait for a key

	cvCircle(img, cvPoint(100,100), 20, cvScalar(0,255,0), 1);//draw a circle at (100,100) with a r=20. green lines of width 1
	cvShowImage("mainWin2", img );	// show the image
	cvWaitKey(0); // wait for a key

	img2=cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_32F,3);//allocates memory for image 2
	cvLaplace( img, img2, 3); //computes Laplacian of img and puts the result on img2. 
	cvShowImage("mainWin3", img2 );// show the image
	cvWaitKey(0); // wait for a key
	
	// release the images
	cvReleaseImage(&img );
	cvReleaseImage(&img2 );
	return 0;
}
