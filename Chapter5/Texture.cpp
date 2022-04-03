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
	
	//이미지 파일 로드
	unsigned char* image = SOIL_load_image(
		fileName.c_str(),	//파일 이름
		&mWidth,			//너비
		&mHeight,			//높이
		&channels,			//채널 수
		SOIL_LOAD_AUTO);	//이미지 파일 타입 (여기선 자동)
	
	if (image == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}
	
	//RGB 인지 RGBA 인지 체크
	//채널수 4개면 RGBA
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}
	
	//OpenGL 텍스처 오브젝트 생성
	glGenTextures(1, &mTextureID);
	
	//텍스처 활성화
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	
	//텍스처 오브젝트를 얻은 후 원본 이미지 데이터를
	//텍스처 오브젝트에 복사
	glTexImage2D(
		GL_TEXTURE_2D,		//텍스처 타깃
		0,					//LOD (Level of Detail)
		format,				//OpenGL이 사용해야 되는 색상 포맷
		mWidth,				//텍스처 너비
		mHeight,			//텍스처 높이
		0,					//보더 (Border)
		format,				//입력 데이터의 색상 포맷
		GL_UNSIGNED_BYTE,	//입력 데이터의 비트 깊이 (Unsigned Byte는 8비트 채널 지정)
		image);				//이미지 데이터의 포인터
	
	//이미지 복사 후 SOIL 메모리 상의 원본 이미지 데이터 해제
	SOIL_free_image_data(image);
	
	//이중 선형 필터링 활성화
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
