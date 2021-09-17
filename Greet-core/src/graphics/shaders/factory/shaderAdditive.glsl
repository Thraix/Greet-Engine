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

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

uniform float uWeightTexture1;
uniform float uWeightTexture2;

void main()
{
  fColor = uWeightTexture1 * texture(uTexture1, vTexCoord) + uWeightTexture2 * texture(uTexture2, vTexCoord);
  fColor.a = 1.0;
}
)"
