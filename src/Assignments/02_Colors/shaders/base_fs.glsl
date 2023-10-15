#version 410

in vec4 vertex_color;

layout(location=0) out vec4 vFragColor;

void main() {
    vFragColor = vertex_color;
}
