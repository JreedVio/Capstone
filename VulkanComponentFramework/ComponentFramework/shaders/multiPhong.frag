#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in  vec3 vertNormal;
layout(location = 1) in  vec3 lightDir[2];
layout(location = 3) in  vec3 eyeDir;
layout(location = 4) in  vec2 fragTexCoord;

layout (binding = 1) uniform GlobalLightingUBO {
	vec4 lightPos[2];
	vec4 diffuse;
} glights;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) out vec4 fragColor;

void main() {
	vec4 kt = texture(texSampler, fragTexCoord);
	const vec4 kd[2] = vec4[2](vec4(0.6, 0.0, 0.0, 0.0),
	vec4(0.0, 0.6, 0.0, 0.0));
	//vec4 ks[2];
	//for(int i = 0; i < 2; i++){
	//	ks[i] = 0.5 * kd[i];
	//}
	vec4 ks = vec4(0.2, 0.2, 0.6, 0.0);

	vec4 ka = 0.1 * kt;
	float diff[2];
	vec3 reflection[2];
	float spec[2];
	for(int i = 0; i < 2; i++){
		diff[i] = max(dot(vertNormal, lightDir[i]), 0.0);
		reflection[i] = normalize(reflect(-lightDir[i], vertNormal));
		spec[i] = max(dot(eyeDir, reflection[i]), 0.0);
		if(diff[i] > 0.0){
			spec[i] = pow(spec[i], 14.0);
		}
	}
	vec4 color = ka;
	for(int i = 0; i < 2; i++){
		color += (diff[i] * kt * kd[i]) + (spec[i] * ks);
		fragColor = color;
	}
} 

