//
// Created by pbialas on 25.09.2020.
//
#include "Engine/KdMaterial.h"

#include "app.h"
#include <vector>
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb/stb_image.h"

void SimpleShapeApplication::init() {

    xe::KdMaterial::init();
    
    //        coordinates       |       colors
    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        -0.5f, 0.5f, 0.0f, // Base top left
        0.8090f, 0.5f,
        0.5f, 0.5f, 0.0f, // Base top right
        0.5f, 0.8090f,
        0.5f, -0.5f, 0.0f,  // Base bottom right
        0.1910f, 0.5f,
        -0.5f, -0.5f, 0.0f, // Base bottom left
        0.5f, 0.1910f,

        0.0f, 0.0f, 1.0f, // Apex R
        0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, // Apex Y
        1.0f, 0.0f,

        0.0f, 0.0f, 1.0f, // Apex B
        1.0f, 1.0f,

        0.0f, 0.0f, 1.0f, // Apex G
        0.0f, 0.0f,
    };

    std::vector<GLubyte> indexes = {
        0, 1, 2, // Base top
        2, 3, 0, // Base bottom

        2, 1, 4, // R
        0, 3, 5, // G
        1, 0, 6, // B
        3, 2, 7, // Y
    };

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

    stbi_set_flip_vertically_on_load(true);
    
    GLint width, height, channels;

    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    
    if (!img) {
        std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
        std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }
    
    GLuint tex_handle;
    OGL_CALL(glGenTextures(1, &tex_handle));
    OGL_CALL(glBindTexture(GL_TEXTURE_2D, tex_handle));
    OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    auto pyramid = new xe::Mesh(5 * sizeof(float), vertices.size() * sizeof(float), GL_STATIC_DRAW,
                                indexes.size() * sizeof(GLubyte), GL_UNSIGNED_BYTE, GL_STATIC_DRAW);
        pyramid->load_vertices(0, vertices.size() * sizeof(float), &vertices[0]);
    
    pyramid->add_attribute(xe::AttributeType::POSITION, 3, GL_FLOAT, 0);
    pyramid->add_attribute(xe::AttributeType::TEXCOORD_0, 2, GL_FLOAT, 3 * sizeof(float));

    pyramid->load_indices(0, indexes.size() * sizeof(GLubyte), &indexes[0]);

    pyramid->add_primitive(0, 18, new xe::KdMaterial({1.f, 1.f, 1.0f, 1.0f}, false, tex_handle));

    add_mesh(pyramid);

    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));
    OGL_CALL(glViewport(0, 0, w, h));

    glEnable(GL_DEPTH_TEST);
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