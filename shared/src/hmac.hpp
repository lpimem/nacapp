#ifndef NACAPP_HMAC_HPP
#define NACAPP_HMAC_HPP

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <string>
using std::string;


#include <cryptopp/cryptlib.h>
using CryptoPP::Exception;

#include <cryptopp/hmac.h>
using CryptoPP::HMAC;

#include <cryptopp/sha.h>
using CryptoPP::SHA256;

#include <cryptopp/base64.h>
using CryptoPP::Base64Encoder;

#include <cryptopp/filters.h>
using CryptoPP::HashFilter;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#if defined(CRYPTOPP_NO_GLOBAL_BYTE)
using CryptoPP::byte;
#endif

extern std::string
sign_hmac(std::string key, std::string plain);
#endif