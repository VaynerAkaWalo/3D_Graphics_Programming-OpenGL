//
// Created by pbialas on 25.09.2020.
//


#include "app.h"

#include <vector>

#include "spdlog/spdlog.h"

#include "glad/gl.h"

#include "Application/utils.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

    //        coordinates       |       colors
    std::vector<GLfloat> vertices = {
            -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // b-tl 0
            0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // b-tr
            -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // b-bl
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, // b-br

            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // apex-red 4
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //apex-green 5
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //apex-blue 6
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, //apex-yellow 7

            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // green-tl 8
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // green-bl

            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // blue-tl 10
            0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // blue-tr

            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // red-tr 12
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // red-br

            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // yellow-bl 14
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // yellow-br
    };

    std::vector<GLubyte> indexes = {
            2, 0, 1,
            3, 2, 1,

            9, 5, 8, //green
            10, 6, 11, // blue
            13, 12, 4, // red
            14, 15, 7 //yellow
    };

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        SPDLOG_WARN("Cannot find Transformations uniform block in program");
    } else {
        glUniformBlockBinding(program, u_transformations_index,1);
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

    GLuint transformations_buffer_handle;
    OGL_CALL(glGenBuffers(1, &transformations_buffer_handle));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, transformations_buffer_handle));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    auto [w, h] = frame_buffer_size();

    glm::mat4 PVM(1.0f);

    glm::mat4 M(1.0);

    glm::mat4 V(1.0);
    V = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 P(1.0);
    P = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 20.0f);

    PVM = P * V * M;

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, transformations_buffer_handle));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &PVM[0]));

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

    OGL_CALL(glViewport(0, 0, w, h));
    glEnable(GL_DEPTH_TEST);

    OGL_CALL(glUseProgram(program));
    glEnable(GL_CULL_FACE);
}

void SimpleShapeApplication::frame() {
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, 0));
    OGL_CALL(glBindVertexArray(0));
}
