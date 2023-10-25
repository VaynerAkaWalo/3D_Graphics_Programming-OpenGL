#version 410

in vec4 vertex_color;

layout(location=0) out vec4 vFragColor;

layout(std140) uniform Mixer {
    float strength;
    vec3  mix_color;
};

out vec4 fragColor;

void main() {
    vFragColor.rgb = mix(vertex_color.rgb, mix_color, strength);;
}
