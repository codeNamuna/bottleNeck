#include "core.hpp"
#include "highgui.hpp"
#include "imgcodecs.hpp"
#include "imgproc.hpp"

#include<windows.h>
#include<conio.h>
#include<iostream>

using namespace cv;
using namespace std;

int main()
{
    /*Mat img;
    img = imread("C:\\Users\\Shivang Singh\\Desktop\\pic.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    img = Mat(500,300,CV_8UC3, Scalar(0,0,100));

    if(img.empty())
    {
        cout<<"unable to load image";
        exit(1);
    }


    imshow("Original Image", img);
    waitKey();*/



    /*
    //VideoCapture vc("C:\\Users\\Shivang Singh\\Desktop\\sample.mp4");
    VideoCapture vc(1);

    if(!vc.isOpened()){
        cout<<"unable to open video file";
        exit(1);
    }

    //double fps = vc.get(CV_CAP_PROP_FPS);
    //cout<<"fps: "<<fps;

    double width = vc.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = vc.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout<<width<<" "<<height;

    namedWindow("lalutar",CV_WINDOW_AUTOSIZE);

    while(1){
        Mat img;
        bool success = vc.read(img);

        if(!success){
            cout<<"unable to read frame";
        }

        else{
            imshow("lalutar",img);
        }

        if(waitKey(30)==27)
            break;
    }*/


    VideoCapture vid(1);
    if(!vid.isOpened()){
        cout<<"error connect to camera";
        exit(1);
    }

    namedWindow("control",CV_WINDOW_AUTOSIZE);

    int iLowH=101;
    int iHighH=123;

    int iLowS=92;
    int iHighS=255;

    int iLowV=0;
    int iHighV=255;


    cvCreateTrackbar("LowH","control",&iLowH,179);
    cvCreateTrackbar("HighH","control",&iHighH,179);

    cvCreateTrackbar("LowS","control",&iLowS,255);
    cvCreateTrackbar("HighS","control",&iHighS,255);

    cvCreateTrackbar("LowV","control",&iLowV,255);
    cvCreateTrackbar("HighV","control",&iHighV,255);

    int iLastx=-1;
    int iLasty=-1;

    Mat imgtemp;
    vid.read(imgtemp);

    double width = vid.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = vid.get(CV_CAP_PROP_FRAME_HEIGHT);
    int current=0;
    int current_new=0;

    width/=8;

    Mat imgBlk = Mat::zeros(imgtemp.size(), CV_8UC3);
    Mat imgLines = Mat::zeros(imgtemp.size(), CV_8UC3);

    while(1){
        Mat img;

        bool bsuccess = vid.read(img);

        if(!bsuccess){
            cout<<"cannot read from camera";
            break;
        }

        Mat imgHSV;

        cvtColor(img,imgHSV,COLOR_BGR2HSV);

        Mat imgThreshold;

        inRange(imgHSV, Scalar(iLowH,iLowS,iLowV), Scalar(iHighH,iHighS,iHighV), imgThreshold);

        //morphological opening (remove small objects from the foreground)
        erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        dilate(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThreshold, imgThreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        Moments omoments = moments(imgThreshold);

        double dM01 = omoments.m01;
        double dM10 = omoments.m10;
        double dArea = omoments.m00;

        if(dArea>100000){
            int posx=dM10/dArea;
            int posy=dM01/dArea;

            if(iLastx>=0 && iLasty>=0 && posx>=0 && posy>=0){
                imgBlk=Mat::zeros(imgtemp.size(), CV_8UC3);
                line(imgBlk,Point(posx,posy),Point(iLastx,iLasty),Scalar(0,0,255),2);
            }

            iLastx=posx;
            iLasty=posy;
            current_new = posx/width;
        }

        imshow("thresholded image",imgThreshold);

        for(int i=0;i<8;i++){
            line(imgLines,Point(width*i,0),Point(width*i,height),Scalar(255,255,255),1);
        }

        Mat flipped;

        img = img + imgBlk + imgLines;

        flip(img,flipped,1);

        imshow("original image",flipped);

        if(current_new != current){
            current=current_new;
            switch(current_new){
                case 0:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\1.wav"), NULL,SND_ASYNC);
                    break;
                case 1:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\2.wav"), NULL,SND_ASYNC);
                    break;
                case 2:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\3.wav"), NULL,SND_ASYNC);
                    break;
                case 3:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\4.wav"), NULL,SND_ASYNC);
                    break;
                case 4:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\5.wav"), NULL,SND_ASYNC);
                    break;
                case 5:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\6.wav"), NULL,SND_ASYNC);
                    break;
                case 6:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\7.wav"), NULL,SND_ASYNC);
                    break;
                case 7:
                    PlaySound(TEXT("C:\\Users\\Shivang Singh\\Desktop\\opencv\\piano notes\\8.wav"), NULL,SND_ASYNC);
                    break;
            }

        }

        if(waitKey(30)==27){
            cout<<"Escaping";
            break;
        }

    }

    return 0;

}
