// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

//���ؽ� ���̴����� �Ѿ�� �ؽ�ó ��ġ
in vec2 fragTexCoord;

//���ؽ� ���̴����� ��µǾ� �Ѿ�� ���ؽ� �÷�
in vec4 vertColor;

//���� �ȼ� �÷�
out vec4 outColor;

//�ؽ�ó ��ǥ�� ������ ��� ���� ���÷�
uniform sampler2D uTexture;

void main()
{
	//�ؽ�ó�κ��� ������ ���ø� + ���ؽ� �÷� ����
	outColor = texture(uTexture, fragTexCoord) * vertColor;
}
