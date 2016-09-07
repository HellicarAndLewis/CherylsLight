#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform float radius;
uniform vec2 size;
uniform sampler2DRect tex0;

void main() {
    vec2 pos = gl_TexCoord[0].xy;
    float distsqrd = (pos.x - size.x/2) * (pos.x - size.x/2) + (pos.y - size.y/2) * (pos.y - size.y/2);
    float radsqrd = radius * radius;
    float diff = radsqrd - distsqrd;
    float a = min(diff, 1.0);
    a = max(diff, 0.0);
	gl_FragColor = texture2DRect(tex0, pos);
	gl_FragColor.a = a;
}