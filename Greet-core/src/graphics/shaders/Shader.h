#pragma once

#include <common/Memory.h>
#include <graphics/Color.h>
#include <math/Maths.h>
#include <utils/Resource.h>

#include <string>
#include <map>
#include <set>

namespace Greet {

  struct UniformData
  {
    UniformData(const std::string& name, int arraySize, uint32_t type, int location)
      : name{name}, arraySize{arraySize}, type{type}, location{location}
    {}
    std::string name;
    int arraySize;
    uint32_t type;
    int location;

    friend bool operator<(const UniformData& u1, const UniformData& u2)
    {
      return u1.name < u2.name;
    }
  };

  class Shader final : public Resource
  {
    private:
      uint32_t m_shaderID;
      std::map<std::string, int> uniforms;

    public:
      Shader(const std::string& filename);
      Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);
      Shader(const std::string& vertSrc, const std::string& fragSrc);

    private:
      uint32_t Load(const std::string& filename);
      uint32_t Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry);
      uint32_t Load(const std::string& vertSrc, const std::string& fragSrc);
      uint32_t LoadError(uint32_t program);
      uint32_t CompileAttachShader(uint32_t program, const std::string& shaderSrc, uint32_t shaderType, bool safeFail);
      uint32_t CompileShader(uint32_t program, const std::string& shaderSrc, uint32_t shaderType, bool safeFail);
      void AttachShader(uint32_t program, uint32_t shader);
      int GetUniformLocation(const std::string& name) const;
      std::map<std::string, int> GetUniforms(uint32_t program) const;
      std::set<UniformData> GetListOfUniforms(uint32_t program) const;
      void MoveUniforms(uint32_t program, uint32_t oldProgram);

    public:
      ~Shader();

      void ReloadResource() override;
      void Enable() const;
      static void Disable();
      uint32_t GetProgram() const { return m_shaderID; }

      int GetAttributeLocation(const std::string& name) const;
      void BindAttributeOutput(uint32_t attachmentId, const std::string& name) const;

      void SetUniformBoolean(const std::string& name, bool value) const;
      void SetUniform1f(const std::string& name, float value) const;
      void SetUniform1fv(const std::string& name, uint32_t count, float* value) const;
      void SetUniform1i(const std::string& name, int value) const;
      void SetUniform1iv(const std::string& name, uint32_t count, int* value) const;
      void SetUniform1ui(const std::string& name, uint32_t value) const;
      void SetUniform1uiv(const std::string& name, uint32_t count, uint32_t* values) const;
      void SetUniform2f(const std::string& name, const Vec2f& value) const;
      void SetUniform3f(const std::string& name, const Vec3f& value) const;
      void SetUniform4f(const std::string& name, const Vec4f& value) const;
      void SetUniformColor3(const std::string& name, const Color& value) const;
      void SetUniformColor4(const std::string& name, const Color& value) const;
      void SetUniformMat3(const std::string& name, const Mat3 &value) const;
      void SetUniformMat4(const std::string& name, const Mat4 &value) const;
      bool operator<(const Shader& s);
    private:
      static std::array<std::pair<std::string, bool>, 3> ReadFile(const std::string& filename);
      static std::array<std::pair<std::string, bool>, 3> ReadStream(std::istream& stream);
      static void ReadLineToStringStream(std::stringstream ss[3], const std::string& line, uint32_t shaderIndex);
    public:
      static Ref<Shader> FromFile(const std::string& shaderPath);
      static Ref<Shader> FromFile(const std::string& vertPath, const std::string& fragPath);
      static Ref<Shader> FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
      static Ref<Shader> FromSource(const std::string& shaderSrc);
      static Ref<Shader> FromSource(const std::string& vertSrc, const std::string& fragSrc);
      static Ref<Shader> FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);

  };

}
