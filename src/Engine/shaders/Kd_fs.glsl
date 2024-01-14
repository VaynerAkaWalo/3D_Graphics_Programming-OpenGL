#version 420

in vec4 vertex_color;
in vec2 vertex_texcoord_0;

layout(location=0) out vec4 vFragColor;

layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd; 
};

uniform sampler2D map_Kd;

vec3 srgb_gamma_correction(vec3 color) {
    color = clamp(color, 0.0, 1.0);
    color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
    return color;
}

void main() {
    vFragColor = Kd;

    if (use_vertex_color == false) {vFragColor *= Kd;}

    if (use_map_Kd) {
        vec4 texture_color = texture(map_Kd, vertex_texcoord_0); 
        vFragColor *= texture_color;
    }

    vFragColor.a = vFragColor.a;
    vFragColor.rgb = srgb_gamma_correction(vFragColor.rgb);
}
