#version 330 core

in  vec2 uv;
in  vec3 rgb;
out vec4 color;

uniform sampler2D tex;

void main() {

	float alpha = texture(tex, uv).r;
	color = vec4(rgb.r, rgb.g, rgb.b, alpha);

}
