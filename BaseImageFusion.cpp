/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BaseImageFusion.cpp
 * Author: ilya
 * 
 * Created on April 29, 2019, 11:10 AM
 */

#include "BaseImageFusion.h"

BaseImageFusion::BaseImageFusion() {
    pathFolder = "/home/ilya/NetBeansProjects/VideoToolbox/data/colourset/";
}

BaseImageFusion::~BaseImageFusion() {
}

void BaseImageFusion::ImagesList(vector<Mat> &vecImages) {
    vecImages.push_back(cv::imread(pathFolder + "visible.png"));
    vecImages.push_back(cv::imread(pathFolder + "visible-color.png"));
}

void BaseImageFusion::BaseExecution() {
    vector<Mat> vecImages;
    ImagesList(vecImages);
    CheckImagesSize(vecImages);
    vector<Mat> *vecI_gray = new vector<Mat>;
    ColorConvertation(vecImages, *vecI_gray);
    vector<Mat> vecH;
    LaplacianOfTheImage(*vecI_gray, vecH);
    vector<Mat> vecS;
    Saliency(vecH, vecS);
    vector<Mat> vecP;
    BinaryWeights(vecS, vecP);
    vector<Mat> vecWB;
    vector<Mat> vecWD;
    GFFOptimization(vecP, *vecI_gray, vecWB, vecWD);
    Mat F;
    Fusion(vecImages, vecWB, vecWD, F);
    F.convertTo(F, CV_8U);
    cv::imshow("F", F);
    waitKey();
}
