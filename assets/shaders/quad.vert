#version 420 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexIdx;

uniform float uScale;
uniform vec2 uTranslation;
uniform ivec2 uViewport;

out vec2 TexCoord;
out float TexIdx;

void main(){
  //Ricordo che aPos è in pixelspace;
  //potrebbe avere più senso prima scalare e poi traslare per ragioni di calcoli
	vec2 pos = aPos - (uViewport/2.0f);
  pos = pos / (uViewport/2.0f);
	gl_Position = vec4(pos,0.0, 1.0);
	TexCoord = aTexCoord;
  TexIdx =aTexIdx;
}
