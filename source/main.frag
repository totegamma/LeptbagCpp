#version 330 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec3 Normal;
in vec4 shadowCoord0;
in vec4 shadowCoord1;
in vec4 shadowCoord2;
in vec4 shadowCoord3;


// Ouput data
out vec3 color;

uniform vec3 LightColor;
uniform float LightPower;
uniform vec3 LightDirection;
uniform sampler2DShadow shadowMap0;
uniform sampler2DShadow shadowMap1;
uniform sampler2DShadow shadowMap2;
uniform sampler2DShadow shadowMap3;


void main(){

	float visibility = 1.0f;
	vec3 MaterialDiffuseColor = fragmentColor;

	vec3 l = normalize(LightDirection);
	float cosTheta = clamp(dot(Normal, l), 0, 1);
	float bias = 0.0005*tan(acos(cosTheta)); // cosThetaはdot( n,l )で0と1の間にします。
	bias = clamp(bias, 0, 0.01);

	if (0 < shadowCoord0.x && shadowCoord0.x < 1 && 0 < shadowCoord0.y && shadowCoord0.y < 1 && 0 < shadowCoord0.z && shadowCoord0.z < 1) {
		if (texture( shadowMap0, vec3(shadowCoord0.xy, (shadowCoord0.z - bias)/shadowCoord0.w)) == 0){
			visibility = 0.5f;
		}
		//MaterialDiffuseColor = vec3(1.0, 0.0, 0.0);
	} else if (0 < shadowCoord1.x && shadowCoord1.x < 1 && 0 < shadowCoord1.y && shadowCoord1.y < 1 && 0 < shadowCoord1.z && shadowCoord1.z < 1) {
		if (texture( shadowMap1, vec3(shadowCoord1.xy, (shadowCoord1.z - bias)/shadowCoord1.w)) == 0){
			visibility = 0.5f;
		}
		//MaterialDiffuseColor = vec3(0.0, 0.0, 1.0);
	} else if (0 < shadowCoord2.x && shadowCoord2.x < 1 && 0 < shadowCoord2.y && shadowCoord2.y < 1 && 0 < shadowCoord2.z && shadowCoord2.z < 1) {
		if (texture( shadowMap2, vec3(shadowCoord2.xy, (shadowCoord2.z - bias)/shadowCoord2.w)) == 0){
			visibility = 0.5f;
		}
		//MaterialDiffuseColor = vec3(0.0, 1.0, 0.0);
	} else if (0 < shadowCoord3.x && shadowCoord3.x < 1 && 0 < shadowCoord3.y && shadowCoord3.y < 1 && 0 < shadowCoord3.z && shadowCoord3.z < 1) {
		if (texture( shadowMap3, vec3(shadowCoord3.xy, (shadowCoord3.z - bias)/shadowCoord3.w)) == 0){
			visibility = 0.5f;
		}


		//MaterialDiffuseColor = vec3(1.0, 1.0, 0.0);

		if (shadowCoord3.z > 1) {
			MaterialDiffuseColor = vec3(1.0, 0, 0);
		}


	} else {
		//MaterialDiffuseColor = vec3(1, 1, 1);
	}

	vec3 MaterialAmbientColor = vec3(0.3, 0.3, 0.3) * MaterialDiffuseColor;


	color = MaterialAmbientColor
		  + visibility * MaterialDiffuseColor * LightColor * LightPower * cosTheta;
}
