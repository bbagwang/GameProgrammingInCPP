// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

//GLSL 3.3 요청
#version 330

// This corresponds to the output color
// to the color buffer
//RGBA 색상 버퍼의 4개 요소에 해당하는 vec4 타입의 변수
//출력 변수이므로 앞에 out 키워드가 들어감.
//최종적으로 구한 색상을 outColor에 저장. (색상 버퍼로 출력되는 컬러)
out vec4 outColor;

void main()
{
    //파란색으로 설정
    outColor = vec4(0.0, 0.0, 1.0, 1.0);
}
