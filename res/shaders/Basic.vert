#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;//Model View Projection Matrix Uniform

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
};

// // 临时修改顶点着色器
// #version 330 core
// void main() {
//     gl_Position = vec4(-0.5,-0.5,0,1); // 左下角
//     // gl_Position = vec4(0.5,0.5,0,1); // 右上角
// }



