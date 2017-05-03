#include "service.hpp"

namespace nacapp {

void Service::serveForever() {
  registerPrefixes();
  m_face.processEvents();
}

void Service::registerPrefixes() {
  //
}

void Service::onRegisterPrefixFailed(const Name &prefix, const string reason) {
  // todo
}

void Service::onInterest(const ndn::InterestFilter &filter,
                         const Interest &interest) {
  // todo
}

void Service::onAddIdentity(const Interest &interest) {
  // todo;
}
} // nacapp