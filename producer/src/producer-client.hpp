#ifndef PRODUCER_CLIENT_HPP
#define PRODUCER_CLIENT_HPP
#include "common.hpp"
namespace nacapp {
class ProducerClient {

public:
  void onInterestForData(const Interest &interst, const Name data,
                         shared_ptr<Data> data);

  void onInterestForCKey(const Interest &interst, const Name ckey,
                         shared_ptr<Data> data);
};

} // nacapp
#endif /* PRODUCER_CLIENT_HPP */
