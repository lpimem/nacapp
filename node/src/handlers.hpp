#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include "common.hpp"

namespace nacapp {

// application interest handlers. @p path is the name components after the
// application namespace. @p args is the name components after @p path but
// before the signature components.
using InterestHandler = void (*)(const Interest &interest, const Name &args,
                                 shared_ptr<Data> data);

// for common validation functions. They are called before interest handlers, if
// something is wrong, the validator should throw a string explaining the error.
using InterestValidator = void (*)(const Interest &);

// for common post-processing functions. They are called after the interest
// handlers.
using DataProcessor = void (*)(shared_ptr<Data>);

} // nacapp
#endif /* HANDLERS_HPP */
