#version 330 core

out vec4 FragColor;

in float v_ColorR;
in float v_ColorG;
in float v_ColorB;
in float v_ColorAlpha;

void main(){
	FragColor = vec4(v_ColorR, v_ColorG, v_ColorB, v_ColorAlpha);
}