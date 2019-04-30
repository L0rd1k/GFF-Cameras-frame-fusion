/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BaseImageFusion.h
 * Author: ilya
 *
 * Created on April 29, 2019, 11:10 AM
 */

#ifndef BASEIMAGEFUSION_H
#define BASEIMAGEFUSION_H

#include "GFF.h"

class BaseImageFusion : public GFF {
public:
    BaseImageFusion();
    void BaseExecution() override;
    virtual ~BaseImageFusion();
    void ImagesList(vector<Mat> &vecImages);
private:
    vector<Mat> vecImages;
    string pathFolder;
};

#endif /* BASEIMAGEFUSION_H */

