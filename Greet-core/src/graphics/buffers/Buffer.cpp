#include "Buffer.h"

#include <internal/GreetGL.h>

namespace Greet {

  Buffer::Buffer(uint32_t dataSize, BufferType type, BufferDrawType drawType)
    : id{0}, dataSize{dataSize}, type{type}, drawType{drawType}
  {
    GLCall(glGenBuffers(1, &id));
    Enable();
  }

  Buffer::~Buffer()
  {
    GLCall(glDeleteBuffers(1, &id));
  }

  void Buffer::Enable() const
  {
    GLCall(glBindBuffer((uint32_t)type, id));
  }

  void Buffer::UpdateData(const void* data) const
  {
    GLCall(glBufferData((uint32_t)type, dataSize, data, (uint32_t)drawType));
  }

  void Buffer::UpdateData(const void* data, uint32_t dataSize)
  {
    this->dataSize = dataSize;
    GLCall(glBufferData((uint32_t)type, dataSize, data, (uint32_t)drawType));
  }

  void* Buffer::MapBuffer() const
  {
    GLCall(void* buffer = glMapBuffer((uint32_t)type, GL_WRITE_ONLY));
    return buffer;
  }

  void Buffer::UnmapBuffer() const
  {
    GLCall(glUnmapBuffer((uint32_t)type));
  }

  void Buffer::Disable() const
  {
    GLCall(glBindBuffer((uint32_t)type, 0));
  }

  Ref<Buffer> Buffer::Create(uint32_t dataSize, BufferType type, BufferDrawType drawType)
  {
    return std::shared_ptr<Buffer>(new Buffer(dataSize, type, drawType));
  }
}
