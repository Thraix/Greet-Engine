#pragma once

#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>
#include <math/Vec3.h>

namespace Greet {

  class Renderer3D;

  class EntityModel {
    private:
      Ref<Material> material;
      Ref<Mesh> mesh;
    private:
      Vec3f m_position;
      Vec3f m_scale;
      Vec3f m_rotation;
      Mat4 m_transformationMatrix;
      bool m_hasChanged;
    public:
      EntityModel(const Ref<Mesh>& mesh, const Ref<Material>& material, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
      EntityModel(const Ref<Mesh>& mesh, const Ref<Material>& material, Vec3f position, Vec3f scale, Vec3f rotation);
      EntityModel(const Ref<Mesh>& mesh, const Ref<Material>& material);

      virtual void PreRender() const;
      virtual void Render() const;
      virtual void PostRender() const;

      virtual void BindShader() const;
      virtual void UnbindShader() const;
      virtual void BindTransformation() const;

      const Ref<Material>& GetMaterial() const { return material; }
      void SetMaterial(const Ref<Material>& material) { this->material = material;}
      const Ref<Mesh>& GetMesh() const { return mesh; }
      void SetMesh(const Ref<Mesh>& mesh) { this->mesh = mesh;}
      const Mat4& GetTransformationMatrix() const { return m_transformationMatrix; }
      virtual void Update(float timeElapsed);
      void UpdateTransformation();
      void SetPosition(const Vec3f& pos) { m_position = pos; m_hasChanged = true; }
      void SetScale(const Vec3f& scale) { m_scale = scale; m_hasChanged = true; }
      void SetRotation(const Vec3f& rotation) { m_rotation = rotation; m_hasChanged = true; }
      void Move(float dx, float dy, float dz) { m_position.x += dx;m_position.y = dy; m_position.z = dz; m_hasChanged = true; }
      void Scale(float x, float y, float z) { m_scale.x += x;m_scale.y += y; m_scale.z += z; m_hasChanged = true; }
      void Rotate(float x, float y, float z) { m_rotation.x += x;m_rotation.y += y; m_rotation.z += z; m_hasChanged = true; }

      const Vec3f& GetPosition() const { return m_position;}
      const Vec3f& GetScale() const { return m_scale;}
      const Vec3f& GetRotation() const { return m_rotation;}
  };
}
