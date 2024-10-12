#version 460 core

layout (location = 0) out vec4 outColor;

layout (std140, binding = 5) uniform DebugLine
{
    uniform vec3 debugLineStart;
    uniform vec3 debugLineEnd;
};

float sdCappedCylinder( vec3 p, float h, float r )
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - vec2(h,r);
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

void main() 
{
    vec3 lineCenter = (debugLineStart - debugLineEnd) / 2.0;
    float lenght = distance(debugLineStart, debugLineEnd);
    float d = sdCappedCylinder(lineCenter, lenght, 10.0);

    outColor = vec4(0,d,0,1);
}