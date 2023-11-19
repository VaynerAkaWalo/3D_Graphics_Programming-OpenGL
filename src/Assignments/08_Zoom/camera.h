#pragma once
   #include <cmath>
   #include "glm/glm.hpp"
   #include "glm/gtc/matrix_transform.hpp"
   
   namespace {
    float logistic(float y) { return 1.0f/(1.0f+std::exp(-y)); }

    float logit(float x) { return std::log(x/(1.0f-x)); }
}


   class Camera {
   public:

     void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
         V_ = glm::lookAt(eye, center, up);
     }
   
     void perspective(float fov, float aspect, float near, float far) {
         fov_ = fov;
         aspect_ = aspect;
         near_ = near;
         far_ = far;
     }
   
     void set_aspect(float aspect) {
         aspect_ = aspect;
     }

     void zoom(float y_offset) {
        auto x = fov_/glm::pi<float>();
        auto y = logit(x);
        y+= y_offset;
        x = logistic(y);
        fov_ = x*glm::pi<float>();
    }
   
     glm::mat4 view() const { return V_; }
   
     glm::mat4 projection() const { return glm::perspective(fov_, aspect_, near_, far_); }
     
   private:
     float fov_;
     float aspect_;
     float near_;
     float far_;
   
     glm::mat4 V_;
   };