#ifndef CONSUMER_CLIENT_HPP
#define CONSUMER_CLIENT_HPP
#include "common.hpp"
namespace nacapp {

class ConsumerClient {

public:
  void setIdentity(Name identity, SafeBag keycert);
  void consume(Name request, OnData onData, OnNack onNack);

private:
  ndn::security::v2::KeyChain("pib-memory", "tpm-memory");
};
} // nacapp
#endif /* CONSUMER_CLIENT_HPP */
