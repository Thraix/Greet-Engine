//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPos;

void main()
{
  vPos = aPosition;
}

//geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 vPos[];

out vec4 gColor;
out float gVisibility;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uLightColor = vec3(1.0f, 0.96f, 0.9f);
uniform float uTime;
uniform float uShadeDamper = 10.0;
uniform float uReflectivity = 1;

uniform float uFogNearDistance = 100;
uniform float uFogFarDistance = 140;

const float cWaveLength = 4.0;
const float cWaveAmplitude = 0.2;
const float cPI = 3.1415926535897932384626433832795;

float generateOffset(float x, float z, float val1, float val2){
	float radiansX = ((mod(x+z*x*val1, cWaveLength)/cWaveLength) + uTime * 0.125 * mod(x * 0.8 + z, 1.5)) * 2.0 * cPI;
	float radiansZ = ((mod(val2 * (z*x +x*z), cWaveLength)/cWaveLength) + uTime * 0.25 * mod(x , 2.0) ) * 2.0 * cPI;
	return cWaveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 GetPosition(vec3 pos)
{
	float xDistortion = generateOffset(pos.x, pos.z, 0.2, 0.1);
	float yDistortion = generateOffset(pos.x, pos.z, 0.1, 0.3);
	float zDistortion = generateOffset(pos.x, pos.z, 0.15, 0.2);
	return pos + vec3(xDistortion, yDistortion, zDistortion);
}

float GetFogVisibility(vec3 positionRelativeToCamera)
{
  float visibility = 1 - (length(positionRelativeToCamera) - uFogNearDistance) / (uFogFarDistance - uFogNearDistance);
  visibility = clamp(visibility, 0.0, 1.0);
  return visibility;
}

void main()
{
  vec3 pos[3];
  pos[0] = vec3(uTransformationMatrix * vec4(GetPosition(vPos[0]), 1.0));
  pos[1] = vec3(uTransformationMatrix * vec4(GetPosition(vPos[1]), 1.0));
  pos[2] = vec3(uTransformationMatrix * vec4(GetPosition(vPos[2]), 1.0));

  vec3 normal = normalize(cross(pos[1] - pos[0], pos[2] - pos[0]));
  for(int i = 0; i < 3; i++)
  {
    gColor = vec4(0.32, 0.91, 0.96, 0.5);
    vec4 worldPosition = vec4(pos[i], 1.0);
    vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
    gl_Position = uProjectionMatrix * positionRelativeToCamera;

    vec3 toLightVector = vec3(-100, 100, 0);
    vec3 toCameraVector = uCameraPos - worldPosition.xyz;

    gVisibility = GetFogVisibility(positionRelativeToCamera.xyz);
    vec3 unitLightVector = normalize(toLightVector);

    float nDot = dot(normal, unitLightVector);
    float brightness = max(nDot, 0.5);
    vec3 diffuse = uLightColor * brightness;

    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 lightDirection = -unitLightVector;
    vec3 reflection = reflect(lightDirection, normal);

    float specularFactor = dot(reflection, unitVectorToCamera);
    specularFactor = max(specularFactor, 0.0);

    float dampedFactor = pow(specularFactor, uShadeDamper);

    vec3 finalSpecular = dampedFactor  * uReflectivity * uLightColor;
    gColor *= vec4(diffuse, 1.0f);
    gColor += vec4(finalSpecular, 0.0);

    gColor *= vec4(diffuse, 1.0f);

    float fresnel = ((1-dot(normal, unitVectorToCamera)) + 0.5) * 0.5;
    gColor.a = fresnel;
    EmitVertex();
  }
  EndPrimitive();
}

//fragment
#version 330 core

in vec4 gColor;
in float gVisibility;

out vec4 fColor;

uniform vec3 uFogColor = vec3(0.125, 0.125, 0.125);

void main()
{
	fColor = gColor;
  fColor = mix(vec4(uFogColor, fColor.a), vec4(fColor), gVisibility);
  fColor.a = min(fColor.a, gVisibility);
}
