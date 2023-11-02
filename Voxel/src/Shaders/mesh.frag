#version 300 es
precision highp float;

in vec4 fragColor;
in vec4 shadowCoord;

out vec4 finalColor;

in vec3 fragPos;
in vec3 norm;

uniform sampler2D shadowMap;

uniform vec3 lightDirection;
uniform vec3 cameraPosition;

void main()
{
	

	float bias = 0.001;
	float visibility = 1.0;
	vec2 texelSize = (1.0 / float(textureSize(shadowMap, 0))) * vec2(1.0, 1);
	if ( texture(shadowMap, shadowCoord.xy).z < shadowCoord.z - bias )
	{
		float shadow = 0.0;
		int distance = 2;
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

		finalColor = vec4(visibility * (vec3(fragColor)), 1);
	}
	else
	{
		vec3 finalNormal = normalize(norm);
		vec3 reflected = normalize(lightDirection - 2.0f * (lightDirection * finalNormal) * finalNormal);
		vec3 viewVec = normalize(cameraPosition - fragPos);
		float specularScale = 0.08f * pow(max(dot(reflected, viewVec), 0.0f), 100.0f);
		vec3 specularComponent = specularScale * vec3(1.0f, 1.0f, 1.0f);

		finalColor = vec4(visibility * (specularComponent + vec3(fragColor)), 1);
	}

	
}