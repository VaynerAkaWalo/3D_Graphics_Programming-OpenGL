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
#include "CameraController.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) :
    Application(width, height, title, debug), camera_(nullptr) {}
    void framebuffer_resize_callback(int w, int h) override;

    void init() override;

    void frame() override;

    void set_camera(Camera *camera) { camera_ = camera; }

    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;
   
    Camera *camera() const {
        assert(camera_);
        return camera_;
    }

    void set_controller(CameraController *controller) { controller_ = controller; }

private:
    GLuint vao_;
    glm::mat4 M_;
    Camera *camera_;
    CameraController *controller_;

    GLuint transformations_buffer_handle;

};