#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};

// // 临时修改片段着色器
// #version 330 core
// out vec4 color;
// void main() {
//     color = vec4(1.0,0.0,0.0,1.0); // 纯红色
// }