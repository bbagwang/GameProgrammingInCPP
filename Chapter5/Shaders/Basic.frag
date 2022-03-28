// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//GLSL 3.3 ��û
#version 330

// This corresponds to the output color
// to the color buffer
//RGBA ���� ������ 4�� ��ҿ� �ش��ϴ� vec4 Ÿ���� ����
//��� �����̹Ƿ� �տ� out Ű���尡 ��.
//���������� ���� ������ outColor�� ����. (���� ���۷� ��µǴ� �÷�)
out vec4 outColor;

void main()
{
    //�Ķ������� ����
    outColor = vec4(0.0, 0.0, 1.0, 1.0);
}
