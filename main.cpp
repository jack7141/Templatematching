#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
vector<Point2f>contours;
vector<vector<Point> > contours2;
int findBiggestContour(vector<vector<Point>>contour)
{
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i=0;i<contour.size();i++) {
        if (contour[i].size() > sizeOfBiggestContour){

            sizeOfBiggestContour=contour[i].size();
            indexOfBiggestContour=i;

        }
    }
    return indexOfBiggestContour;
}
//int findBiggestPoint(vector<vector<Point>>contour)
//{
//    int indexOfBiggestContour = -1;
//    int sizeOfBiggestContour = 0;
//    for (int i=0;contour.size();i++) {
//        contour.at(i);

//    }
//}
vector<Point> contoursConvexHull( vector<vector<Point> > contours )
{
    vector<Point> result;
    vector<Point> pts;
    for ( size_t i = 0; i< contours.size(); i++)
        for ( size_t j = 0; j< contours[i].size(); j++)
            pts.push_back(contours[i][j]);
    convexHull( pts, result );
    return result;
}
int main()
{
    VideoCapture cam(1);
    Mat sample = imread("C:/Users/ksrnd/Desktop/asfasdgasgeg_a.png");
    namedWindow("Control",CV_WINDOW_AUTOSIZE);

    int match_method=0;
    createTrackbar("Method", "Control", &match_method ,5);
    while (true) {
        Mat frame;
        cam >> frame;

        int result_cols = frame.cols - sample.cols +1;
        int result_rows = frame.rows - sample.rows +1;

        Mat result;
        result.create(result_rows,result_cols,CV_32FC1);

        matchTemplate(frame,sample,result,match_method);
        normalize(result,result,0,1,NORM_MINMAX,-1,Mat());

        double minVal; double maxVal; Point minLoc; Point maxLoc;
        Point matchLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

        if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
        { matchLoc = minLoc; }
        else
        { matchLoc = maxLoc; }

            rectangle( frame, matchLoc, Point( matchLoc.x + sample.cols , matchLoc.y + sample.rows ), Scalar::all(0), 2, 8, 0 );
            rectangle( result, matchLoc, Point( matchLoc.x + sample.cols , matchLoc.y + sample.rows ), Scalar::all(0), 2, 8, 0 );

        Mat Roi = frame(Rect(matchLoc,Point( matchLoc.x + sample.cols , matchLoc.y + sample.rows )));
        Mat prup;
        pyrUp(Roi,prup);
        Mat gray,hsv,hsv2;
        cvtColor(prup, hsv, COLOR_BGR2HSV);
        Mat mask,mask2;
        inRange(hsv, Scalar(0, 130, 0), Scalar(179, 255, 255), mask);
        dilate( mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        Mat mask_inv,mask_inv2;
        bitwise_not(mask,mask_inv,noArray());

        Mat Test,Test2;
        bitwise_and(prup,prup,Test,mask_inv);

        Mat crane;
        Test.copyTo(crane);
        cvtColor(crane, hsv2, COLOR_BGR2HSV);
        inRange(hsv2, Scalar(71, 35, 179), Scalar(179, 255, 255), mask2);

        dilate( mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );
        dilate( mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );
        dilate( mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );

        //morphological closing (fill small holes in the foreground)
        erode(mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );
        erode(mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );
        erode(mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );
        erode(mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );
        erode(mask2, mask2, getStructuringElement(MORPH_RECT, Size(5, 5)) );

        bitwise_and(crane,crane,Test2,mask2);
        cvtColor(Test2,gray,COLOR_RGB2GRAY);
        blur(gray,gray,Size(5,5));
        Mat canny;
        Canny(gray,canny,0,100,3,true);

        imshow("gray",canny);
        imshow("ROi",Roi);

        imshow("frame",frame);
        //    imshow("result",result);
        if (waitKey(1)==27)break;
    }
}
