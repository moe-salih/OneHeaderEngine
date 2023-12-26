#version 450 core

layout (location = 0) in vec4 a_Pos;

uniform mat4 u_view_projection;


void main() {
	gl_Position = u_view_projection * vec4(a_Pos.xyz, 1.0);
}