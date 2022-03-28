// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//GLSL 3.3 ��û
#version 330

// This should correspond to the data stored
// for each vertex in the vertex buffer.
// For now, just a position.
//��� ���ؽ� �Ӽ��� ���⿡
//���� ���ؽ� �Ӽ��� ��ġ �ϳ�����
//�Է� �����̹Ƿ� �տ� in Ű���尡 ��.
in vec3 inPosition;

void main()
{
	// The vertex shader needs to output a 4D
	// coordinate.
	// For now set the 4th coordinate to 1.0
	//inPosition�� gl_Position�� ���� ����
	gl_Position = vec4(inPosition, 1.0);
}
