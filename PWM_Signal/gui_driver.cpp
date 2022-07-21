#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "imgui.h"
#include "imgui_impl_allegro5.h"

#include "gui.h"

int main(int argc, char **argv)
{
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);

    ALLEGRO_DISPLAY *display = al_create_display(1280, 720);
    al_set_window_title(display, "PWM");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    ImGui::StyleColorsClassic();
    ImGui_ImplAllegro5_Init(display);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    io.Fonts->AddFontFromFileTTF("Mx437_DOS-V_TWN19.ttf", 38);

    Gui gui;

    bool running = true;
    while (running) {
        ALLEGRO_EVENT ev;
        while (al_get_next_event(queue, &ev)) {
            ImGui_ImplAllegro5_ProcessEvent(&ev);
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                running = false;
            if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
                ImGui_ImplAllegro5_InvalidateDeviceObjects();
                al_acknowledge_resize(display);
                ImGui_ImplAllegro5_CreateDeviceObjects();
            }
        }

        ImGui_ImplAllegro5_NewFrame();
        ImGui::NewFrame();

        gui.do_frame(io.DeltaTime, &running);

        ImGui::Render();
        al_clear_to_color(al_map_rgba_f(
                clear_color.x * clear_color.w,
                clear_color.y * clear_color.w,
                clear_color.z * clear_color.w,
                clear_color.w));
        ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());
        al_flip_display();
    }

    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}
