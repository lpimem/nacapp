#ifndef PRODUCER_CLIENT_HPP
#define PRODUCER_CLIENT_HPP


#include "common.hpp"

#include "../../node/src/node.hpp"


namespace nacapp {
namespace producer {

class ProducerClient
{
public:
  ProducerClient(string prefix, shared_ptr<Node> node, shared_ptr<Producer> gepProd)
    : m_prefix(prefix)
    , m_node(node)
    , m_prod(gepProd)
  {
  }

private:
  void
  registerPrefixes();

  // <prefix>/SAMPLES/<dataType>/[timestamp]
  // args:
  //    0: dataType
  //    1: timestamp
  void
  onGetContent(const Interest& interest,

               const Name& args,
               shared_ptr<Data> data,
               InterestShower show);

  // <prefix>/SAMPLES/<dataType>/C-KEY/<timestamp>
  // args:
  //    0: dataType
  //    2: timestamp
  void
  onGetKey(const Interest& interest,

           const Name& args,
           shared_ptr<Data> data,
           InterestShower show);

private:
  string m_prefix;
  shared_ptr<Node> m_node;
  shared_ptr<Producer> m_prod;
};

} // producer
} // nacapp
#endif /* PRODUCER_CLIENT_HPP */
