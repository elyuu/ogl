#version 330 core

in vec3 vert_color;
in vec2 tex_coord0;

out vec4 frag_color;

uniform sampler2D texture1;

void main()
{
	frag_color = texture(texture1, tex_coord0);
}
