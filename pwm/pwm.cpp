#include "pwm.h"

bool pwm_step(bool *last_value, float *time_acc, float time_delta, float duty, float freq)
{
    const float period = 1 / freq; // 0.5f
    const float on_time = duty * period; // 0.25f
    const float off_time = (1 - duty) * period; // 0.25f

    *time_acc = *time_acc + time_delta;
    if (*last_value) {
	if (*time_acc >= on_time) {
	    *last_value = 0;
	    *time_acc -= on_time;
	}
    } else {
	if (*time_acc >= off_time) {
	    *last_value = 1;
	    *time_acc -= off_time;
	}
    }

    return *last_value;
}

/*
 * acc	last	cur
 * 0.1	false	false
 * 0.2	false	false
 * 0.05	false	true
 *
 *         0.25      0.25
 *   0 0.1 0.2  0.05 0.15 0
 *   1 1   1    0    0    1 1 1 0 0
 *
 *   1111100000 1111100000
 */
