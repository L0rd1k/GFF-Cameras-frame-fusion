/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GFF.h
 * Author: ilya
 *
 * Created on April 29, 2019, 10:43 AM
 */

#ifndef GFF_H
#define GFF_H

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/ximgproc.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;
using namespace std::chrono;

class GFF {
public:
    GFF();
    virtual ~GFF();
public:
    virtual void BaseExecution() = 0;
    void ColorConvertation(const vector<Mat>& vecImages, vector<Mat>& vecImages_gray);
    void LaplacianOfTheImage(const vector<Mat> &vecImages_gray, vector<Mat> &vecKernel);
    void Saliency(const vector<Mat> &vecKernel, vector<Mat> &vecSaliency);
    void BinaryWeights(const vector<Mat> &vecSaliency, vector<Mat> &vecW);
    void GFFOptimization(const vector<Mat> &vecW, const vector<Mat> &vecImage_gray, vector<Mat> &vecWB, vector<Mat> &vecWD);
    void GFFDecomposition(const vector<Mat>& vecImage, vector<Mat>& vecB, vector<Mat>& vecD);
    void Fusion(const vector<Mat>& vecImage, vector<Mat>& vecWB, vector<Mat>& vecWD, Mat& F);
private:
};

#endif /* GFF_H */

