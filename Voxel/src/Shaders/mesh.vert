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

void main()
{
	vec3 finalPos = vec3(pos.x+positionOffset.x, pos.y+positionOffset.y, pos.z);

	fragColor = vec4(vec3(color.x, color.y, color.z), color.w);
	gl_Position = projection * view * vec4(finalPos, 1.0);
	shadowCoord = lightTransform * vec4(pos,1);
}