//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include "Application/application.h"

#include "glm/glm.hpp"
#include "glad/gl.h"

#include "Application/application.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title,
                                                                                               debug) {}
    void framebuffer_resize_callback(int w, int h) override;

    void init() override;

    void frame() override;

private:
    GLuint vao_;
    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 P_;
    glm::mat4 V_;
    glm::mat4 M_;

    GLuint transformations_buffer_handle;

};