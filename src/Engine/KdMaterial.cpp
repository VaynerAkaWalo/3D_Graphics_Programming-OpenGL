#include "KdMaterial.h"
#include "glm/glm.hpp"

namespace xe {
    void KdMaterial::init() {
        create_material_uniform_buffer(2*sizeof(glm::vec4));

        create_program_in_engine({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                           {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});

        auto map_Kd_location = glGetUniformLocation(program(),"map_Kd");
        if (map_Kd_location == -1) {
                SPDLOG_WARN("Cannot find map_Kd uniform");
            }
    
    }
    
    void KdMaterial::KdMaterial::bind() const { 
        glUseProgram(program()); 

        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer()));
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
        int vertexColorCastTo_INT;
        if (use_vertex_colors == false) { vertexColorCastTo_INT = 0;}
        else if (use_vertex_colors == true) { vertexColorCastTo_INT = 1;}
        
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &vertexColorCastTo_INT));

         int useMapKd_INT;
        if (texture_ > 0) { useMapKd_INT = 1;}
        else { useMapKd_INT = 0;}

        if (texture_ > 0) {
            OGL_CALL(glActiveTexture(GL_TEXTURE0));
            OGL_CALL(glBindTexture(GL_TEXTURE_2D, texture_));
        }
        
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) + sizeof(int), sizeof(int), &useMapKd_INT));
    }

    void KdMaterial::KdMaterial::unbind() const {
        OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }
}