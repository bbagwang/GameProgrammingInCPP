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
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

//�Էµ� �ؽ�ó ��ġ�� ��� ������ ������ ���� ����
out vec2 fragTexCoord;

void main()
{
	//��ġ�� ���� ��ǥ�� ��ȯ
	vec4 pos = vec4(inPosition, 1.0);
	//��ġ�� ���� �������� ��ȯ�ѵ� Ŭ�� �������� ��ȯ
	gl_Position = pos * uWorldTransform * uViewProj;

	//���ؽ� �Է� ������ ��� ������ �ؽ�ó ��ǥ ����
	fragTexCoord = inTexCoord;
}
