#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in mat4 M;


out vec3 fragmentColor;
out vec3 Normal;
out vec4 shadowCoord0;
out vec4 shadowCoord1;
out vec4 shadowCoord2;
out vec4 shadowCoord3;

uniform mat4 V;
uniform mat4 P;
uniform mat4 DepthBiasVP0;
uniform mat4 DepthBiasVP1;
uniform mat4 DepthBiasVP2;
uniform mat4 DepthBiasVP3;

void main(){
	gl_Position =  P * V * M * vec4(vertexPosition_modelspace ,1);
	fragmentColor = vertexColor;

	Normal = normalize(M * vec4(vertexNormal_modelspace,0)).xyz;

	shadowCoord0 = DepthBiasVP0 * M * vec4(vertexPosition_modelspace, 1);
	shadowCoord1 = DepthBiasVP1 * M * vec4(vertexPosition_modelspace, 1);
	shadowCoord2 = DepthBiasVP2 * M * vec4(vertexPosition_modelspace, 1);
	shadowCoord3 = DepthBiasVP3 * M * vec4(vertexPosition_modelspace, 1);
}

