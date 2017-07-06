#ifndef DECODE_H
#define DECODE_H

#include <string>

namespace Decode
{
std::string DecryptB64 (std::string s);
std::string base64_decode(const std::string &s, const std::string &BASE64_CODES);

}

#endif // DECODE_H
