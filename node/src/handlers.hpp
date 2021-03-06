#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include "common.hpp"

namespace nacapp {

// Validation functions. They are called before interest handlers, if
// something is wrong, the validator should throw a string explaining the error.
using InterestValidator = void (*)(const Interest&);

// Post-process data before they are sent. These functions are called after the interest handlers.
using DataProcessor = void (*)(shared_ptr<Data>);

// Process received data
// using DataReceiver = void (*)(const Data &);
using DataReceiver = std::function<void(const Data&)>;

// express an interest. Data parameter for @p DataReceiver may be an application Nack.
// using InterestShower = void (*)(void(const Interest &, DataReceiver));
// typedef std::function<void(const Interest &, DataReceiver)> InterestShower;
using InterestShower = std::function<void(const Interest&, DataReceiver)>;

using PutData = std::function<void(shared_ptr<Data> d)>;

/**
 * Application interest handler
 * @p path is the name components after the application namespace. 
 * @p args is the name components in between @p path and signature components.
 * @return true if data is sent asynchronously
 */
using InterestHandler = std::function<
  bool(const Interest& interest, const Name& args, shared_ptr<Data> data, InterestShower show, PutData put)>;

} // nacapp
#endif /* HANDLERS_HPP */
