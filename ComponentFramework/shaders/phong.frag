#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 vertNormal;
layout (location = 1) in vec3 lightDir;
layout (location = 2) in vec3 eyeDir; 
layout (location = 3) in vec2 fragTexCoords; 

layout (location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() { 
	const vec4 ks = vec4(0.2, 0.2, 0.6, 0.0);
	const vec4 kd = vec4(0.2, 0.2, 0.6, 0.0); /// const means it cannot be changed just like C++
	const vec4 ka = 0.1 * kd;
	vec4 kt = texture(texSampler, fragTexCoords);
	
	float diff = max(dot(vertNormal, lightDir), 0.0);
	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	//fragColor =  ka + (diff * kt * kd) + (spec * ks);	
	fragColor =  ka + (diff * kt) + (spec * ks);	
} 

