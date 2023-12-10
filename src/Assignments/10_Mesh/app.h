//
// Created by pbialas on 05.08.2020.
//

#pragma once
#include <vector>
#include "Application/application.h"
#include "Engine/camera.h"
#include "glm/glm.hpp"
#include "glad/gl.h"
#include "Engine/Mesh.h"

#include "Application/application.h"
#include "Engine/CameraController.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) :
    Application(width, height, title, debug), camera_(nullptr) {}
    void framebuffer_resize_callback(int w, int h) override;

    void init() override;

    void frame() override;

    void set_camera(xe::Camera *camera) { camera_ = camera; }

    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void add_mesh(xe::Mesh *mesh) {
       meshes_.push_back(mesh);
   }


   
    xe::Camera *camera() const {
        assert(camera_);
        return camera_;
    }

    void set_controller(xe::CameraController *controller) { controller_ = controller; }

private:
    GLuint vao_;
    glm::mat4 M_;
    xe::Camera *camera_;
    xe::CameraController *controller_;

    std::vector<xe::Mesh*> meshes_; 

    GLuint transformations_buffer_handle;

};