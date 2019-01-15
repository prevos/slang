#version 450

#define MAX_POINT_CLOUD_SIZE 9
#define float2 vec2
#define float3 vec3
#define static

layout(std140, set = 0, binding = 0) uniform UBO
{
    uniform mat4 uMVP;
};

#pragma stage vertex
layout(location = 0) in vec4 aVertex;
layout(location = 1) in vec3 aNormal;

layout(location = 0) out vec3 vNormal;

float3 get_eye_pos(float3 eye_pos,
    const float2 geom_aspect, const float3 global_coords[MAX_POINT_CLOUD_SIZE],
    const int num_points)
{
    return global_coords[num_points] * eye_pos;
}

void main()
{
    static const int num_points = MAX_POINT_CLOUD_SIZE;
    float3 global_coords[MAX_POINT_CLOUD_SIZE];
    vNormal = get_eye_pos(aNormal, float2(aVertex.x, aVertex.y), global_coords, num_points);
    gl_Position = uMVP * aVertex;
}

#pragma stage fragment
layout(location = 0) in vec3 vNormal;
layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(vNormal, 1.0);
}
