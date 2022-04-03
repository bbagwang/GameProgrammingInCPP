// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Texture.h"
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

Texture::Texture()
:mTextureID(0)
,mWidth(0)
,mHeight(0)
{
	
}

Texture::~Texture()
{
	
}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;
	
	//�̹��� ���� �ε�
	unsigned char* image = SOIL_load_image(
		fileName.c_str(),	//���� �̸�
		&mWidth,			//�ʺ�
		&mHeight,			//����
		&channels,			//ä�� ��
		SOIL_LOAD_AUTO);	//�̹��� ���� Ÿ�� (���⼱ �ڵ�)
	
	if (image == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}
	
	//RGB ���� RGBA ���� üũ
	//ä�μ� 4���� RGBA
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}
	
	//OpenGL �ؽ�ó ������Ʈ ����
	glGenTextures(1, &mTextureID);
	
	//�ؽ�ó Ȱ��ȭ
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	
	//�ؽ�ó ������Ʈ�� ���� �� ���� �̹��� �����͸�
	//�ؽ�ó ������Ʈ�� ����
	glTexImage2D(
		GL_TEXTURE_2D,		//�ؽ�ó Ÿ��
		0,					//LOD (Level of Detail)
		format,				//OpenGL�� ����ؾ� �Ǵ� ���� ����
		mWidth,				//�ؽ�ó �ʺ�
		mHeight,			//�ؽ�ó ����
		0,					//���� (Border)
		format,				//�Է� �������� ���� ����
		GL_UNSIGNED_BYTE,	//�Է� �������� ��Ʈ ���� (Unsigned Byte�� 8��Ʈ ä�� ����)
		image);				//�̹��� �������� ������
	
	//�̹��� ���� �� SOIL �޸� ���� ���� �̹��� ������ ����
	SOIL_free_image_data(image);
	
	//���� ���� ���͸� Ȱ��ȭ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
