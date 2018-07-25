//
// Created by swj on 18-7-25.
//

#include "DSC.h"
#include <ctime>
#include <random>

using namespace std;

/* DSC Ctor
 * Setup: generate bilinear group & generate public parameter
 *
 */
DSC::DSC(uint n) {
    int lambda; // signkey for Boneh-Boyen signature
    element_pp_t g_pp;

/* wrong lambda 以p为消息空间
 * default_random_engine engine((unsigned long)time(nullptr));
    uniform_int_distribution<> dis(0, 1023);
    lambda = dis(engine); // generate lambda randomly*/

    group = new BiliGroup(n);
    element_pp_init(g_pp, group->g1);
    for (int i = 0; i < 1024; ++i) {
        element_pp_pow(sigma[i], );

    }
}
