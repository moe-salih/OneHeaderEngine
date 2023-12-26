

#include "game.hh"

void game::on_attach() {
    // draw the ground 


    // a square containing the lower half of the screen
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f, -0.3f, 0.0f, // top left
         1.0f, -0.3f, 0.0f, // top right
         1.0f, -1.0f, 0.0f  // bottom right
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // attribute 0
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // index buffer object
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void game::on_detach() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
}

void game::on_update(const float&) {
    // clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // draw the square
    m_shader.bind();

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void game::on_event(oge::events::event&) {
    // handle events

}