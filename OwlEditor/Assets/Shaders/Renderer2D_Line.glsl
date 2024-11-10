#type vertex
#version 450 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec4 in_Color;
layout(location = 2) in int in_EntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) out flat int v_EntityId;

void main() 
{
	Output.Color = in_Color;
	v_EntityId = in_EntityId;

	gl_Position = u_ViewProjection * vec4(in_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 out_Color;
layout(location = 1) out int out_EntityId;

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) in flat int v_EntityId;

void main() 
{
	out_Color = Input.Color;

	out_EntityId = v_EntityId;
}