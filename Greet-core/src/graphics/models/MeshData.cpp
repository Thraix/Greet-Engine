#include "MeshData.h"

#include <graphics/models/MeshFactory.h>
#include <logging/Log.h>

#include <fstream>
#include <set>
#include <map>

namespace Greet {

  MeshData::MeshData(const Pointer<Vec3f>& vertices, const Pointer<uint32_t>& indices)
    : m_vertices(vertices), m_indices(indices)
  {
    for(auto it = m_indices.begin(); it != m_indices.end(); ++it)
    {
      if(*it >= m_vertices.Size())
      {
        Log::Error("Index out of bound in MeshData creation");
        *it = 0;
      }
    }
  }

  const std::pair<BufferAttribute, Pointer<char>>* MeshData::GetAttribute(int location) const
  {
    for (auto it = m_data.begin(); it != m_data.end(); it++)
    {
      if ((*it).first.location == location)
      {
        return &*it;
      }
    }
    return nullptr;
  }

  std::pair<BufferAttribute, Pointer<char>>* MeshData::RemoveAttribute(int location)
  {
    for (auto it = m_data.begin(); it != m_data.end(); it++)
    {
      if ((*it).first.location == location)
      {
        m_data.erase(it);
        return &*it;
      }
    }
    return nullptr;
  }

  void MeshData::GenerateNormals()
  {
    Pointer<Vec3f> normals = MeshFactory::CalculateNormals(m_vertices, m_indices);
    AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
  }

  MeshData* MeshData::LowPolify()
  {
    std::set<uint32_t> usedProvokingVertices;
    std::map<uint32_t,uint32_t> usedIndices;
    std::vector<Vec3f> newVertices;
    std::vector<uint32_t> newIndices;

    // Loop through all triangles
    for(uint32_t i = 0;i<GetIndexCount();i+=3)
    {
      bool foundProvoking = true;
      uint32_t provokingOffset = 0;

      // Check if the first vertex has been used as a provoking vertex
      if(usedProvokingVertices.count(m_indices[i]) == 0)
      {
        provokingOffset = 0;
      }
      else if(usedProvokingVertices.count(m_indices[i+1]) == 0)
      {
        provokingOffset = 1;
      }
      else if(usedProvokingVertices.count(m_indices[i+2]) == 0)
      {
        provokingOffset = 2;
      }
      else
      {
        foundProvoking = false;
      }

      // First (provoking) vertex
      if(foundProvoking)
      {
        uint32_t provokingIndex = m_indices[i+provokingOffset];
        Vec3f provokingVertex = m_vertices[provokingIndex];
        usedProvokingVertices.emplace(provokingIndex);

        auto vertex = usedIndices.find(provokingIndex);
        if(!foundProvoking || vertex == usedIndices.end())
        {
          newIndices.push_back(newVertices.size());
          usedIndices.emplace(provokingIndex, newVertices.size());
          newVertices.push_back(provokingVertex);
        }
        else
        {
          newIndices.push_back(usedIndices.find(provokingIndex)->second);
        }
      }
      else
      {

        newIndices.push_back(newVertices.size());
        newVertices.push_back(m_vertices[m_indices[i]]);
      }

      // Second vertex
      uint32_t index = m_indices[i+((provokingOffset+1)%3)];
      auto vertex = usedIndices.find(index);
      if(vertex == usedIndices.end())
      {
        newIndices.push_back(newVertices.size());
        usedIndices.emplace(index, newVertices.size());
        newVertices.push_back(m_vertices[index]);
      }
      else
      {
        newIndices.push_back(usedIndices.find(index)->second);
      }

      // Third vertex
      index = m_indices[i+((provokingOffset+2)%3)];
      vertex = usedIndices.find(index);
      if(vertex == usedIndices.end())
      {
        newIndices.push_back(newVertices.size());
        usedIndices.emplace(index, newVertices.size());
        newVertices.push_back(m_vertices[index]);
      }
      else
      {
        newIndices.push_back(usedIndices.find(index)->second);
      }
    }

    return new MeshData(newVertices, newIndices);
  }

