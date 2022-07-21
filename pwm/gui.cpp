#include "imgui.h"

#include "gui.h"
#include "pwm.h"

Gui::Gui()
{
    samples.resize(SAMPLE_SIZE);
    for (auto i = 0; i < SAMPLE_SIZE; i++)
        samples.push_back(false);
}

void Gui::do_frame(float delta, bool *running)
{
    ImGui::Begin("WINDOW");

    ImGui::Text("duty: %.4f", duty);

    if (ImGui::Button("TOGGLE")) {
        duty = (duty != 0.2f) ? 0.2f : 0.5f;
    }

    static float arr[SAMPLE_SIZE];

    frame_acc += delta;
    if (frame_acc > 0.2) {
        display.clear();
        push_sample(pwm_step(&last_value, &time_acc, frame_acc, duty, 4));
	int i = 0;
        for (auto const& sample: samples) {
            if (sample) {
                display += '1';
		arr[i] = 1.0f;
	    } else {
                display += '0';
		arr[i] = 0.0f;
	    }
	    i++;
	}
        frame_acc = 0;
    }

    ImGui::PlotLines("PWM", arr, IM_ARRAYSIZE(arr));
    ImGui::TextUnformatted(display.c_str());

    if (ImGui::Button("CLOSE"))
        *running = false;

    ImGui::End();
}

void Gui::push_sample(bool sample)
{
    samples.pop_front();
    samples.push_back(sample);
}
