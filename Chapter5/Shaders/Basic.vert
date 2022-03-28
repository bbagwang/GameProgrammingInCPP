// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//GLSL 3.3 요청
#version 330

// This should correspond to the data stored
// for each vertex in the vertex buffer.
// For now, just a position.
//모든 버텍스 속성은 여기에
//현재 버텍스 속성은 위치 하나뿐임
//입력 변수이므로 앞에 in 키워드가 들어감.
in vec3 inPosition;

void main()
{
	// The vertex shader needs to output a 4D
	// coordinate.
	// For now set the 4th coordinate to 1.0
	//inPosition을 gl_Position로 직접 전달
	gl_Position = vec4(inPosition, 1.0);
}
