#ifndef NACAPP_BUFFER_HPP
#define NACAPP_BUFFER_HPP

#include <memory>
#include <string>

#include <ndn-cxx/encoding/buffer.hpp>

namespace nacapp {
ndn::ConstBufferPtr
fromString(std::string buf)
{
  return std::make_shared<ndn::Buffer>(reinterpret_cast<const uint8_t*>(buf.data()), buf.size());
}

} // namespace nacapp

#endif