R"(
//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec3 aNormal;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vSurfaceNormal;
out vec3 vToLightVector;
out vec3 vToCameraVector;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uLightPosition = vec3(0.0, 10.0, -10.0);
uniform vec4 uMaterialColor = vec4(1,1,1,1);

void main()
{
	vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1.0f);
	vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * positionRelativeToCamera;
	vColor = uMaterialColor * vec4(aColor.b, aColor.g, aColor.r, aColor.a);

	vTexCoord = aTexCoord;
	vSurfaceNormal = mat3(uTransformationMatrix) * aNormal;
	vToLightVector = uLightPosition - worldPosition.xyz;
	vToCameraVector = uCameraPos - worldPosition.xyz;
}

//fragment
#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vSurfaceNormal;
in vec3 vToLightVector;
in vec3 vToCameraVector;

out vec4 fColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 uFogColor;
uniform float uHasColor = 1.0;
uniform float uSpecularExponent = 10.0;
uniform float uSpecularStrength = 1;
uniform float uDiffuseStrength = 1;
uniform float uAmbient = 0.3;

void main()
{
	fColor = vColor;
	if (uHasColor > 0.5)
	{
		fColor *= texture(uTexture, vTexCoord);
		if (fColor.a < 0.1)
			discard;
	}
	vec3 unitNormal = normalize(vSurfaceNormal);
	vec3 unitLightVector = normalize(vToLightVector);
	vec3 unitVectorToCamera = normalize(vToCameraVector);
	vec3 lightDirection = -unitLightVector;

	float uDiffuse = uDiffuseStrength * max(dot(unitNormal, unitLightVector), 0.0);
	float uSpecular = uSpecularStrength * pow(max(dot(reflect(lightDirection, unitNormal), unitVectorToCamera), 0.0f), uSpecularExponent);

	fColor *= vec4((uAmbient + (uDiffuse + uSpecular) * uLightColor), 1.0f);
}
)"
