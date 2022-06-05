R"(
//vertex
#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in float aTexID;
layout(location = 3) in vec4 aColor;

out vec2 vPos;
out float vTexID;
out vec2 vTexCoord;
out vec4 vColor;

uniform mat3 uProjectionMatrix = mat3(1.0);
uniform mat3 uViewMatrix = mat3(1.0);

void main()
{
	gl_Position = vec4(uProjectionMatrix * uViewMatrix * vec3(aPosition, 1.0), 1.0);
	vPos = vec2(vec3(aPosition, 1.0));
	vTexCoord = vec2(aTexCoord.x, 1 - aTexCoord.y);
	vTexID = aTexID;
	vColor = vec4(aColor.z, aColor.y, aColor.x, aColor.w);
}

//fragment
#version 330 core

out vec4 fColor;

in vec2 vPos;
in float vTexID;
in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTextures[32];


void main()
{
	fColor = vColor;
	if (vTexID > 0.0)
	{
		int tid = int(vTexID - 0.5);
		fColor *= texture(uTextures[tid], vTexCoord);
	}
}
)"
