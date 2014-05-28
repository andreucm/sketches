
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(int argc, char *argv[])
{
        cv::Mat rawImage, outImage, thImage, artifImage; 
        cv::Scalar_<uchar> pixel1,pixel2;
        cv::Rect_<int> box;
               
        if(argc<2)
        {
                std::cout << "Usage: ./matMainOpenCV2 <image-file-name>" << std::endl;
                return -1;
        }
        
        // load rawImage from the file provided by the user
        rawImage = cv::imread(argv[1], 1);
        if(rawImage.data == NULL)
        {
                std::cout << "Could not load outImage file: " << argv[1] << std::endl;
                return -1;
        }
        
        //initializes windows
        cv::namedWindow("rawImage",CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);//, CV_WINDOW_AUTOSIZE);
        cv::moveWindow("rawImage",20,50);
        cv::namedWindow("outImage",CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);//, CV_WINDOW_AUTOSIZE);
        cv::moveWindow("outImage",420,50);
        cv::namedWindow("ArtificialImage",CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);//, CV_WINDOW_AUTOSIZE);
        cv::moveWindow("ArtificialImage",820,50);

        //clone (memcopy) raw image to outImage, since the later will be manipulated and we want to keep original image
        outImage = rawImage.clone();
        
        //get some info about the outImage
        std::cout << "Image size is (wxh): " << outImage.cols << " x " << outImage.rows << std::endl;
        
        //Get the value of the central pixel
        pixel1 = outImage.at< cv::Scalar_<uchar> >(outImage.rows/2,outImage.cols/2);
        pixel2 = outImage.at< cv::Scalar_<uchar> >(cv::Point(outImage.cols/2,outImage.rows/2));
//         uchar pixelB = outImage.at<uchar>(outImage.rows/2,outImage.cols/2)[0];
//         uchar pixelG = outImage.at<uchar>(outImage.rows/2,outImage.cols/2,1);
//         uchar pixelR = outImage.at<uchar>(outImage.rows/2,outImage.cols/2,2);
        std::cout << "Central pixel1 value is: " << (int)pixel1[0] << ","  << (int)pixel1[1] << "," << (int)pixel1[2] << "," << (int)pixel1[3] << std::endl;
        std::cout << "Central pixel2 value is: " << (int)pixel2[0] << ","  << (int)pixel2[1] << "," << (int)pixel2[2] << "," << (int)pixel2[3] << std::endl;
//         std::cout << "Central pixel2 value is: " << (int)pixelB << ","  << (int)pixelG << "," << (int)pixelR << "," << (int)pixel2[3] << std::endl;
        
        //draw a green 30x30 rectangle around that central pixel. Trait width 5
        box.x = outImage.cols/2-15;
        box.y = outImage.rows/2-15;
        box.width = 30;
        box.height = 30;
        cv::rectangle(outImage, box, cv::Scalar(0,255,0), 5);
        
        //draw a 100x100 patch on the up-right corner filled with the previous obtained color 
        outImage(cv::Range(1,100),cv::Range(outImage.cols-100,outImage.cols)) = pixel1;
        //outImage(cv::Range(1,100),cv::Range(1,100)) = pixel1;
        
        //draw a 50x50 patch on the down-right corner filled with pure red
        outImage(cv::Range(outImage.rows-50,outImage.rows),cv::Range(outImage.cols-50,outImage.cols)) = cv::Scalar_<uchar>(0,0,255);
        
        //flood fill from the bottom-central pixel
        //cv::floodFill(outImage, cv::Point(outImage.cols/2,outImage.rows-1), cv::Scalar(0,0,0), &box, cv::Scalar_<uchar>(5, 5, 5, 1), cv::Scalar_<uchar>(10, 10, 10, 1) ); 
        cv::floodFill(outImage, cv::Point(outImage.cols/2,outImage.rows-1), pixel1, &box, cv::Scalar_<uchar>(5, 5, 5, 1), cv::Scalar_<uchar>(10, 10, 10, 1) ); 
        std::cout << "Flood bounding box size is: " << box.width << " x " << box.height << std::endl;
        
        //draw flood bounding box in blue
        cv::rectangle(outImage, box, cv::Scalar(255,0,0), 5);
        
        //draw a green line from the left-bottom corner to the center of the flood box
        cv::line(outImage,cv::Point(0,outImage.rows-1),cv::Point(box.x+box.width/2,box.y+box.height/2), cv::Scalar(0,255,0), 5);
        
        //creates an artificial image
        artifImage.create(1000,1000,CV_8UC3);
        artifImage = cv::Scalar(0,255,0,1);
        
        //display all images
        cv::imshow("rawImage", rawImage);// show the outImage
        cv::imshow("outImage", outImage);// show the outImage
        cv::imshow("ArtificialImage", artifImage);// show the outImage
        cv::waitKey(0); // wait for a key

        //ends 
        return 0;
}