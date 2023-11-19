//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>
#include "Application/application.h"
#include "camera.h"
#include "glm/glm.hpp"
#include "glad/gl.h"

#include "Application/application.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) :
    Application(width, height, title, debug), camera_(nullptr) {}
    void framebuffer_resize_callback(int w, int h) override;

    void init() override;

    void frame() override;

    void set_camera(Camera *camera) { camera_ = camera; }

    void scroll_callback(double xoffset, double yoffset) override;
   
    Camera *camera() const {
        assert(camera_);
        return camera_;
    }

private:
    GLuint vao_;
    glm::mat4 M_;
    Camera *camera_;

    GLuint transformations_buffer_handle;

};