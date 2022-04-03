// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//���� ��ȯ ��Ŀ� ���� uniform ����
uniform mat4 uWorldTransform;
//��-���� ��ȯ ��Ŀ� ���� uniform ����
uniform mat4 uViewProj;

//�Ӽ� 0�� ���ؽ� ��ġ ����(XYZ)
//�Ӽ� 1�� �ؽ�ó ��ġ ����(UV)
//�Ӽ� 2�� ���ؽ� ���� ����(RGB)
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inVertColor;

//�Էµ� �ؽ�ó ��ġ�� ��� ������ ������ ���� ����
out vec2 fragTexCoord;

//�Էµ� ���ؽ� ���� ������ ��� ������ ������ ���� ����
out vec4 vertColor;

void main()
{
	//��ġ�� ���� ��ǥ�� ��ȯ
	vec4 pos = vec4(inPosition, 1.0);
	//��ġ�� ���� �������� ��ȯ�ѵ� Ŭ�� �������� ��ȯ
	gl_Position = pos * uWorldTransform * uViewProj;

	//�ؽ�ó �Է� ������ ��� �ؽ�ó ������ ����
	fragTexCoord = inTexCoord;

	//���ؽ� �÷� �Է� ������ ��� ���ؽ� �÷� ������ ����
	vertColor = inVertColor;	
}
