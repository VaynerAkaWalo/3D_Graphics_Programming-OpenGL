#version 410

in vec4 vertex_color;

layout(location=0) out vec4 vFragColor;

layout(std140) uniform Mixer {
    float strength;
    vec3  mix_color;
};

void main() {
    vFragColor = vertex_color;
}
