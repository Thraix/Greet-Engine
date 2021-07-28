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
uniform float uIntensity = 1.0;

void main()
{
  vec4 texColor = texture(uTexture, vTexCoord);
  float brightness = texColor.r * 0.2126 + texColor.g * 0.7152 + texColor.b * 0.0722;
  brightness = min(brightness, 1.0);
  fColor = texColor * int(brightness > uThreshold) * pow(brightness, 1 / uIntensity);
}
)"
