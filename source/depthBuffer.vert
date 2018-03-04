#version 330 core

// 入力頂点データ。このシェーダのすべての実行で異なる
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 3) in mat4 M;

// メッシュ全体で固定した値
uniform mat4 depthMV;

void main(){
	gl_Position =  depthMV * M * vec4(vertexPosition_modelspace,1);
}
