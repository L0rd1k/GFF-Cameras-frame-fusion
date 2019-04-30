/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BaseVideoFusion.h
 * Author: ilya
 *
 * Created on April 29, 2019, 11:11 AM
 */

#ifndef BASEVIDEOFUSION_H
#define BASEVIDEOFUSION_H

#include "GFF.h"

class BaseVideoFusion : public GFF {
public:
    BaseVideoFusion();
    void BaseExecution() override;
    virtual ~BaseVideoFusion();
private:
    int CheckImagesSize(vector<Mat> &vecImages);
    std::string url1,url2;
    VideoCapture cap1,cap2;
    Mat frame1,frame2;
};
#endif /* BASEVIDEOFUSION_H */

