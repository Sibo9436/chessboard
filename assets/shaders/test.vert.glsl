#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexSampler;
out vec2 TexCoord;
out float TexSampler;

void main(){
	gl_Position = vec4(aPos, 1.0);
  TexCoord = aTexCoord;
  TexSampler = aTexSampler;
}
