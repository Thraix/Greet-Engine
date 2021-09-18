#pragma once

#include "Chunk.h"
#include <graphics/layers/Scene.h>
#include <graphics/models/EntityModel.h>
#include <graphics/cameras/Camera3D.h>

class World : public Greet::Scene
{
  Greet::Camera3D* camera;
  Greet::Material terrainMaterial;
  uint32_t width;
  uint32_t length;

  Chunk* chunks;
  public:

    World(Greet::Camera3D* camera, uint32_t width, uint32_t length)
      : camera{camera}, width{width}, length{length}, terrainMaterial{Greet::Shader::FromFile("res/shaders/terrain.shader")}
    {
      chunks = new Chunk[width * length];
      for(int z = 0;z < length; z++)
      {
        for(int x = 0;x < width; x++)
        {
          chunks[x+z*width].Initialize(x,z);
        }
      }
    }

    void Render() const override
    {
      terrainMaterial.Bind();
      camera->SetShaderUniforms(terrainMaterial.GetShader());
      for(int z = 0;z < length; z++)
      {
        for(int x = 0;x < width; x++)
        {
          terrainMaterial.GetShader()->SetUniformMat4("uTransformationMatrix", Greet::Mat4::Translate((x - width / 2.0f) * Chunk::CHUNK_WIDTH, -15, (z - length / 2.0f) * Chunk::CHUNK_HEIGHT));
          Greet::Mesh* mesh = chunks[x + z * width].mesh;
          mesh->Bind();
          mesh->Render();
          mesh->Unbind();
        }
      }
      terrainMaterial.Unbind();
    }

    void Update(float timeElapsed) override
    {

    }
};
