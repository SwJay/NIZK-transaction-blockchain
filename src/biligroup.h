//
// Created by swj on 18-7-25.
//

#ifndef NIZK_BILIGROUP_H
#define NIZK_BILIGROUP_H

#include <pbc.h>

class BiliGroup{
public:
    pairing_t pairing;
    element_t g1, g2, gt, h;
public:
    BiliGroup();
    ~BiliGroup();
};

#endif //NIZK_BILIGROUP_H
