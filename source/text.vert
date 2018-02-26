#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 7) in vec2 _pos;
layout(location = 8) in vec2 _uv;
layout(location = 9) in vec3 _rgb;

out vec2 uv;
out vec3 rgb;

void main(){

	uv = _uv;
	rgb = _rgb;
    gl_Position = vec4(_pos.x, _pos.y, 0.0, 1.0);

}
