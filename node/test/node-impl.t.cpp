#include <ndn-cxx/lp/nack.hpp>
#include <ndn-cxx/util/dummy-client-face.hpp>
#include <ndn-cxx/util/io.hpp>
#include <ndn-cxx/util/string-helper.hpp>


#include "../../shared/test/catch.hpp"
#include "../src/node-impl.hpp"

namespace nacapp {

namespace test {

using ndn::security::v2::KeyChain;
using ndn::util::DummyClientFace;


const std::uint32_t KEY_SIZE = 1024;
const ndn::RsaKeyParams RSA_KEY_PARAMS(KEY_SIZE, ndn::KeyIdType::SHA256);

class NodeImplTestFixture
{
public:
  NodeImplTestFixture()
  {
    keyChain->setDefaultIdentity(id);
  }
  Name prefix{"/PREFIX"};
  shared_ptr<KeyChain> keyChain = make_shared<KeyChain>();
  shared_ptr<DummyClientFace> face =
    make_shared<DummyClientFace>(*keyChain, DummyClientFace::Options{true, true});
  Identity id = keyChain->createIdentity(prefix, RSA_KEY_PARAMS);
  NodeImpl impl{prefix, face, keyChain};
};

TEST_CASE("NodeImple::route")
{
  NodeImplTestFixture fixture;
  const Name reqPath("/PATH/TO/SERVICE");
  const Name reqArgs("/ARG1/ARG2/ARG3");

  SECTION("serve local")
  {
    fixture.impl.route(reqPath.toUri(),
                       [](const Interest& interest,
                          const Name& args,
                          shared_ptr<Data> data,
                          InterestShower showInterest,
                          PutData put) {
                         uint8_t resp[] = {1};
                         size_t leng = 1;
                         data->setContent(resp, leng);
                         return false;
                       },
                       {},
                       {});

    fixture.impl.registerPrefixes();
    fixture.face->processEvents(time::milliseconds(10));

    Name req(fixture.prefix);
    req.append(reqPath);
    req.append(reqArgs);
    Interest interest(req, time::milliseconds(500));
    fixture.keyChain->sign(interest);

    // auto client = make_shared<Face>("localhost");
    auto client = fixture.face;
    LOG(INFO) << "express interest: " << interest.toUri();
    client->expressInterest(interest,
                            [&](const Interest& i, const Data& d) {
                              LOG(INFO) << "on data";
                              auto block = d.getContent();
                              LOG(INFO)
                                << "data: " << ndn::toHex(block.value(), block.value_size(), true);
                              REQUIRE(block.value_size() == 1);
                              const std::uint32_t val = *(block.value());
                              REQUIRE(val == 1);
                            },
                            [&](const Interest& i, const ndn::lp::Nack& n) {
                              LOG(ERROR) << "Unexpected NACK: " << n.getReason();
                              REQUIRE(false);
                            },
                            bind([] {
                              LOG(ERROR) << "Unexpected TIMEOUT";
                              REQUIRE(false);
                            }));
    CHECK_NOTHROW(client->processEvents(time::milliseconds(10)));
    CHECK_NOTHROW(fixture.face->processEvents(time::milliseconds(10)));
    CHECK_NOTHROW(client->processEvents(time::milliseconds(10)));
  }
}

TEST_CASE("NodeImple::parseInterestName")
{
  NodeImplTestFixture fixture;

  const Name reqPath("/PATH/TO/SERVICE");
  const Name reqArgs("/ARG1/ARG2/ARG3");

  fixture.impl.route(reqPath.toUri(),
                     [](const Interest& interest,
                        const Name& args,
                        shared_ptr<Data> data,
                        InterestShower show,
                        PutData put) { return false; },
                     {},
                     {});

  SECTION("parse correct interest name ")
  {
    Name req(fixture.prefix);
    req.append(reqPath);
    req.append(reqArgs);
    Interest interest(req);

    // Something doesn't feel correct here
    // According to this page,
    //   http://named-data.net/doc/ndn-cxx/current/tutorials/signed-interest.html
    // Signed interested should have 4 more components
    // But here we are only getting 2 more.
    // fixture.keyChain->sign(interest);
    // Oct 30, 2017: do not require signed interests

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
} // namespace test
} // namespace nacapp