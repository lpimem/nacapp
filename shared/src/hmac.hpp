#ifndef NACAPP_HMAC_HPP
#define NACAPP_HMAC_HPP

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;


// TODO: install/resolve crypto++ libs
#include "cryptlib.h"
using CryptoPP::Exception;

#include "hmac.h"
using CryptoPP::HMAC;

#include "sha.h"
using CryptoPP::SHA256;

#include "base64.h"
using CryptoPP::Base64Encoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;

extern std::string
sign_hmac(std::string key, std::string plain);
#endif