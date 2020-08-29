#version 330 core

in vec2 a_vertPos;
in vec2 a_texCoord;

out vec2 v_texCoord;

uniform mat4 u_viewMatrix;
uniform int u_texWidth, u_texHeight;

void main () {
  v_texCoord = a_texCoord / vec2(u_texWidth, u_texHeight); // varying, normalize & pass to fragment shader
  gl_Position = u_viewMatrix * vec4(a_vertPos, 0.0, 1.0);
}
