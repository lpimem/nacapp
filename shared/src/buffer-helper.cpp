#include "buffer-helper.hpp"

namespace nacapp{

ndn::ConstBufferPtr
fromString(std::string buf){
  return std::make_shared<ndn::Buffer>(reinterpret_cast<const uint8_t*>(buf.data()), buf.size());
}

}