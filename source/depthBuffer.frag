#version 330 core

// 出力データ
layout(location = 0) out float fragmentdepth;

void main(){
	// OpenGLがやってくれるので、本当は必要ない
	fragmentdepth = gl_FragCoord.z;
}
