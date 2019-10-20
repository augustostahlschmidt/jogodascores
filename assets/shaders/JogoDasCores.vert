#version 330 core

layout(location = 0) in vec3 vao_Position;

uniform float u_ColorR;
uniform float u_ColorG;
uniform float u_ColorB;
uniform float u_ColorAlpha;

uniform float u_OffsetX;
uniform float u_OffsetY;

out float v_ColorR;
out float v_ColorG;
out float v_ColorB;
out float v_ColorAlpha;

uniform mat4 u_MVP;

void main(){

	//gl_Position = vec4(vao_Position.x + u_OffsetX, vao_Position.y + u_OffsetY, 1.0, 1.0);

	gl_Position = u_MVP * vec4(vao_Position.x + u_OffsetX, vao_Position.y + u_OffsetY, 1.0, 1.0);

	v_ColorR = u_ColorR;
	v_ColorG = u_ColorG;
	v_ColorB = u_ColorB;
	v_ColorAlpha = u_ColorAlpha;
}