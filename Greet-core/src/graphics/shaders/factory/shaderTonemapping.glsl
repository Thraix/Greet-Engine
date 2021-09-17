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

void main()
{
  vec3 color = texture(uTexture, vTexCoord).rgb;
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;

  fColor = vec4(clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0), 1.0);
}
)"
