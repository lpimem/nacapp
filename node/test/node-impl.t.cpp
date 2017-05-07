
#include "../../shared/test/catch.hpp"
#include "../src/node-impl.hpp"

namespace nacapp {

namespace test {

using ndn::security::v2::KeyChain;

shared_ptr<KeyChain> initKeyChain(const Name &prefix) {
  auto kc = make_shared<KeyChain>("pib-memory", "tpm-memory");
  kc->createIdentity(prefix);
  return kc;
}

TEST_CASE("Node::parseInterestName") {

  const Name prefix("/PREFIX");
  auto face = make_shared<Face>("localhost");
  NodeImpl impl(prefix, face);

  const Name reqPath("/PATH/TO/SERVICE");
  const Name reqArgs("/ARG1/ARG2/ARG3");

  auto keyChain = initKeyChain(prefix);

  SECTION("parse interest name ") {
    Name req(prefix);
    req.append(reqPath);
    req.append(reqArgs);
    Interest interest(req);
    keyChain->sign(interest);

    vector<Name> parts = impl.parseInterestName(interest);
    REQUIRE(parts.size() == 3);

    Name parsedPrefix = parts[0];
    Name parsedPath = parts[1];
    Name parsedArgs = parts[2];
    REQUIRE(parsedPrefix == prefix);
    REQUIRE(parsedPath == reqPath);
    REQUIRE(parsedArgs == reqArgs);
  }
}
} // test
} // nacapp