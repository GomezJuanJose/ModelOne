#type vertex

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec3 a_Normal;
layout(location = 1) in vec2 a_Texcoord;
		
uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;

uniform mat4 u_LightSpaceMatrix; // For shadow map

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_Texcoord;

out vec3 v_FragmentPosition;

out vec4 v_FragPosLightSpace; // For shadow map

void main(){
	v_Position = a_Position;
	v_Normal = u_NormalMatrix * a_Normal;
	v_Texcoord = a_Texcoord;

	v_FragmentPosition = vec3(u_ModelMatrix * vec4(v_Position, 1.0));

	v_FragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragmentPosition, 1.0);

	gl_Position = u_ProjectionViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 finalColor;
		
in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_Texcoord;

in vec3 v_FragmentPosition;

in vec4 v_FragPosLightSpace;

uniform sampler2D u_ShadowMap;

uniform vec3 u_AmbientLightColor;
uniform float u_AmbientLightIntensity;

uniform vec3 u_DiffuseLightDirection;
uniform vec3 u_DiffuseLightColor;
uniform float u_DiffuseLightIntensity;

uniform float u_Gamma;


float ShadowCalculation(vec4 fragPosLightSpace){
	//Perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
				
	//Transforms NDC to range [0,1]
	projCoords = projCoords * 0.5 + 0.5;


	//Avoids oversampling fustrum side
	if (projCoords.z > 1.0){
		return 0.0;
	}



	float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	//Prevents shadow acne
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(u_DiffuseLightDirection);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}


void main(){

	//Ambient light
	vec3 ambientLight = vec3(1.0) * vec3(u_AmbientLightColor) * u_AmbientLightIntensity;
			
	//Directional light
	vec3 normal = normalize(v_Normal);

	/*this is more for a point light like ilumination*/
	//vec3 lightDirection = normalize(u_DiffuseLightDirection - v_FragmentPosition);
	//float diffuse = max(dot(normal, lightDirection), 0.0);

	vec3 lightDirection = normalize(u_DiffuseLightDirection);
	float diffuse = max(dot(normal, lightDirection), 0.0);
	vec3 diffuseLight = diffuse * u_DiffuseLightColor * u_DiffuseLightIntensity;
			
	//Shadows
	float shadow = ShadowCalculation(v_FragPosLightSpace);

	vec4 fragmentColor = vec4(0.0, 0.9, 0.3, 1.0) * vec4(ambientLight + (1.0 - shadow) * (diffuseLight), 1.0);
	finalColor = vec4(pow(fragmentColor.rgb, vec3(1.0/u_Gamma)), 1.0);
	//color = vec4(v_Texcoord, 0.0, 1.0);
}