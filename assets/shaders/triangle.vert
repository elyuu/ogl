#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texcoord0;

out vec3 vert_color;
out vec2 tex_coord0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 o = projection * view * model * vec4(in_pos, 1.0);
	gl_Position = o;
	vert_color = in_color;
	tex_coord0 = in_texcoord0;
}
