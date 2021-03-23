#include "OBJUtils.h"

#include <utils/StringUtils.h>
#include <graphics/models/MeshFactory.h>

#include <fstream>

namespace Greet {

  MeshData OBJUtils::ErrorModel()
  {
    return MeshFactory::Cube();
  }

  MeshData OBJUtils::LoadObj(const std::string& filename)
  {
    std::vector<std::string> dataLine;

    std::vector<Vec3f> positions;
    std::vector<Vec2f> texCoords;
    std::vector<Vec3f> normals;
    std::vector<uint32_t> indices;

    std::ifstream input(filename);
    if(!input.is_open())
    {
      Log::Error("Obj File not found: ", filename);
    }
    std::map<std::string, uint32_t> verticesMap;

    // These are the same length.
    std::vector<Vec3f> vertexPos;
    std::vector<Vec2f> vertexTexCoords;
    std::vector<Vec3f> vertexNormals;

    for (std::string line; getline(input, line); )
    {
      std::istringstream iss{line};
      std::string s;
      iss >> s;
      if (s == "v")
      {
        Vec3f vector{};
        iss >> vector.x;
        iss >> vector.y;
        iss >> vector.z;
        positions.push_back(vector);
      }
      else if (s == "vt")
      {
        Vec2f vector{};
        iss >> vector.x;
        iss >> vector.y;
        texCoords.push_back(vector);
      }
      else if (s == "vn")
      {
        Vec3f vector{};
        iss >> vector.x;
        iss >> vector.y;
        iss >> vector.z;
        normals.push_back(vector);
      }
      else if (s == "f")
      {
        dataLine = StringUtils::SplitString(line, " ");
        for(int i = 1; i<dataLine.size();i++)
        {
          if(dataLine[i] == "" || dataLine[i] == "\r")
            continue;
          auto it = verticesMap.find(dataLine[i]);
          if(it != verticesMap.end())
          {
            indices.push_back(it->second);
          }
          else
          {
            indices.push_back(vertexPos.size());
            verticesMap.emplace(dataLine[i], vertexPos.size());
            std::vector<std::string> vertex = StringUtils::SplitString(dataLine[i], "/");

            // Only position defined
            if(vertex.size() == 1)
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
            }
            else if(vertex.size() == 2) // Position and texcoord defined
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
              if(vertex[1] != "")
                vertexTexCoords.push_back(texCoords[atoi(vertex[1].c_str())-1]);
            }
            else if(vertex.size() == 3)
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
              if(vertex[1] != "")
                vertexTexCoords.push_back(texCoords[atoi(vertex[1].c_str())-1]);
              if(vertex[2] != "")
                vertexNormals.push_back(normals[atoi(vertex[2].c_str())-1]);
            }
          }
        }
      }
    }

    input.close();

    MeshData mesh{vertexPos, indices};
    if(vertexTexCoords.size() > 0)
      mesh.AddAttribute({MESH_TEXCOORDS_LOCATION, BufferAttributeType::VEC2}, Pointer<Vec2f>{vertexTexCoords});

    if(vertexNormals.size() > 0)
      mesh.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, Pointer<Vec3f>{vertexNormals});

    return mesh;
  }
}
