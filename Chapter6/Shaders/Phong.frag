// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

// Create a struct for directional light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;
};

struct PointlLight
{
	// ºû À§Ä¡
	vec3 mPosition;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;

	float mConstant;
	float mLinear;
	float mQuadratic;
};

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Specular power for this surface
uniform float uSpecPower;
// Ambient light level
uniform vec3 uAmbientLight;

// Directional Light
uniform DirectionalLight uDirLight;

//Point Lights
#define POINT_LIGHT_COUNT 4
uniform PointlLight uPointLights[POINT_LIGHT_COUNT];

vec4 CalculateDirectionalLight();
vec4 CalculatePointLight(int index);

void main()
{
    outColor = CalculateDirectionalLight();
	/*for(int i = 0 ; i < POINT_LIGHT_COUNT ; i++)
	{
		outColor += CalculatePointLight(i);
	}*/
}

vec4 CalculateDirectionalLight()
{
	// Surface normal
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos-fragWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	// Final color is texture color times phong light (alpha = 1)
    return texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}

vec4 CalculatePointLight(int index)
{
	// Surface normal
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	vec3 L = normalize(uPointLights[index].mPosition - fragWorldPos);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos-fragWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));
	
	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{

		//°¨¼â °è»ê
		float distance    = length(uPointLights[index].mPosition - fragWorldPos);
		float Attenuation = 1.0 / (uPointLights[index].mConstant + uPointLights[index].mLinear * distance + 
  		uPointLights[index].mQuadratic * (distance * distance));

		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += (Diffuse + Specular) * Attenuation;
	}

	 return texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}