  void MeshData::WriteToFile(const std::string& filename) const
  {
    std::ofstream fout(filename);

    // Write signature
    fout.write("MESH",4*sizeof(char));

    uint32_t vertexCount = GetVertexCount();
    uint32_t indexCount = GetIndexCount();
    size_t dataCount = m_data.size();

    // Write how many vertices we have
    fout.write((char*)&vertexCount,sizeof(uint32_t));
    // Write how many indices we have
    fout.write((char*)&indexCount,sizeof(uint32_t));
    // Write how many attributes we have
    fout.write((char*)&dataCount,sizeof(size_t));

    // Write the different data of the mesh
    for(auto it = m_data.begin();it != m_data.end(); ++it)
    {
      fout.write((char*)&(*it).first.location,sizeof(uint32_t));
      fout.write((char*)&(*it).first.type,sizeof(uint32_t));
      fout.write((char*)&(*it).first.normalize,sizeof(bool));
    }

    // Write all vertex data.
    fout.write((char*)m_vertices.Data(), vertexCount * sizeof(Vec3f));

    // Write index data.
    fout.write((char*)m_indices.Data(), indexCount * sizeof(uint32_t));


    // Write all attribute data
    for(auto it = m_data.begin();it != m_data.end(); ++it)
    {
      fout.write((char*)(*it).second.Data(), BufferAttributeToByteSize((*it).first.type) * vertexCount);
    }
    fout.close();
  }

  MeshData MeshData::ReadFromFile(const std::string& filename)
  {
    // Small buffer for initial stuffs
    char buffer[1024];
    std::ifstream fin(filename);

    // Determine size of the file and reset to beginning.
    int fileSize = fin.tellg();
    fin.seekg(0,std::ios::end);
    fileSize = (uint32_t)fin.tellg() - fileSize;
    fin.seekg(0,std::ios::beg);

    // Check if the file is big enough to contain signature, vertex count, index count and attribute count
    fileSize -= 4 * sizeof(char) + 2 * sizeof(uint32_t) + sizeof(size_t);
    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain signature.");
      return MeshFactory::Cube();
    }

    // Read signature
    fin.read(buffer, 4 * sizeof(char));

    if(std::string(buffer, 4) != "MESH")
    {
      Log::Error("Could not read MESH file, signature invalid.");
      return MeshFactory::Cube();
    }
    // Read how many attributes we have.
    uint32_t vertexCount;
    fin.read((char*)&vertexCount,sizeof(uint32_t));
    fileSize -= vertexCount * sizeof(Vec3f);
    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain vertex data");
      return MeshFactory::Cube();
    }

    // Read how many attributes we have.
    uint32_t indexCount;
    fin.read((char*)&indexCount,sizeof(uint32_t));
    fileSize -= indexCount * sizeof(uint32_t);
    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain vertex data");
      return MeshFactory::Cube();
    }

    // Read how many attributes we have.
    size_t attributeCount;
    fin.read((char*)&attributeCount,sizeof(size_t));

    // Check if the file is big enough to contain attribute parameters.
    int attribLength = (sizeof(uint32_t) * 2 + sizeof(bool));
    int attribsLength = attributeCount * attribLength;

    // Remove attribLength from fileSize to easier check sizes later.
    fileSize -= attribsLength;

    if(fileSize < 0)
    {
      Log::Error("Could not read MESH file, file is too small to contain attribute parameters");
      return MeshFactory::Cube();
    }

    std::vector<BufferAttribute> attributeParameters;

    if(attribLength > 1024)
    {
      Log::Error("Could not read MESH file, too many attributes.");
      return MeshFactory::Cube();
    }
    fin.read(buffer,attribsLength);

    const char* pointer = buffer;
    for(size_t i = 0;i<attributeCount;i++)
    {
      // Read uints
      int location = ((int*)pointer)[0];
      uint32_t bufferType = ((uint32_t*)pointer)[1];

      // Move pointer
      pointer += sizeof(uint32_t)*4;

      // Read bool
      bool normalize = ((bool*)buffer)[0];

      // Move pointer
      pointer += sizeof(bool);

      attributeParameters.push_back({location, (BufferAttributeType)bufferType, normalize});
      fileSize -= BufferAttributeToByteSize((BufferAttributeType)bufferType) * vertexCount;
      if(fileSize < 0)
      {
        Log::Error("Could not read MESH file, file is too small to contain attribute data");
        return MeshFactory::Cube();
      }
    }
    if(fileSize != 0)
      Log::Warning("MESH file is larger than expected. Something might be wrong...");

    // Read vertices
    std::vector<Vec3f> vertices = std::vector<Vec3f>(vertexCount);
    fin.read((char*)vertices.data(),vertexCount*sizeof(Vec3f));

    // Read indices
    std::vector<uint32_t> indices = std::vector<uint32_t>(indexCount);
    fin.read((char*)indices.data(),indexCount*sizeof(uint32_t));

    MeshData meshData{vertices,indices};

    // Read attribute data
    for(auto it = attributeParameters.begin();it!=attributeParameters.end();++it)
    {
      uint32_t memSize = BufferAttributeToByteSize(it->type);
      Pointer<char> data(memSize * vertexCount);
      fin.read(data.Data(), memSize * vertexCount);
      meshData.AddAttribute(*it, data);
    }

    fin.close();
    return meshData;
  }
}
