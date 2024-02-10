#version 420 core

in vec2 TexCoord;
in float TexIdx;

out vec4 outColor;

uniform sampler2D[16] uTextures;

void main(){
  int idx = int(TexIdx);
	outColor = texture(uTextures[idx], TexCoord);
	//outColor = vec4(1.0f, TexCoord.x, TexCoord.y,1.0f);
  //outColor = vec4(1.0f,TexIdx/16.0f,0.0f,1.0f);
  //outColor = vec4(1.0f * TexIdx/16.0f, 0.0f,0.0f,1.0f);
	//outColor = vec4(1.0f, 1.0f,1.0f,1.0f);
}

