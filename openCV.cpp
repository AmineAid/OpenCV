#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <array>
using namespace cv;
using namespace std;
static void toGray(Mat& img) {
  int i, j;
  uint8_t * dd = img.data;
  assert(img.channels() == 3);
  for(i = 0; i < img.rows; ++i) {
    for(j = 0; j < img.cols; ++j) {
      double v = 0;
      v += 0.114 * dd[i * img.step + j * img.channels() + 0]; //B
      v += 0.587 * dd[i * img.step + j * img.channels() + 1]; //G
      v += 0.299 * dd[i * img.step + j * img.channels() + 2]; //R
      dd[i * img.step + j * img.channels() + 0] = (uint8_t)v;
      dd[i * img.step + j * img.channels() + 1] = (uint8_t)v;
      dd[i * img.step + j * img.channels() + 2] = (uint8_t)v;
    }
  }
}




static void addnoise(Mat& img, int noiselevel) {
  int i, j;
  uint8_t * dd = img.data;
  assert(img.channels() == 3);
  for(i = 0; i < img.rows; ++i) {
    for(j = img.cols/2; j < img.cols; ++j) {
      int v = rand() % noiselevel; 
      if(v==0){
      dd[i * img.step + j * img.channels() + 0] = 0;
      dd[i * img.step + j * img.channels() + 1] = 0;
      dd[i * img.step + j * img.channels() + 2] = 0;
    }
     if(v==1){
      dd[i * img.step + j * img.channels() + 0] = 255;
      dd[i * img.step + j * img.channels() + 1] = 255;
      dd[i * img.step + j * img.channels() + 2] = 255;
    }

    }

  }
}


double findMedian(int a[], int n) 
{ 
    sort(a, a+n); 
    if (n % 2 != 0) 
       return (double)a[n/2];       
    return (double)(a[(n-1)/2] + a[n/2])/2.0; 
} 


void insertionSort(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

static void medianremove(Mat& img){
int window[9];
 
 Mat dst = img.clone();
        for(int y = 0; y < img.rows; y++)
            for(int x = 0; x < img.cols; x++)
                dst.at<uchar>(y,x) = 0.0;
 
        for(int y = 1; y < img.rows - 1; y++){
            for(int x = 1; x < img.cols - 1; x++){
                 window[0] = img.at<uchar>(y - 1 ,x - 1);
                window[1] = img.at<uchar>(y, x - 1);
                window[2] = img.at<uchar>(y + 1, x - 1);
                window[3] = img.at<uchar>(y - 1, x);
                window[4] = img.at<uchar>(y, x);
                window[5] = img.at<uchar>(y + 1, x);
                window[6] = img.at<uchar>(y - 1, x + 1);
                window[7] = img.at<uchar>(y, x + 1);
                window[8] = img.at<uchar>(y + 1, x + 1);
                insertionSort(window);

                dst.at<uchar>(y,x) = window[4];

                cout << findMedian(window,9)
;                cout << " ";
            }
        }

}





int main(int argc, char ** argv) {
  VideoCapture cap(1);
  if(!cap.isOpened()) {
    cap.open(CV_CAP_ANY);
    if(!cap.isOpened())
      return 1;
  }
  Size s(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
  cerr << "dimensions du flux " << s.width << "x" << s.height << endl;
  namedWindow("Addnoisef", WINDOW_AUTOSIZE);
  namedWindow("AUTO_No_noise", WINDOW_AUTOSIZE);
  Mat src,img, frame,frame2;
  for(;;) {
    cap >> frame;
    frame2=frame;
    img=frame;
    src=frame;
    toGray(frame);
    addnoise(img,500);
    imshow("Addnoisef", img);
    medianremove(img);
    imshow("AUTO_No_noise", img);
    //src= cv::medianBlur(src,img,3);
    if((waitKey(10) & 0xFF) == 27)
      break;
  }
  return 0;
}