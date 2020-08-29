#version 330 core

in vec2 v_texCoord;
out vec4 f_fragColor;

uniform sampler2D u_tex;

void main () {
   f_fragColor = texture(u_tex, v_texCoord.st);
//  f_fragColor = vec4(0, 1, 0, 1);
}
