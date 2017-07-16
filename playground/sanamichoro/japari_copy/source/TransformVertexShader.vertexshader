#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in mat4 M;


out vec3 fragmentColor;
out vec3 Normal;


uniform mat4 V;
uniform mat4 P;


void main(){	

	gl_Position =  P * V * M * vec4(vertexPosition_modelspace ,1);
	fragmentColor = vertexColor;

	Normal = normalize(M * vec4(vertexNormal_modelspace,0)).xyz;

}

