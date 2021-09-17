#include "Shader.h"

#include <graphics/shaders/ShaderFactory.h>
#include <internal/GreetGL.h>
#include <utils/ErrorHandle.h>
#include <utils/FileUtils.h>
#include <utils/HotSwapping.h>

#include <fstream>
#include <sstream>

namespace Greet {

  static const uint SHADER_VERTEX = 0;
  static const uint SHADER_FRAGMENT = 1;
  static const uint SHADER_GEOMETRY = 2;

  Shader::Shader(const std::string& filename)
    : Resource(filename), m_shaderID{Load(filename)}, uniforms{GetUniforms(m_shaderID)}
  {}

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
    : m_shaderID{Load(vertSrc, fragSrc, geomSrc, geomSrc.size() != 0)}, uniforms{GetUniforms(m_shaderID)}
  {}

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
    : m_shaderID{Load(vertSrc,fragSrc)}, uniforms{GetUniforms(m_shaderID)}
  {}

  Shader::~Shader()
  {
    GLCall(glDeleteProgram(m_shaderID));
  }

  void Shader::ReloadResource()
  {
    if(m_shaderID)
    {
      std::array<std::pair<std::string, bool>, 3> ret = ReadFile(filePath);
      uint vertShader = CompileShader(m_shaderID, ret[0].first, GL_VERTEX_SHADER, false);
      if(!vertShader)
        return;
      uint fragShader = CompileShader(m_shaderID, ret[1].first, GL_FRAGMENT_SHADER, false);
      if(!fragShader)
        return;
      uint geomShader = 0;
      if(ret[2].second)
      {
        geomShader = CompileShader(m_shaderID, ret[2].first, GL_GEOMETRY_SHADER, false);
        if(!geomShader)
          return;
      }
      GLCall(uint program = glCreateProgram());
      AttachShader(program, vertShader);
      AttachShader(program, fragShader);
      if(geomShader)
        AttachShader(program, geomShader);
      GLCall(glLinkProgram(program));
      int resultFlag;
      GLCall(glGetProgramiv(program, GL_LINK_STATUS, &resultFlag));
      if (resultFlag == GL_FALSE)
      {
        GLint length;
        GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> error(length);
        GLCall(glGetProgramInfoLog(program, length, &length, &error[0]));
        Log::Error("Failed to link shader!\n", &error[0]);
        return;
      }
      GLCall(glValidateProgram(program));
      uint oldProgram = m_shaderID;
      m_shaderID = program;
      MoveUniforms(program, oldProgram);
      GLCall(glDeleteProgram(oldProgram));
      Log::Info("Reloaded Shader: ", filePath);
    }
    else
      Log::Error("Invalid pointer");
  }

