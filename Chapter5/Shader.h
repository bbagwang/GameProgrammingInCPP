// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	//�־��� �̸����� ���ؽ� / �����׸�Ʈ ���̴� �ε�
	//���̴��� �������ϰ� ���� �����Ų��.
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//�� ���̴��� Ȱ��ȭ�� ���̴� ���α׷����� ����
	void SetActive();
	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	//�Ʒ��� ������ �Լ��� Load �Լ����� ����ϴ� ���� �Լ�

	//������ ���̴��� ������
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	
	//���̴��� ���������� �����ϵƴ��� Ȯ��
	bool IsCompiled(GLuint shader);
	//���ؽ� / �����׸�Ʈ ���α׷��� ����ƴ��� Ȯ��
	bool IsValidProgram();

private:
	//���̴� ������Ʈ ID�� ����
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
