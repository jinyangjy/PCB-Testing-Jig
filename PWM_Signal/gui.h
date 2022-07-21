#ifndef PWM_GUI_H
#define PWM_GUI_H

#include <deque>
#include <string>

#define SAMPLE_SIZE 20

class Gui {

    float duty = 0;
    bool last_value = false;
    std::deque<bool> samples;
    float time_acc = 0;

    std::string display;
    float frame_acc = 0;

public:
    Gui();
    void do_frame(float delta, bool* running);
    void push_sample(bool sample);
};

#endif //PWM_GUI_H
