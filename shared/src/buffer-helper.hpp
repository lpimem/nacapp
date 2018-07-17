#ifndef NACAPP_BUFFER_HPP
#define NACAPP_BUFFER_HPP

#include <memory>
#include <string>

#include <ndn-cxx/encoding/buffer.hpp>

namespace nacapp {

extern ndn::ConstBufferPtr
fromString(std::string buf);

} // namespace nacapp

#endif