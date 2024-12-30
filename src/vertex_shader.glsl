#version 330 core
layout (location = 0) in vec3 v_Loc;
layout (location = 1) in vec3 v_Col;
out vec4 fragmentColor;
void main() {
    gl_Position = vec4(v_Loc.x, v_Loc.y, v_Loc.z, 1.0f);
    fragmentColor = vec4(v_Col, 1.0f);
}