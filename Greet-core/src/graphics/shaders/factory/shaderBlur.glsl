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

const float[11] guassian = float[11](0.0093, 0.028002, 0.065984, 0.121703, 0.175713, 0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093);
uniform sampler2D uTexture;
uniform vec2 uSampleOffset;

void main()
{
  fColor = vec4(0);
  for(int i = 0; i < 11; i++)
  {
    fColor += texture(uTexture, vTexCoord + (i - 5) * uSampleOffset) * guassian[i];
  }
}
)"
