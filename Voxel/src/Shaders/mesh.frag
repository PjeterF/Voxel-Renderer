#version 300 es
precision highp float;

in vec4 fragColor;
in vec4 shadowCoord;

uniform sampler2D shadowMap;

out vec4 finalColor;

void main()
{
	float bias = 0.005;
	float visibility = 1.0;
	if ( texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - bias)
	{
		visibility = 0.4;
	}

	finalColor = vec4(visibility * vec3(fragColor), 1);
	//finalColor = vec4(vec3(gl_FragCoord.z), 1);
}