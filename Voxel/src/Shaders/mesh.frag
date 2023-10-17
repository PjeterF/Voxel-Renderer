#version 300 es
precision highp float;

in vec4 fragColor;
in vec4 shadowCoord;

uniform sampler2D shadowMap;

out vec4 finalColor;

void main()
{
	float bias = 0.002;
	float visibility = 1.0;
	vec2 texelSize = (1.0 / float(textureSize(shadowMap, 0))) * vec2(1.0, 1);
	if ( texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - bias )
	{
		float shadow = 0.0;
		int distance = 5;
		for(int x = -distance ; x <= distance ; x++)
		{
			for(int y = -distance ; y <= distance ; y++)
			{
				if(texture(shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).z < shadowCoord.z - bias)
				{
					shadow = shadow + 1.0;
				}
			}
		}
		visibility = 1.0 - shadow / float(distance*distance)/8.0;
	}

	finalColor = vec4(visibility * vec3(fragColor), 1);
}