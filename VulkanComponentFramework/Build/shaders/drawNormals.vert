#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec3 inNormal;

layout (binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout (push_constant) uniform PushConst {
	mat4 model;
	mat4 normal;
} pushConst;


layout (location = 0) out VertexStage {
    vec3 normal;
} vs_out;



void main() {

    mat3 normalMatrix = mat3(pushConst.normal);
    gl_Position = ubo.view * pushConst.model * inPosition;
    vs_out.normal = mat3(pushConst.normal) * inNormal;
}