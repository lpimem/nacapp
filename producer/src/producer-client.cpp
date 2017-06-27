#include "producer-client.hpp"

namespace nacapp {
namespace producer {


void
ProducerClient::registerPrefixes()
{
  // m_node->route(NAME_COMPONENT_SAMPLES,
  //               [this](const Interest& interest,
  //                      const Name& args,
  //                      shared_ptr<Data> data,
  //                      InterestShower show) {
  //                 if (args.get(2) == NAME_COMPONENT_C_KEY) {
  //                   this->onGetKey(interest, args, data, show);
  //                 }
  //                 else {
  //                   this->onGetContent(interest, args, data, show);
  //                 }
  //               });
}

void
ProducerClient::onGetContent(const Interest& interest,

                             const Name& args,
                             shared_ptr<Data> data,
                             InterestShower show)
{
}

void
ProducerClient::onGetKey(const Interest& interest,

                         const Name& args,
                         shared_ptr<Data> data,
                         InterestShower show)
{
}


} // producer
} // nacapp