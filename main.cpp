/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: ilya petrikov
 *
 * Created on April 2, 2019, 2:55 PM
 */

#include <cstdlib>
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include "GFF.h"
#include "BaseVideoFusion.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    int chooseValue = 0;
    std::cout << "1 - Images fusion" << std::endl;
    std::cout << "2 - Video fusion" << std::endl;
    cin >> chooseValue;
    
    BaseVideoFusion *bf = new BaseVideoFusion();
    
    switch(chooseValue)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
            bf->BaseExecution();
            break;
        }
        default:
            return 0;
    }
    
    return 0;
}

