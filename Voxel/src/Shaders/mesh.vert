#version 300 es
precision highp float;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightTransform;
uniform vec2 positionOffset;

out vec4 fragColor;
out vec4 shadowCoord;
out vec3 fragPos;
out vec3 norm;

void main()
{
	fragColor = vec4(vec3(color.x, color.y, color.z), color.w);

	shadowCoord = lightTransform * vec4(pos,1);

	gl_Position = projection * view * vec4(pos, 1.0);
	fragPos = pos;
	norm = normal;
}