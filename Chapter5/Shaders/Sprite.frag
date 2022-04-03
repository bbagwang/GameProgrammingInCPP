// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//버텍스 쉐이더에서 넘어온 텍스처 위치
in vec2 fragTexCoord;

//버텍스 쉐이더에서 출력되어 넘어온 버텍스 컬러
in vec4 vertColor;

//최종 픽셀 컬러
out vec4 outColor;

//텍스처 좌표로 색상을 얻기 위한 샘플러
uniform sampler2D uTexture;

void main()
{
	//텍스처로부터 색상을 샘플링 + 버텍스 컬러 배합
	outColor = texture(uTexture, fragTexCoord) * vertColor;
}
