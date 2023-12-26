



#ifndef GAME_HH
#define GAME_HH

#include "oge.hh"

struct game : public oge::core::layer {
    public:
        game() : layer("Game") , m_shader("res/shaders/v.glsl", "res/shaders/f.glsl") {}

        void on_attach() override;
        void on_detach() override;

        void on_update(const float& ) override;
        void on_event(oge::events::event& e) override;

    private:
        unsigned int m_vao, m_vbo, m_ibo;
        oge::utils::shader m_shader;
};

#endif//GAME_HH