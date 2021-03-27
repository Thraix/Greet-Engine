//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec3 aNormal;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vSurfaceNormal;
out vec3 vToCameraVector;
out float vVisibility;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix = mat4(1);
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec4 uMaterialColor = vec4(1,1,1,1);
uniform float uFogNearDistance = 100;
uniform float uFogFarDistance = 140;

float GetFogVisibility(vec3 positionRelativeToCamera)
{
  float visibility = 1 - (length(positionRelativeToCamera) - uFogNearDistance) / (uFogFarDistance - uFogNearDistance);
  visibility = clamp(visibility, 0.0, 1.0);
  return visibility;
}

void main()
{
	vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1.0f);
	vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * positionRelativeToCamera;
	vColor = uMaterialColor * vec4(aColor.b, aColor.g, aColor.r, aColor.a);

	vTexCoord = aTexCoord;
	vSurfaceNormal = mat3(uTransformationMatrix) * aNormal;
	vToCameraVector = uCameraPos - worldPosition.xyz;
  vVisibility = GetFogVisibility(positionRelativeToCamera.xyz);
}

//fragment
#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vSurfaceNormal;
in vec3 vToLightVector;
in vec3 vToCameraVector;
in float vVisibility;

out vec4 fColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 uFogColor = vec3(0.125, 0.125, 0.125);
uniform float uHasTexture = 1.0;
uniform float uSpecularExponent = 10.0;
uniform float uSpecularStrength = 1;
uniform float uDiffuseStrength = 1;
uniform float uAmbient = 0.3;
uniform vec3 uLightDirection = vec3(1,1,1);

void main()
{
	fColor = vColor;
	if (uHasTexture > 0.5)
	{
		fColor *= texture(uTexture, vTexCoord);
		if (fColor.a < 0.1)
			discard;
	}
	vec3 unitNormal = normalize(vSurfaceNormal);
	vec3 unitLightVector = normalize(uLightDirection);
	vec3 unitVectorToCamera = normalize(vToCameraVector);
	vec3 lightDirection = -unitLightVector;

	float uDiffuse = uDiffuseStrength * max(dot(unitNormal, unitLightVector), 0.0);
	float uSpecular = uSpecularStrength * pow(max(dot(reflect(lightDirection, unitNormal), unitVectorToCamera), 0.0f), uSpecularExponent);

	fColor *= vec4((uAmbient + (uDiffuse + uSpecular) * uLightColor), 1.0f);
  fColor = mix(vec4(uFogColor.xyz, fColor.a), vec4(fColor), vVisibility);
  fColor = vColor;
}
