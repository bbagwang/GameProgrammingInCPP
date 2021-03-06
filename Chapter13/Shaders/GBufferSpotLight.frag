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

// This corresponds to the output color to the color buffer
layout(location = 0) out vec4 outColor;

// Different textures from G-buffer
uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uGSpecPower;

// Create a struct for the point light
struct SpotLight
{
	// Position of light
	vec3 mWorldPos;
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	//Specular Color
	vec3 mSpecularColor;
	// Radius of the light
	float mInnerRadius;
	float mOuterRadius;
};

uniform SpotLight uSpotLight;
// Stores width/height of screen
uniform vec2 uScreenDimensions;

void main()
{
	// From this fragment, calculate the coordinate to sample into the G-buffer
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;
	
	// Sample from G-buffer
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
	float gbufferSpecPower = texture(uGSpecPower, gbufferCoord).x;
	
	// Surface normal
	vec3 N = normalize(gbufferNorm);
	// Vector from surface to light
	vec3 L = normalize(uSpotLight.mWorldPos - gbufferWorldPos);

	// Compute Phong diffuse component for the light
	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		// Get the distance between the light and the world pos
		float dist = distance(uSpotLight.mWorldPos, gbufferWorldPos);
		// Use smoothstep to compute value in range [0,1]
		// between inner/outer radius
		float intensity = smoothstep(uSpotLight.mInnerRadius,
									 uSpotLight.mOuterRadius, dist);
		// The diffuse color of the light depends on intensity
		vec3 DiffuseColor = mix(uSpotLight.mDiffuseColor,
								vec3(0.0, 0.0, 0.0), intensity);

		vec3 SpecularColor = mix(uSpotLight.mSpecularColor,
								vec3(0.0, 0.0, 0.0), intensity);

		Phong = DiffuseColor * NdotL + SpecularColor;
	}

	// Final color is texture color times phong light (alpha = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
