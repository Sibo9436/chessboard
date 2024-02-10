#version 420 core
in vec2 TexCoord;
in float TexSampler;
out vec4 outColor;

uniform sampler2D[2] samplers;

void main(){
  int texIdx = int(TexSampler);
	outColor = texture(samplers[texIdx], TexCoord);//*vec4(TexCoord.x,1.0-TexCoord.y, 0.0,1.0) ;
  //outColor = vec4(TexCoord,0.0,0.0);
}
