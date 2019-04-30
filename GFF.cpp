/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GFF.cpp
 * Author: ilya
 * 
 * Created on April 29, 2019, 10:43 AM
 */

#include "GFF.h"

GFF::GFF() {
}

GFF::~GFF() {
}

void GFF::ColorConvertation(const vector<Mat>& vecImages, vector<Mat>& vecImages_gray) {
    vecImages_gray.reserve(vecImages.size());
    for (vector<Mat>::const_iterator iter = vecImages.begin(); iter != vecImages.end(); ++iter) {
        Mat gray;
        cvtColor(*iter, gray, COLOR_BGR2GRAY);
        vecImages_gray.push_back(gray);
    }
}

void GFF::LaplacianOfTheImage(const vector<Mat> &vecImages_gray, vector<Mat> &vecKernel) {
    vecKernel.reserve(vecImages_gray.size());
    Mat laplaceFilter = (Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1); //discrete approximations to the Laplacian filter.
    for (vector<Mat>::const_iterator iter = vecImages_gray.begin(); iter != vecImages_gray.end(); ++iter) {
        Mat convolveLaplace;
        filter2D(*iter, convolveLaplace, -1, laplaceFilter);
        vecKernel.push_back(convolveLaplace);
    }
}

void GFF::Saliency(const vector<Mat> &vecKernel, vector<Mat> &vecSaliency) {
    vecSaliency.reserve(vecKernel.size());
    for (vector<Mat>::const_iterator it = vecKernel.begin(); it != vecKernel.end(); ++it) {
        Mat absKernel = abs(*it);
        vecSaliency.push_back(absKernel);
    }
}

void GFF::BinaryWeights(const vector<Mat> &vecSaliency, vector<Mat> &vecW) {

    high_resolution_clock::time_point beg = high_resolution_clock::now();

    vecW.reserve(vecSaliency.size());
    for (int it = 0; it < vecSaliency.size(); ++it) {
        vecW.push_back(Mat::zeros(vecSaliency.front().size(), CV_32FC1));
    }
    int width = vecSaliency.front().cols;
    int height = vecSaliency.front().rows;

    for (int x = 0; x < height; ++x) {
        for (int y = 0; y < width; ++y) {
            int iter = 0;
            float maxValue = 0.0f;
            for (int i = 0; i < vecSaliency.size(); ++i) {
                if (maxValue < vecSaliency[i].at<float>(x, y)) {
                    maxValue = vecSaliency[i].at<float>(x, y);
                    iter = i;
                }
            }
            vecW[iter].at<float>(x, y) = 1.0f;
        }
    }
}

void GFF::GFFOptimization(const vector<Mat> &vecW, const vector<Mat> &vecImage_gray, vector<Mat> &vecWB, vector<Mat> &vecWD) {
    high_resolution_clock::time_point beg = high_resolution_clock::now();
    //vecWB.reserve(vecW.size());
    //vecWD.reserve(vecW.size());
    cout << "VECSIZE" << vecW.size() << endl;

    for (size_t it = 0; it < vecW.size(); it++) {
        Mat wb, wd;
        ximgproc::guidedFilter(vecImage_gray[it], vecW[it], wb, 45, 0.3 * 255 * 255);
        ximgproc::guidedFilter(vecImage_gray[it], vecW[it], wd, 7, 1e-6 * 255 * 255);
        vecWB.push_back(wb);
        vecWD.push_back(wd);
    }

    //high_resolution_clock::time_point beg = high_resolution_clock::now();

    const int *width = &vecW.begin()->cols;
    const int *height = &vecW.begin()->rows;

    for (int x = 0; x < *height; ++x) {
        for (int y = 0; y < *width; ++y) {
            float sumB = 0.0f, sumD = 0.0f;
            for (size_t i = 0; i < vecWB.size(); ++i) {
                float fB = vecWB[i].at<float>(x, y);
                if (fB > 1.0f) {
                    vecWB[i].at<float>(x, y) = 1.0f;
                    fB = 1.0f;
                }
                sumB += fB;
                float fD = vecWD[i].at<float>(x, y);
                if (fD > 1.0f) {
                    vecWD[i].at<float>(y, x) = 1.0f;
                    fD = 1.0f;
                }
                sumD += fD;
            }
            for (size_t i = 0; i < vecWB.size(); ++i) {
                vecWB[i].at<float>(x, y) /= sumB;
                vecWD[i].at<float>(x, y) /= sumD;
            }
        }
    }

    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_res = duration_cast<duration<double>>(end - beg);
    std::cout << "OPTIMIZATION: " << time_res.count() << " seconds." << std::endl;

}

void GFF::GFFDecomposition(const vector<Mat>& vecImage, vector<Mat>& vecB, vector<Mat>& vecD) {
    vecB.reserve(vecImage.size());
    vecD.reserve(vecImage.size());
    for (vector<Mat>::const_iterator it = vecImage.begin(); it != vecImage.end(); ++it) {
        Mat b;
        boxFilter(*it, b, -1, cv::Size(31, 31));
        Mat d = *it - b;
        vecB.push_back(b);
        vecD.push_back(d);
    }
}

void GFF::Fusion(const vector<Mat>& vecImage, vector<Mat>& vecWB, vector<Mat>& vecWD, Mat& F) {

    high_resolution_clock::time_point beg = high_resolution_clock::now();


    F = cv::Mat::zeros(vecImage.begin()->size(), vecImage.begin()->type());

    std::vector<cv::Mat> vecB;
    std::vector<cv::Mat> vecD;
    GFFDecomposition(vecImage, vecB, vecD);

    if (vecImage.begin()->channels() == 3) {
        std::vector<cv::Mat> vec;
        for (size_t i = 0; i < vecWB.size(); ++i) {
            vec.clear();

            for (int it = 0; it < 3; it++) {
                vec.push_back(vecWB[i]);
            }
            cv::merge(vec, vecWB[i]);
            vec.clear();
            for (int it = 0; it < 3; it++) {
                 vec.push_back(vecWD[i]);
            }
            cv::merge(vec, vecWD[i]);
        }
    }
    for (size_t i = 0; i < vecImage.size(); ++i) {
        cv::Mat temp1 = vecWB[i].mul(vecB[i]);
        cv::Mat temp2 = vecWD[i].mul(vecD[i]);
        F = F + temp1 + temp2;
    }

    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_res = duration_cast<duration<double>>(end - beg);
    std::cout << "FUSION: " << time_res.count() << " seconds." << std::endl;

}

