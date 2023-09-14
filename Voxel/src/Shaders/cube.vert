#version 300 es
precision highp float;

layout(location = 0) in vec3 inPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

out vec4 fragColor;

void main()
{
	fragColor = color;
	gl_Position = projection * view * model * vec4(inPos.x, inPos.y, inPos.z, 1.0);
}