  void Shader::MoveUniforms(uint program, uint oldProgram)
  {
    std::set<UniformData> oldUniforms = GetListOfUniforms(oldProgram);
    uniforms = GetUniforms(program);

    std::set<UniformData> newUniforms = GetListOfUniforms(program);
    GLint numActiveUniforms = 0;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    std::vector<GLchar> nameData(256);
    Enable();
    for(auto it = newUniforms.begin(); it != newUniforms.end();++it)
    {
      auto found = oldUniforms.find(UniformData{it->name, 1, 0, -1});
      if(found != oldUniforms.end())
      {
        // Type has changed
        if(it->type != found->type || it->arraySize != found->arraySize)
          continue;
        if(it->type == GL_FLOAT)
        {
          if(it->arraySize == 1)
          {
            float f;
            GLCall(glGetUniformfv(oldProgram, found->location, &f));
            SetUniform1f(it->name, f);
          }
          else
          {
            float values[it->arraySize];
            // For some reason GetnUniforms doesn't work
            // So you have to query each element individually...
            for(int i = 0;i<it->arraySize;i++)
            {
              GLCall(int location = glGetUniformLocation(program,
                    std::string(found->name + "["+std::to_string(i)+"]").c_str()));
              GLCall(glGetUniformfv(oldProgram, location, &values[i]));
            }
            SetUniform1fv(it->name, it->arraySize, values);
          }
        }
        else if(it->type == GL_FLOAT_VEC2)
        {
          Vec2f f;
          GLCall(glGetnUniformfv(oldProgram, found->location, sizeof(Vec2f), (float*)&f));
          SetUniform2f(it->name, f);
        }
        else if(it->type == GL_FLOAT_VEC3)
        {
          Vec3f f;
          GLCall(glGetnUniformfv(oldProgram, found->location, sizeof(Vec3f), (float*)&f));
          SetUniform3f(it->name, f);
        }
        else if(it->type == GL_FLOAT_VEC4)
        {
          Vec4f f;
          GLCall(glGetnUniformfv(oldProgram, found->location, sizeof(Vec4f), (float*)&f));
          SetUniform4f(it->name, f);
        }
        else if(it->type == GL_FLOAT)
        {
          Vec4f f;
          GLCall(glGetnUniformfv(oldProgram, found->location, sizeof(Vec4f), (float*)&f));
          SetUniform4f(it->name, f);
        }
        else if(it->type == GL_INT ||
            it->type == GL_SAMPLER_1D ||
            it->type == GL_SAMPLER_2D ||
            it->type == GL_SAMPLER_3D ||
            it->type == GL_SAMPLER_CUBE)
        {
          if(it->arraySize == 1)
          {
            int i;
            GLCall(glGetUniformiv(oldProgram, found->location, &i));
            SetUniform1i(it->name, i);
          }
          else
          {
            int values[it->arraySize];
            // For some reason GetnUniforms doesn't work
            // So you have to query each element individually...
            for(int i = 0;i<it->arraySize;i++)
            {
              GLCall(int location = glGetUniformLocation(program,
                    std::string(found->name + "["+std::to_string(i)+"]").c_str()));
              GLCall(glGetUniformiv(oldProgram, location, &values[i]));
            }
            SetUniform1iv(it->name, it->arraySize, values);
          }
        }
        else if(it->type == GL_UNSIGNED_INT)
        {
          if(it->arraySize == 1)
          {
            uint i;
            GLCall(glGetUniformuiv(oldProgram, found->location, &i));
            SetUniform1ui(it->name, i);
          }
          else
          {
            uint values[it->arraySize];
            // For some reason GetnUniforms doesn't work
            // So you have to query each element individually...
            for(int i = 0;i<it->arraySize;i++)
            {
              GLCall(int location = glGetUniformLocation(program,
                    std::string(found->name + "["+std::to_string(i)+"]").c_str()));
              GLCall(glGetUniformuiv(oldProgram, location, &values[i]));
            }
            SetUniform1uiv(it->name, it->arraySize, values);
          }
        }
        else if(it->type == GL_FLOAT_MAT3)
        {
          Mat3 mat;
          GLCall(glGetnUniformfv(oldProgram, found->location, sizeof(Mat3), mat.elements));
          SetUniformMat3(it->name, mat);
        }
        else if(it->type == GL_FLOAT_MAT4)
        {
          Mat4 mat;
          GLCall(glGetnUniformfv(oldProgram, found->location, sizeof(Mat4), mat.elements));
          SetUniformMat4(it->name, mat);
        }
      }
    }

  }

