#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "pwm.h"

TEST_CASE("Simple duty cycle counting")
{
    const float duty = 0.5f;
    const float frame_time = 0.05;
    const float freq = 2;
    const int sample_size = 10;

    bool last_value = false;
    float time_acc = 0;

    int sum = 0;
    for (int i = 0; i < sample_size; i++) {
	sum += pwm_step(&last_value, &time_acc, frame_time, duty, freq);
    }

    CHECK((float) sum / sample_size == duty);
}

TEST_CASE("Count duty cycle with frequency")
{
    const float duty = 0.5f;
    const float frame_time = 0.1;
    const float freq = 2;
    const int sample_size = 10;

    bool last_value = false;
    float time_acc = 0;

    const float period = 1 / freq; // 0.5f
    const float on_time = duty * period; // 0.25f
    const float off_time = (1 - duty) * period; // 0.25f

    for (int i = 0; i < sample_size; i++) {
	if (pwm_step(&last_value, &time_acc, frame_time, duty, freq)) {
	    CHECK(time_acc < on_time);
	} else {
	    CHECK(time_acc < off_time);
	}
    }
}
