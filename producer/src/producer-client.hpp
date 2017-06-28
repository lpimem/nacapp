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
    , m_producer(gepProd)
  {
  }

public:
  // TODO:
  // void
  // bootstrap(OnSuccess&, OnFail&);

  // return the underlying application node object
  shared_ptr<Node>
  getNode()
  {
    return m_node;
  }

  shared_ptr<Producer>
  getNACProducer()
  {
    return m_producer;
  }

  // start listening for incoming interests
  void
  serve();

private:
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
  shared_ptr<Producer> m_producer;
  // std::map<string, shared_ptr<
};

} // producer
} // nacapp
#endif /* PRODUCER_CLIENT_HPP */
