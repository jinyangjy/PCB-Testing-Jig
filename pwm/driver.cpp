#include <iostream>
#include "pwm.h"

int main(int argc, char *const argv[])
{
    const float freq = 2;
    const float duty = 0.5;

    bool last_val = false;
    float time_acc = 0;
    for (auto i = 0; i < 10; i++) {
        const auto v = pwm_step(&last_val, &time_acc, 0.05, duty, freq);
        std::cout << v;
    }
    std::cout << std::endl;
}


