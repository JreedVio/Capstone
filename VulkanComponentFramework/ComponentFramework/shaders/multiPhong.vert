#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in  vec4 inPosition;
layout (location = 1) in  vec4 inNormal;
layout (location = 2) in  vec2 inTexCoord;

layout (binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout (binding = 1) uniform GlobalLightingUBO {
	vec4 lightPos[2];
	vec4 diffuse;
} glights;

layout (push_constant) uniform PushConst {
	mat4 model;
	mat4 normal;
} pushConst;


layout (location = 0) out vec3 vertNormal;
layout (location = 1) out vec3 lightDir[2];
layout (location = 3) out vec3 eyeDir;
layout (location = 4) out vec2 fragTexCoord;

void main() {

	fragTexCoord = inTexCoord;

	mat3 normalMatrix = mat3(pushConst.normal);

	vertNormal = normalize(normalMatrix * inNormal.xyz);
	vec3 vertPos = vec3(ubo.view * pushConst.model * inPosition);
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	for(int i = 0; i < 2; ++i){
		lightDir[i] = normalize(vec3(glights.lightPos[i]) - vertPos);
	}
	gl_Position = ubo.proj * ubo.view * pushConst.model * inPosition;
}
