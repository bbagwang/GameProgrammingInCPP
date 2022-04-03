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
	//버텍스 배열 개체 생성
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	//버텍스 버퍼 생성
	glGenBuffers(1, &mVertexBuffer);
	//glBindBuffer의 첫 번째 인자인 GL_ARRAY_BUFFER 파라미터는 버텍스 버퍼를 버퍼로 사용하겠다는 뜻.
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	//버텍스 버퍼 생성했으면, VertexArray 생성자로 전달된 버텍스 데이터를 이 버텍스 버퍼로 복사한다.
	//GL_ARRAY_BUFFER 는 지금 막 생성한 버텍스 버퍼를 사용하겠다는 것을 뜻함.
	glBufferData(
		GL_ARRAY_BUFFER,				//데이터를 쓸 버퍼의 버퍼 타입
		numVerts * 9 * sizeof(float),	//복사할 바이트 크기 (각 버텍스의 데이터 크기에 버텍스 개수를 곱한 값)
		verts,							//복사할 소스(포인터)
		GL_STATIC_DRAW);				//이 데이터를 어떻게 사용할 것인가?

	//GL_STATIC_DRAW 는 데이터를 오직 한 번만 로드하며, 버텍스가 자주 그려지는 경우에 사용되는 옵션.

	//인덱스 버퍼 생성
	glGenBuffers(1, &mIndexBuffer);
	//glBindBUffer의 첫 번째 인자인 GL_ELEMENT_ARRAY_BUFFER 파라미터는 인덱스 버퍼에 해당하는 타입.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	//인덱스 버퍼 생성 후 인덱스 데이터를 인덱스 버퍼에 복사.
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			//인덱스 버퍼
		numIndices * sizeof(unsigned int),	//데이터의 크기 (인덱스 수에 unsigned int 크기 곱한 값) [unsinged int가 인덱스에 사용]
		indices,
		GL_STATIC_DRAW);

	//첫번째 버텍스 속성(0번 인덱스) 활성화를 위해 glEnableVertexAttribArray 사용
	// (For now, assume one vertex format)
	// 위치는 버텍스 속성 0이며 3개의 요소(x, y, z)가 있으므로 처음 두 파라미터는 0과 3이다.
	glEnableVertexAttribArray(0);
	//크기, 타입, 속성의 포맷을 지정하기 위해 glVertexAttribPointer 사용
	glVertexAttribPointer(
		0,					//속성 인덱스 (첫 번째 버텍스 속성 인덱스는 0)
		3,					//요소의 수 (이번 예에서는 3)
		GL_FLOAT,			//요소의 타입
		GL_FALSE,			//(정수형 타입에서만 사용된다.)
		sizeof(float) * 9,	//간격 (일반적으로 각 버텍스의 크기) [버텍스 버퍼에서 패딩이 없다면 간격은 버텍스의 크기가 됨]
		0);					//버텍스의 시작에서 이 속성까지의 오프셋 (0은 위치 속성은 버텍스의 시작 위치와 동일하기 때문)
	//추가 속성에 대해서는 오프셋에 0이 아닌 값을 전달해야함.

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,											//버텍스 속성 인덱스
		2,											//컴포넌트의 수 (UV, 2)
		GL_FLOAT,									//각 컴포넌트의 타입
		GL_FALSE,									//GL_FLOAT 에서는 사용 하지 않음
		sizeof(float) * 9,							//간격 (간격은 항상 버텍스의 크기)
		reinterpret_cast<void*>(sizeof(float) * 3));//오프셋 포인터

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,											//버텍스 속성 인덱스
		4,											//컴포넌트의 수 (RGBA, 4)
		GL_FLOAT,									//각 컴포넌트의 타입
		GL_FALSE,									//GL_FLOAT 에서는 사용 하지 않음
		sizeof(float) * 9,							//간격 (간격은 항상 버텍스의 크기)
		reinterpret_cast<void*>(sizeof(float) * 5));//오프셋 포인터
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
