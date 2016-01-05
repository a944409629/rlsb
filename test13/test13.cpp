// test13.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

#include "cv.h" 
#include "highgui.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h> 
#include <math.h> 
#include <float.h> 
#include <limits.h> 
#include <time.h> 
#include <ctype.h>

#ifdef _EiC 
#define WIN32 
#endif
static CvMemStorage* storage = 0; 
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );

const char* cascade_name = ""; 

int main( int argc, char** argv ) 
{
    
 cascade_name = "D:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
 cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 ); 
  
    if( !cascade ) 
    { 
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" ); 
        return 0; 
    } 
    storage = cvCreateMemStorage(0); 
    cvNamedWindow( "人脸识别", 1 ); 
     
    const char* filename = "48.jpg"; // - 待识别人脸的图片的路径
    IplImage* image = cvLoadImage( filename, 1 );

    if( image ) 
    { 
        detect_and_draw( image ); 
        cvWaitKey(0); 
        cvReleaseImage( &image );
	}

    cvDestroyWindow("人脸识别"); 
  
    return 0; 
}


void detect_and_draw(IplImage* img ) 
{ 
    double scale=0.3; 
    static CvScalar colors[] = { 
		{{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}}, 
        {{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}} 
    };//Just some pretty colors to draw with
	//标记图片时的颜色
    // - 图片预处理
    IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1); 
    IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1); 
    cvCvtColor(img,gray, CV_BGR2GRAY); 
    cvResize(gray, small_img, CV_INTER_LINEAR);

   // cvEqualizeHist(small_img,small_img); //直方图均衡

    //Detect objects if any 
    // 
    cvClearMemStorage(storage); 
    double t = (double)cvGetTickCount(); 
    CvSeq* objects = cvHaarDetectObjects(small_img, 
    cascade,storage,1.1,2,0/*CV_HAAR_DO_CANNY_PRUNING*/,cvSize(10,10));

    t = (double)cvGetTickCount() - t; 
    printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );

    //Loop through found objects and draw boxes around them 
    for(int i=0;i<(objects? objects->total:0);++i) 
    { 
        CvRect* r=(CvRect*)cvGetSeqElem(objects,i); 
		cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale),colors[i%4]);//colors[i%4]); 
    } 
    for( int i = 0; i < (objects? objects->total : 0); i++ ) 
    { 
        CvRect* r = (CvRect*)cvGetSeqElem( objects, i ); 
        CvPoint center; 
        int radius; 
        center.x = cvRound((r->x + r->width*0.5)*scale); 
        center.y = cvRound((r->y + r->height*0.5)*scale); 
        radius = cvRound((r->width + r->height)*0.25*scale); 
        cvCircle( img, center, radius, colors[i%3], 2, 6, 0 ); 
    }

    cvShowImage( "人脸识别", img ); 
    cvReleaseImage(&gray); 
    //cvReleaseImage(&small_img); 
	}