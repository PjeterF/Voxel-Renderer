#version 300 es
precision highp float;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightDirection;
uniform mat4 lightTransform;

out vec4 fragColor;
out vec4 shadowCoord;

void main()
{
	float colorScale = dot(normal, -lightDirection);

	if(colorScale<0.0)
	{
		colorScale=0.0;
	}

	colorScale = 0.8;

	fragColor = vec4(colorScale * vec3(color.x, color.y, color.z), color.w);
	gl_Position = projection * view * vec4(pos, 1.0);
	shadowCoord = lightTransform * vec4(pos,1);
}