// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	,mNumIndices(numIndices)
{
	//���ؽ� �迭 ��ü ����
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	//���ؽ� ���� ����
	glGenBuffers(1, &mVertexBuffer);
	//glBindBuffer�� ù ��° ������ GL_ARRAY_BUFFER �Ķ���ʹ� ���ؽ� ���۸� ���۷� ����ϰڴٴ� ��.
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	//���ؽ� ���� ����������, VertexArray �����ڷ� ���޵� ���ؽ� �����͸� �� ���ؽ� ���۷� �����Ѵ�.
	//GL_ARRAY_BUFFER �� ���� �� ������ ���ؽ� ���۸� ����ϰڴٴ� ���� ����.
	glBufferData(
		GL_ARRAY_BUFFER,				//�����͸� �� ������ ���� Ÿ��
		numVerts * 9 * sizeof(float),	//������ ����Ʈ ũ�� (�� ���ؽ��� ������ ũ�⿡ ���ؽ� ������ ���� ��)
		verts,							//������ �ҽ�(������)
		GL_STATIC_DRAW);				//�� �����͸� ��� ����� ���ΰ�?

	//GL_STATIC_DRAW �� �����͸� ���� �� ���� �ε��ϸ�, ���ؽ��� ���� �׷����� ��쿡 ���Ǵ� �ɼ�.

	//�ε��� ���� ����
	glGenBuffers(1, &mIndexBuffer);
	//glBindBUffer�� ù ��° ������ GL_ELEMENT_ARRAY_BUFFER �Ķ���ʹ� �ε��� ���ۿ� �ش��ϴ� Ÿ��.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	//�ε��� ���� ���� �� �ε��� �����͸� �ε��� ���ۿ� ����.
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			//�ε��� ����
		numIndices * sizeof(unsigned int),	//�������� ũ�� (�ε��� ���� unsigned int ũ�� ���� ��) [unsinged int�� �ε����� ���]
		indices,
		GL_STATIC_DRAW);

	//ù��° ���ؽ� �Ӽ�(0�� �ε���) Ȱ��ȭ�� ���� glEnableVertexAttribArray ���
	// (For now, assume one vertex format)
	// ��ġ�� ���ؽ� �Ӽ� 0�̸� 3���� ���(x, y, z)�� �����Ƿ� ó�� �� �Ķ���ʹ� 0�� 3�̴�.
	glEnableVertexAttribArray(0);
	//ũ��, Ÿ��, �Ӽ��� ������ �����ϱ� ���� glVertexAttribPointer ���
	glVertexAttribPointer(
		0,					//�Ӽ� �ε��� (ù ��° ���ؽ� �Ӽ� �ε����� 0)
		3,					//����� �� (�̹� �������� 3)
		GL_FLOAT,			//����� Ÿ��
		GL_FALSE,			//(������ Ÿ�Կ����� ���ȴ�.)
		sizeof(float) * 9,	//���� (�Ϲ������� �� ���ؽ��� ũ��) [���ؽ� ���ۿ��� �е��� ���ٸ� ������ ���ؽ��� ũ�Ⱑ ��]
		0);					//���ؽ��� ���ۿ��� �� �Ӽ������� ������ (0�� ��ġ �Ӽ��� ���ؽ��� ���� ��ġ�� �����ϱ� ����)
	//�߰� �Ӽ��� ���ؼ��� �����¿� 0�� �ƴ� ���� �����ؾ���.

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,											//���ؽ� �Ӽ� �ε���
		2,											//������Ʈ�� �� (UV, 2)
		GL_FLOAT,									//�� ������Ʈ�� Ÿ��
		GL_FALSE,									//GL_FLOAT ������ ��� ���� ����
		sizeof(float) * 9,							//���� (������ �׻� ���ؽ��� ũ��)
		reinterpret_cast<void*>(sizeof(float) * 3));//������ ������

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,											//���ؽ� �Ӽ� �ε���
		4,											//������Ʈ�� �� (RGBA, 4)
		GL_FLOAT,									//�� ������Ʈ�� Ÿ��
		GL_FALSE,									//GL_FLOAT ������ ��� ���� ����
		sizeof(float) * 9,							//���� (������ �׻� ���ؽ��� ũ��)
		reinterpret_cast<void*>(sizeof(float) * 5));//������ ������
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
