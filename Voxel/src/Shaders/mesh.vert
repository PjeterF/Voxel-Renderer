#version 300 es
precision highp float;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

uniform mat4 projection;
uniform mat4 view;

out vec4 fragColor;

void main()
{
	fragColor = color;
	gl_Position = projection * view * vec4(pos, 1.0);
}