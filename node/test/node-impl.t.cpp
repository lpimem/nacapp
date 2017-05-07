
#include <ndn-cxx/util/dummy-client-face.hpp>

#include "../../shared/test/catch.hpp"
#include "../src/node-impl.hpp"

namespace nacapp {

namespace test {

using ndn::security::v2::KeyChain;

shared_ptr<KeyChain> initKeyChain(const Name &prefix) {
  auto kc = make_shared<KeyChain>();
  kc->createIdentity(prefix);
  return kc;
}

class NodeImplTestFixture {

public:
  Name prefix{"/PREFIX"};
  shared_ptr<Face> face = make_shared<Face>("localhost");
  shared_ptr<KeyChain> keyChain = initKeyChain(prefix);
  NodeImpl impl{prefix, face};
};

TEST_CASE("NodeImple::route") {
  NodeImplTestFixture fixture;
  const Name reqPath("/PATH/TO/SERVICE");
  const Name reqArgs("/ARG1/ARG2/ARG3");

  SECTION("serve local") {
    fixture.impl.route(
        reqPath.toUri(),
        [](const Interest &interest, const Name &args, shared_ptr<Data> data) {
          uint8_t resp[] = {1};
          size_t leng = 1;
          data->setContent(resp, leng);
        },
        {}, {});

    fixture.impl.registerPrefixes();
    fixture.face->processEvents(time::milliseconds(50));

    ndn::util::DummyClientFace client;

    Name req(fixture.prefix);
    req.append(reqPath);
    req.append(reqArgs);
    Interest interest(req, time::milliseconds(100));
    fixture.keyChain->sign(interest);
    client.expressInterest(interest,
                           [&](const Interest &i, const Data &d) {
                             LOG(INFO) << "on data";
                             auto buffer = d.getContent().getBuffer();
                             REQUIRE(buffer->size() == 1);
                             REQUIRE(*buffer->get() == 1);
                           },
                           bind([] {
                             LOG(ERROR) << "Unexpected NACK";
                             REQUIRE(false);
                           }),
                           bind([] {
                             LOG(ERROR) << "Unexpected TIMEOUT";
                             REQUIRE(false);
                           }));

    CHECK_NOTHROW(client.processEvents(time::milliseconds(10)));
    CHECK_NOTHROW(fixture.face->processEvents(time::milliseconds(50)));
    CHECK_NOTHROW(client.processEvents(time::milliseconds(10)));
  }
}

TEST_CASE("NodeImple::parseInterestName") {

  NodeImplTestFixture fixture;

  const Name reqPath("/PATH/TO/SERVICE");
  const Name reqArgs("/ARG1/ARG2/ARG3");

  fixture.impl.route(
      reqPath.toUri(),
      [](const Interest &interest, const Name &args, shared_ptr<Data> data) {},
      {}, {});

  SECTION("parse correct interest name ") {
    Name req(fixture.prefix);
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
    fixture.keyChain->sign(interest);

    // LOG(INFO) << "signed interest: " << interest.toUri();
    // LOG(INFO) << interest.getName().size();

    vector<Name> parts = fixture.impl.parseInterestName(interest);
    REQUIRE(parts.size() == 3);

    Name parsedPrefix = parts[0];
    Name parsedPath = parts[1];
    Name parsedArgs = parts[2];
    REQUIRE(parsedPrefix == fixture.prefix);
    REQUIRE(parsedPath == reqPath);
    REQUIRE(parsedArgs == reqArgs);
  }
}
} // test
} // nacapp