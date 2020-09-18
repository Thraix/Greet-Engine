//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec4 vColor;

uniform vec4 uMaterialColor = vec4(1,0,0,1);
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
	vec4 positionRelativeToCamera = uViewMatrix * vec4(aPosition, 1.0f);
	gl_Position = uProjectionMatrix * positionRelativeToCamera;
	vColor = uMaterialColor;
}

//fragment
#version 330 core

in vec4 vColor;
out vec4 fColor;

void main()
{
	fColor = vColor;
	return;
}
