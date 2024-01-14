#version 420

layout(location = 0) in vec4 a_vertex_position;

layout(location = 3) in vec2 a_vertex_texcoord_0;

layout(location = 5) in vec4 a_vertex_color;

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

out vec2 vertex_texcoord_0;
out vec4 vertex_color;

void main() {
    gl_Position =  PVM * a_vertex_position;
    
    vertex_color = a_vertex_color;
    vertex_texcoord_0 = a_vertex_texcoord_0;
}
