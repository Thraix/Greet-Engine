//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in float aIs2D;

out vec4 vColor;

uniform mat3 uPVMatrix2D;
uniform mat4 uPVMatrix3D;

void main()
{
  if(aIs2D > 0.5)
  {
    gl_Position = vec4(uPVMatrix2D * vec3(aPosition.xy, 1.0f), 1.0f);
  }
  else
  {
    gl_Position = uPVMatrix3D * vec4(aPosition, 1.0f);
  }
	vColor = aColor;
}

//fragment
#version 330 core

in vec4 vColor;
out vec4 fColor;

void main()
{
	fColor = vColor;
}
