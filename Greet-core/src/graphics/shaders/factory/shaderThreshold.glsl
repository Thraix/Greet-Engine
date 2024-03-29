R"(
//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec2 vTexCoord;

void main()
{
	gl_Position = vec4(aPosition, 1.0);
	vTexCoord = (aPosition.xy + 1) * 0.5;
}

//fragment
#version 330 core

in vec2 vTexCoord;

out vec4 fColor;

uniform sampler2D uTexture;
uniform float uThreshold = 0.7;

void main()
{
  vec4 texColor = texture(uTexture, vTexCoord);
  float brightness = max(texColor.r, max(texColor.g, texColor.b));
  float contribution = max(0, brightness - uThreshold) / max(brightness, 0.00001);

  fColor = texColor * contribution;
}
)"
