#version 300 es
precision highp float;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightDirection;

out vec4 fragColor;

void main()
{
	float scale = dot(normal, -lightDirection);
	if(scale<0.1)
	{
		scale=0.1;
	}
	fragColor = vec4(scale * vec3(color.x, color.y, color.z), color.w);
	gl_Position = projection * view * vec4(pos, 1.0);
}