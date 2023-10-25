//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    auto program = xe::utils::create_program(
            {
                    {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
                    {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
            });


    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }

    //        coordinates   |       colors
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    std::vector<GLubyte> indexes = {
            0, 1, 2,
            3, 4, 5,
            5, 4, 6
    };

    auto u_mixer_index = glGetUniformBlockIndex(program, "Mixer");
    if (u_mixer_index == GL_INVALID_INDEX) {
        SPDLOG_WARN("Cannot find Mixer uniform block in program");
    } else {
        glUniformBlockBinding(program, u_mixer_index,0);
    }

    GLuint v_buffer_handle;
    OGL_CALL(glGenBuffers(1, &v_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    OGL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLuint i_buffer_handle;
    OGL_CALL(glGenBuffers(1, &i_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));
    OGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(GLubyte), indexes.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    GLuint interface_buffer_handle;
    OGL_CALL(glGenBuffers(1, &interface_buffer_handle));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, interface_buffer_handle));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW));

    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));

    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(0)));

    OGL_CALL(glEnableVertexAttribArray(1));

    OGL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat))))

    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));

    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glUseProgram(program));
}

void SimpleShapeApplication::frame() {
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_BYTE, 0));
    OGL_CALL(glBindVertexArray(0));
}
