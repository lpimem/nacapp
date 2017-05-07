
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

  impl.route(reqPath.toUri(), [](const Interest &interest, const Name &args,
                                 shared_ptr<Data> data) {},
             {}, {});

  auto keyChain = initKeyChain(prefix);

  SECTION("parse interest name ") {
    Name req(prefix);
    req.append(reqPath);
    req.append(reqArgs);
    Interest interest(req);

    // LOG(INFO) << "interest: " << interest.toUri();
    // LOG(INFO) << interest.getName().size();

    // Something doesn't feel correct here
    // According to this page,
    //   http://named-data.net/doc/ndn-cxx/current/tutorials/signed-interest.html
    // Signed interested should have 4 more components
    // But here we are only getting 2 more.
    keyChain->sign(interest);

    // LOG(INFO) << "signed interest: " << interest.toUri();
    // LOG(INFO) << interest.getName().size();

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