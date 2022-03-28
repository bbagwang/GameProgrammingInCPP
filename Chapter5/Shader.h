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
	//주어진 이름으로 버텍스 / 프래그먼트 쉐이더 로드
	//쉐이더를 컴파일하고 서로 연결시킨다.
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	//이 쉐이더를 활성화된 쉐이더 프로그램으로 설정
	void SetActive();
	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
private:
	//아래의 세가지 함수는 Load 함수에서 사용하는 헬퍼 함수

	//지정된 쉐이더를 컴파일
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	
	//쉐이더가 성공적으로 컴파일됐는지 확인
	bool IsCompiled(GLuint shader);
	//버텍스 / 프래그먼트 프로그램이 연결됐는지 확인
	bool IsValidProgram();

private:
	//쉐이더 오브젝트 ID를 저장
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
