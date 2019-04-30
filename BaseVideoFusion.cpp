/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BaseVideoFusion.cpp
 * Author: ilya
 * 
 * Created on April 29, 2019, 11:11 AM
 */

#include "BaseVideoFusion.h"

BaseVideoFusion::BaseVideoFusion() {
    url1 = "/opt/data/videos/OES-footage/ch0_2018.12.06_12.46.19.avi";
    url2 = "/opt/data/videos/OES-footage/ch1_2018.12.06_12.46.19.avi";
}


void BaseVideoFusion::BaseExecution() {
    VideoCapture cap1(url1);
    VideoCapture cap2(url2);
    while (true) {
        cap1 >> frame1;
        cap2 >> frame2;
        vector<Mat> vecImages;
        resize(frame1, frame1, cv::Size(768, 576)); // in case if images are different
        vecImages.push_back(frame1);
        vecImages.push_back(frame2);
        CheckImagesSize(vecImages);
        vector<Mat> *vecI_gray = new vector<Mat>;
        ColorConvertation(vecImages, *vecI_gray);
        vector<Mat> vecH;
        LaplacianOfTheImage(*vecI_gray, vecH);
        vector<Mat> vecS;
        Saliency(vecH, vecS);
        high_resolution_clock::time_point beg = high_resolution_clock::now();
        vector<Mat> vecP;
        BinaryWeights(vecS, vecP);
        vector<Mat> vecWB;
        vector<Mat> vecWD;
        GFFOptimization(vecP, *vecI_gray, vecWB, vecWD);
        Mat F;
        Fusion(vecImages, vecWB, vecWD, F);
        F.convertTo(F, CV_8U);
        high_resolution_clock::time_point end = high_resolution_clock::now();       
        duration<double> time_res = duration_cast<duration<double>>(end - beg);
        std::cout << "it takes in total time: " << time_res.count() << " seconds." << std::endl;
        cv::imshow("F", F);
        waitKey(1);
    }
}

BaseVideoFusion::~BaseVideoFusion() {
}

