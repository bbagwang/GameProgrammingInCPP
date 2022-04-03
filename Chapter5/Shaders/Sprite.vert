// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//세계 변환 행렬에 대한 uniform 변수
uniform mat4 uWorldTransform;
//뷰-투영 변환 행렬에 대한 uniform 변수
uniform mat4 uViewProj;

//속성 0은 버텍스 위치 정보(XYZ)
//속성 1은 텍스처 위치 정보(UV)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

//입력된 텍스처 위치를 출력 변수로 보내기 위한 선언
out vec2 fragTexCoord;

void main()
{
	//위치를 동차 좌표로 변환
	vec4 pos = vec4(inPosition, 1.0);
	//위치를 세계 공간으로 변환한뒤 클립 공간으로 변환
	gl_Position = pos * uWorldTransform * uViewProj;

	//버텍스 입력 변수를 출력 변수로 텍스처 좌표 복사
	fragTexCoord = inTexCoord;
}
