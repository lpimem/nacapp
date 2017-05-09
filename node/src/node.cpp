#include "node.hpp"
namespace nacapp
{

void Node::serveForever() { impl.serveForever(); }

void Node::route(string path, InterestHandler handler,
                 vector<InterestValidator> validators,
                 vector<DataProcessor> processors)
{
  impl.route(path, handler, validators, processors);
}

void Node::route(string path, InterestHandler handler,
                 vector<InterestValidator> validators)
{
  impl.route(path, handler, validators, {});
}

void Node::route(string path, InterestHandler handler,
                 vector<DataProcessor> processors)
{
  impl.route(path, handler, {}, processors);
}

void Node::route(string path, InterestHandler handler)
{
  impl.route(path, handler, {}, {});
}

void Node::showInterest(const Interest &interest, DataReceiver proc)
{
  impl.showInterest(interest, proc);
}

} // nacapp