  std::array<std::pair<std::string, bool>, 3> Shader::ReadFile(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.good())
    {
      Log::Error("Shader::FromFile Couldn't find shader in path \"%s\"", filename);
      return {
        std::pair{ShaderFactory::shaderErrorVert, true},
        {ShaderFactory::shaderErrorFrag, true},
        {"", false}};
    }
    return ReadStream(file);
  }

  std::array<std::pair<std::string, bool>, 3> Shader::ReadStream(std::istream& stream)
  {
    std::stringstream ss[3];
    bool hasWritten[3] = {false, false, false};

    std::string line;
    uint shader = SHADER_VERTEX;
    while (std::getline(stream, line))
    {
      if (line == "//vertex" || line == "#shader vertex")
      {
        shader = SHADER_VERTEX;
        ReadLineToStringStream(ss, "", shader);
      }
      else if (line == "//fragment" || line == "#shader fragment")
      {
        shader = SHADER_FRAGMENT;
        ReadLineToStringStream(ss, "", shader);
      }
      else if (line == "//geometry" || line == "#shader geometry")
      {
        shader = SHADER_GEOMETRY;
        ReadLineToStringStream(ss, "", shader);
      }
      else
      {
        hasWritten[shader] = true;
        ReadLineToStringStream(ss, line, shader);
      }
    }
    return {std::pair{ss[0].str(), hasWritten[0]}, {ss[1].str(), hasWritten[1]},{ss[2].str(), hasWritten[2]}};
  }

  void Shader::ReadLineToStringStream(std::stringstream ss[3], const std::string& line, uint shaderIndex)
  {
    ss[shaderIndex] << line << std::endl;

    // Write newlines so that the line number in errors are correct
    ss[(shaderIndex + 1) % 3] << std::endl;
    ss[(shaderIndex + 2) % 3] << std::endl;
  }

  uint Shader::Load(const std::string& filename)
  {
    std::array<std::pair<std::string, bool>,3> ret =  ReadFile(filename);
    return Load(ret[0].first, ret[1].first, ret[2].first, ret[2].second);
  }

  uint Shader::Load(const std::string& vertSrc, const std::string& fragSrc)
  {
    return Load(vertSrc, fragSrc, "", false);
  }

  uint Shader::Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry)
  {
    GLCall(uint program = glCreateProgram());

    uint shader = CompileAttachShader(program, vertSrc, GL_VERTEX_SHADER, true);
    if (!shader)
      return LoadError(program);
    shader = CompileAttachShader(program, fragSrc, GL_FRAGMENT_SHADER, true);
    if (!shader)
      return LoadError(program);
    if (hasGeometry)
    {
      shader = CompileAttachShader(program, geomSrc, GL_GEOMETRY_SHADER, true);
      if(!shader)
        return LoadError(program);
    }
    GLCall(glLinkProgram(program));
    int resultFlag;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &resultFlag));
    if (resultFlag == GL_FALSE)
    {
      GLint length;
      GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> error(length);
      GLCall(glGetProgramInfoLog(program, length, &length, &error[0]));
      Log::Error("Failed to link shader!\n", &error[0]);
      return LoadError(program);
    }
    GLCall(glValidateProgram(program));

    return program;
  }

  uint Shader::LoadError(uint program)
  {
    glDeleteProgram(program);
    GLCall(program = glCreateProgram());

    CompileAttachShader(program, ShaderFactory::shaderErrorVert, GL_VERTEX_SHADER, true);
    CompileAttachShader(program, ShaderFactory::shaderErrorFrag, GL_FRAGMENT_SHADER, true);
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    return program;
  }

  std::map<std::string, int> Shader::GetUniforms(uint program) const
  {
    std::map<std::string, int> uniforms;
    GLint numActiveUniforms = 0;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    char name[256];
    for(int i= 0; i < numActiveUniforms; ++i)
    {
      GLint arraySize = 0;
      GLenum type = 0;
      GLsizei actualLength = 0;
      GLCall(glGetActiveUniform(program, i, sizeof(name), &actualLength, &arraySize, &type, name));

      // For some reason arrays return 'arrayName[0]'
      for(int j = 0;j<actualLength;j++)
      {
        if(name[j] == '[')
        {
          name[j] = '\0';
          break;
        }
      }
      // i != glGetUniformLocation(program, name) sometimes for some reason
      GLCall(int location = glGetUniformLocation(program, name));
      uniforms.emplace(name, location);
    }
    return uniforms;
  }

  std::set<UniformData> Shader::GetListOfUniforms(uint program) const
  {
    std::set<UniformData> uniforms;
    GLint numActiveUniforms = 0;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    char name[256];
    for(int i= 0; i < numActiveUniforms; ++i)
    {
      GLint arraySize = 0;
      GLenum type = 0;
      GLsizei actualLength = 0;
      GLCall(glGetActiveUniform(program, i, sizeof(name), &actualLength, &arraySize, &type, name));

      // For some reason arrays return 'arrayName[0]'
      for(int j = 0;j<actualLength;j++)
      {
        if(name[j] == '[')
        {
          name[j] = '\0';
          break;
        }
      }
      GLCall(int location = glGetUniformLocation(program, name));
      uniforms.emplace(UniformData{name, arraySize, type, location});
    }
    return uniforms;
  }

  uint Shader::CompileAttachShader(uint program, const std::string& shaderSrc, uint shaderType, bool safeFail)
  {
    uint shader = CompileShader(program, shaderSrc, shaderType, safeFail);
    if(shader == 0)
      return 0;
    AttachShader(program, shader);
    return 1;
  }

  uint Shader::CompileShader(uint program, const std::string& shaderSrc, uint shaderType, bool safeFail)
  {
    GLCall(uint shader = glCreateShader(shaderType));
    const char* src = shaderSrc.c_str();
    GLCall(glShaderSource(shader, 1, &src, NULL));
    GLCall(glCompileShader(shader));

    GLint resultFrag;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &resultFrag));
    if (resultFrag == GL_FALSE)
    {
      GLint length;
      GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> error(length);
      GLCall(glGetShaderInfoLog(shader, length, &length, &error[0]));
      if (shaderType == GL_FRAGMENT_SHADER)
      {
        Log::Error("Failed to compile fragment Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_FRAGMENT);
        if(safeFail)
        {
          // Should never fail
          GLCall(glShaderSource(shader, 1, &ShaderFactory::shaderErrorFrag, NULL));
        }
        else
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      else if (shaderType == GL_VERTEX_SHADER)
      {
        Log::Error("Failed to compile vertex Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_VERTEX);
        if(safeFail)
        {
          // Should never fail
          GLCall(glShaderSource(shader, 1, &ShaderFactory::shaderErrorVert, NULL));
        }
        else
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      else if (shaderType == GL_GEOMETRY_SHADER)
      {
        Log::Error("Failed to compile geometry Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_GEOMETRY);
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      else
      {
        Log::Error("Failed to compile unknown shader!\n", &error[0]);
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      GLCall(glCompileShader(shader));
    }
    return shader;
  }



  void Shader::AttachShader(uint program, uint shader)
  {
    GLCall(glAttachShader(program, shader));
    GLCall(glDeleteShader(shader));
  }

  void Shader::BindAttributeOutput(uint attachmentId, const std::string& name) const
  {
    GLCall(glBindFragDataLocation(m_shaderID,attachmentId,name.c_str()));
  }

  int Shader::GetUniformLocation(const std::string& name) const
  {
    auto it = uniforms.find(name);
    if(it != uniforms.end())
      return it->second;
    return -1;
  }

  void Shader::SetUniformBoolean(const std::string& name, bool value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value ? 1.0f : 0.0f));
  }

  void Shader::SetUniform1f(const std::string& name, float value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1fv(const std::string& name, uint count, float* value) const
  {
    GLCall(glUniform1fv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1i(const std::string& name, int value) const
  {
    GLCall(glUniform1i(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1iv(const std::string& name, uint count, int* value) const
  {
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1ui(const std::string& name, uint value) const
  {
    GLCall(glUniform1ui(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1uiv(const std::string& name, uint count, uint* value) const
  {
    GLCall(glUniform1uiv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform2f(const std::string& name, const Vec2f& value) const
  {
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
  }

  void Shader::SetUniform3f(const std::string& name, const Vec3f& value) const
  {
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
  }

  void Shader::SetUniform4f(const std::string& name, const Vec4f& value) const
  {
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
  }

  void Shader::SetUniformColor3(const std::string& name, const Color& value) const
  {
    GLCall(glUniform3f(GetUniformLocation(name), value.r, value.g, value.b));
  }

  void Shader::SetUniformColor4(const std::string& name, const Color& value) const
  {
    GLCall(glUniform4f(GetUniformLocation(name), value.r, value.g, value.b, value.a));
  }

  void Shader::SetUniformMat3(const std::string& name, const Mat3& value) const
  {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  void Shader::SetUniformMat4(const std::string& name, const Mat4& value) const
  {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  bool Shader::operator<(const Shader& s)
  {
    return m_shaderID < s.m_shaderID;
  }

  void Shader::Enable() const
  {
    GLCall(glUseProgram(m_shaderID));
  }

  void Shader::Disable()
  {
    GLCall(glUseProgram(0));
  }

  int Shader::GetAttributeLocation(const std::string& name) const
  {
    GLCall(int location = glGetAttribLocation(m_shaderID, name.c_str()));
    return location;
  }

  Ref<Shader> Shader::FromFile(const std::string& shaderPath)
  {
    return NewRef<Shader>(shaderPath);
  }

  Ref<Shader> Shader::FromFile(const std::string& vertPath, const std::string& fragPath)
  {
    std::string vertSourceString = FileUtils::ReadFile(vertPath.c_str());
    std::string fragSourceString = FileUtils::ReadFile(fragPath.c_str());
    return NewRef<Shader>(vertSourceString, fragSourceString);
  }


  Ref<Shader> Shader::FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
  {
    std::string vertSourceString = FileUtils::ReadFile(vertPath.c_str());
    std::string fragSourceString = FileUtils::ReadFile(fragPath.c_str());
    std::string geomSourceString = FileUtils::ReadFile(geomPath.c_str());
    return NewRef<Shader>(vertSourceString,fragSourceString,geomSourceString);
  }

  Ref<Shader> Shader::FromSource(const std::string& shaderSrc)
  {
    std::stringstream shaderStream{shaderSrc};
    std::array<std::pair<std::string, bool>, 3> ss = ReadStream(shaderStream);
    if(ss[2].second)
      return NewRef<Shader>(ss[0].first, ss[1].first, ss[2].first);
    else
      return NewRef<Shader>(ss[0].first, ss[1].first);
  }

  Ref<Shader> Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc)
  {
    return NewRef<Shader>(vertSrc, fragSrc);
  }

  Ref<Shader> Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
  {
    return NewRef<Shader>(vertSrc, fragSrc, geomSrc);
  }
}
