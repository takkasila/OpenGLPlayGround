#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;

void main()
{

	// Will be uniform soon
	//Light
	vec3 LightColor = vec3(1, 1, 1);
	float LightPower = 50f;
	//Material
	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3) ;

	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);

	float cosTheta = clamp( dot( n, l), 0, 1);

	// specular
	vec3 E = normalize(EyeDirection_cameraspace);
	//reflect 
	vec3 R = reflect(-l, n);
	float cosAlpha = clamp(dot(R, E), 0, 1);


	float distance = length(LightPosition_worldspace - Position_worldspace);
	color = 
		//Diffuse
		MaterialDiffuseColor * LightColor * LightPower * cosTheta/(distance * distance)
		//Ambient
		+ MaterialAmbientColor
		//Specular
		+ MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 20)/(distance*distance)
	;

}