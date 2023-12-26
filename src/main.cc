#define OGE_IMPL
#include "game.hh"


struct gui : public oge::core::imgui_layer {
    void on_update(const float& ) override {
        ImGui::Begin("Hello, world!");
        // few controls, text, label, button and slider
        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Another Window", &show_another_window);
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

        if (ImGui::Button(("Button" + std::to_string(counter)).c_str())) {
            counter++;
        }


        ImGui::SameLine();

        // another window
        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) {
                show_another_window = false;
            }
            ImGui::End();
        }

        // demo window
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        ImGui::End();
    }

    private:
        bool show_demo_window = true;
        bool show_another_window = true;
        float f = 0.0f;
        int counter = 0;
};

struct app : public oge::core::application {
    app() : oge::core::application("app") {
        push_layer(new game());
        push_layer(new gui());
    }
    ~app() = default;
};

int main() {
    
    {
        std::make_unique<app>()->run();
    }
    
    return 0;
}