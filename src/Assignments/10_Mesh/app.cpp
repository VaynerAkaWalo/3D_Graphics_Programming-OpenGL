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

    OGL_CALL(glGenBuffers(1, &transformations_buffer_handle));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, transformations_buffer_handle));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(float), nullptr, GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    auto [w, h] = frame_buffer_size();

    M_ = glm::mat4(1.0f);
    float aspect_ = (float)w / (float)h;
    float fov_ = glm::radians(45.0f);
    float near_ = 0.1f;
    float far_  = 20.0f;

    set_camera(new xe::Camera);
    camera()->perspective(fov_, aspect_, near_, far_);
    camera()->look_at(glm::vec3(2.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto pyramid = new xe::Mesh(6 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                                indexes.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);
        pyramid->load_vertices(0, vertices.size() * sizeof(float), &vertices[0]);
    
    pyramid->add_attribute(xe::AttributeType::POSITION, 3, GL_FLOAT, 0);
    pyramid->add_attribute(xe::AttributeType::COLOR_0, 3, GL_FLOAT, 3 * sizeof(float));

    pyramid->load_indices(0, indexes.size() * sizeof(GLubyte), &indexes[0]);
    pyramid->add_primitive(0, 18);

    add_mesh(pyramid);

    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));
    OGL_CALL(glViewport(0, 0, w, h));

    glEnable(GL_DEPTH_TEST);

    OGL_CALL(glUseProgram(program));
    glEnable(GL_CULL_FACE);

    set_controller(new xe::CameraController(camera()));
}


void SimpleShapeApplication::frame() {
    glm::mat4 P = camera()->projection();
    glm::mat4 V = camera()->view();

    glm::mat4 PVM(1.0f);
    PVM = P * V * M_;

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, transformations_buffer_handle));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &PVM[0]));

    for (auto m: meshes_)
        m->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
       Application::framebuffer_resize_callback(w, h);
       OGL_CALL(glViewport(0, 0, w, h));
       camera()->set_aspect((float)w / (float)h);
   }

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);   
    camera()->zoom(yoffset / 20.0f);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controller_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controller_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controller_->LMB_released(x, y);
    }
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controller_) {
        controller_->mouse_moved(x, y);
    }
}