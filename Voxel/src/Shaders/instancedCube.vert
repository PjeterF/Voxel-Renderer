#version 300 es
precision highp float;

layout(location = 0) in vec3 modelPos;
layout(location = 1) in vec3 positionOffset;
layout(location = 2) in float size;
layout(location = 3) in vec4 color;

uniform mat4 projection;
uniform mat4 view;

out vec4 fragColor;

void main()
{
	fragColor = color;
	gl_Position = projection * view * vec4(size*modelPos+positionOffset, 1.0);
}