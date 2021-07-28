#shader vertex

#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPos;

void main()
{
  vPos = aPosition;
}

#shader geometry

#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uLightColor = vec3(1.0f, 0.96f, 0.9f);
uniform vec3 uLightPosition = vec3(-100, 100, 0);

in vec3 vPos[];

out vec4 gColor;
out float gVisibility;

uniform float uFogNearDistance = 100;
uniform float uFogFarDistance = 140;

vec3 GetColorFromHeight(float y)
{
  if (y < 0.48)
    return vec3(0.91, 0.83, 0.38);
  else if (y < 0.58)
    return vec3(0.62, 0.90, 0.35);
  else if (y < pow(0.07, 0.6) + 0.58)
    return vec3(0.76, 0.71, 0.63);
  else
    return vec3(0.88, 0.95, 0.97);
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
  pos[0] = vec3(uTransformationMatrix * vec4(vPos[0], 1.0f));
  pos[1] = vec3(uTransformationMatrix * vec4(vPos[1], 1.0f));
  pos[2] = vec3(uTransformationMatrix * vec4(vPos[2], 1.0f));
  vec3 color = GetColorFromHeight((vPos[0].y + vPos[1].y + vPos[2].y) / 3.0f);
  vec3 normal = normalize(cross(pos[1] - pos[0], pos[2] - pos[0]));

  for(int i = 0; i < 3; i++)
  {
    gColor = vec4(color, 1.0);
    vec4 positionRelativeToCamera = uViewMatrix * vec4(pos[i], 1.0);
    gl_Position = uProjectionMatrix * positionRelativeToCamera;

    gVisibility = GetFogVisibility(positionRelativeToCamera.xyz);
    /* if(distance < uFogNearDistance) */
    /*   gVisibility = 1.0f; */
    gColor *= vec4(0.8f, 0.8f, 0.8f, 1.0f);

    vec3 unitLightVector = normalize(uLightPosition);
    float nDot = dot(normal, unitLightVector);
    float brightness = nDot;
    if (brightness < 0.6)
      brightness = mix(0.4, 0.6, brightness / 0.6);

    vec3 diffuse = uLightColor * brightness;
    gColor *= vec4(diffuse, 1.0f);
    EmitVertex();
  }
  EndPrimitive();
}

#shader fragment

#version 330 core

in vec4 gColor;
in float gVisibility;

out vec4 fColor;

uniform vec3 uFogColor = vec3(0.125, 0.125, 0.125);

void main()
{
	fColor = gColor;
	fColor = mix(vec4(uFogColor, fColor.a), vec4(fColor.rgb, fColor.a), gVisibility);
}
