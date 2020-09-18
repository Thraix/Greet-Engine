#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/textures/Texture2D.h>

namespace Greet {

  class Material
  {
    private:
      Ref<Shader> m_shader;
      uint uuid;
      Ref<Texture2D> m_texture;
      float specularExponent = 10;
      float specularStrength = 0.5;
      float diffuseStrength = 0.5;
      Color color;

    public:
      Material(const Ref<Shader>& shader, const Ref<Texture2D>& texture);
      Material(const Ref<Shader>& shader);
      virtual ~Material();

      void Bind() const;
      void Unbind() const;
      void SetShader(const Ref<Shader>& shader);
      Material* SetSpecularExponent(float specularExponent) { this->specularExponent = specularExponent; return this; }
      Material* SetSpecularStrength(float specularStrength) { this->specularStrength = specularStrength; return this; }
      Material* SetDiffuseStrength(float diffuseStrength) {this->diffuseStrength = diffuseStrength; return this;}
      Material* SetColor(const Color& color) { this->color = color; return this; }

      inline float GetSpecularExponent() const { return specularExponent; }
      inline float GetSpecularStrength() const { return specularStrength; }
      inline float GetDiffuseStrength() const { return diffuseStrength; }
      inline const Color& GetColor() const { return color; }
      inline const Ref<Shader>& GetShader() const { return m_shader; }
      inline Ref<Texture2D> GetTexture() const { return m_texture; }
      friend bool operator<(const Material& m1, const Material& m2) { return m1.uuid < m2.uuid;}
    private:
      void UpdateTexture();
  };

}
