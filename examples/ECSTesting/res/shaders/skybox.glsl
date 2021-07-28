//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPos;

uniform mat4 uPVInvMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

void main()
{
  vec3 worldPos = mat3(uViewMatrix) * aPosition;
	vec4 pos = uProjectionMatrix * vec4(worldPos, 1.0);
  gl_Position = pos;
  vPos = aPosition;
}

//fragment
#version 330 core

in vec3 vPos;

out vec4 fColor;

uniform mat4 uInvPVMatrix;
uniform vec3 uSunDir = vec3(0.7, 0.7, 0.7);

void main()
{
  vec3 dir = vPos;

  vec3 unitDir = normalize(dir);
  float sun = 10 * pow(dot(normalize(uSunDir), unitDir), 400.0);
  float grad = (unitDir.y + 1.0) * 0.5;
  vec3 skycolor = mix(vec3(0.13, 0.37, 0.35), vec3(0.38, 0.85, 0.80), pow(grad, 2.0f));
  vec3 skyboxColor = skycolor + max(vec3(sun, sun, 0), vec3(0));
	fColor = vec4(skyboxColor, 1.0);
}
