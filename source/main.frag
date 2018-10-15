#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec3 Normal;


// Ouput data
out vec3 color;

uniform vec3 LightColor;
uniform float LightPower;
uniform vec3 LightDirection;


void main(){

	float visibility = 1.0f;
	vec3 MaterialDiffuseColor = fragmentColor;

	vec3 l = normalize(LightDirection);
	float cosTheta = clamp(dot(Normal, l), 0, 1);
	float bias = 0.0005*tan(acos(cosTheta)); // cosThetaはdot( n,l )で0と1の間にします。
	bias = clamp(bias, 0, 0.01);

	vec3 MaterialAmbientColor = vec3(0.3, 0.3, 0.3) * MaterialDiffuseColor;

	color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta;
}
