//
// Created by swj on 18-7-25.
//

#ifndef NIZK_DSC_H
#define NIZK_DSC_H

#include <pbc.h>
#include "biligroup.h"
#include "account.h"
#include <vector>

#define RANGE 1024

class DSC {
public:
    BiliGroup *group;
    element_t vk;
    element_t sigma[RANGE], T[RANGE];
public:
    DSC();
    ~DSC();

};

#endif