#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 PixelPos;
in vec2 TexCoords;
in vec4 PixelPosLightSpace;

uniform sampler2D occlusionMap;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float bias;

void main()
{       
    vec3 surfaceColor = texture(occlusionMap, TexCoords).rgb;

    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(1.0f);
	vec3 lightDirection = normalize(lightPos - PixelPos);

    // ambient
    vec3 ambient = 0.3 * surfaceColor;

	//diffuse
    vec3 diffuse = clamp(dot(normal, lightDirection), 0.0, 1.0) * lightColor;

    // specular
    vec3 halfWayDir = normalize(lightDirection);  
    vec3 specular = pow(max(dot(normal, halfWayDir), 0.0), 256.0) * lightColor;
    specular *= 0.1;    

	//attenuation
	float intensity = 0.01f;
	//get distance from light to the surface it will shine on and calculate attenuation
	float distanceToLight = length(lightPos - PixelPos);
	float attenuation = 1.0 / (1.0 + intensity * pow(distanceToLight, 2));

	//*-----------Calculate Shadow -------------------*/
	float shadowBias = bias;
	//get the projected coordinate of the pixel by perspective divide from the light's POV
    vec3 projectedCoords = PixelPosLightSpace.xyz / PixelPosLightSpace.w;
    //transform the coords to [0,1] since texture coords are in [0,1]
    projectedCoords = projectedCoords * 0.5 + 0.5;
    //get the depth value of the pixel in the shadow map
    float depthValue = texture(shadowMap, projectedCoords.xy).r; 
    // check whether current frag pos is in shadow
    float shadow = projectedCoords.z - 0.05;

	if(shadow > depthValue)
	{
		shadow = 1.0f;
	}
	else 
	{
		shadow = 0.0f;
	}
    
	//final lighting 
	vec3 lighting = ambient + (1.0 - shadow)*(diffuse + specular) * surfaceColor;
    FragColor = vec4(normal, 1.0);
}