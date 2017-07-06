
#include <vector>

#include "decode.h"
#include "base64.h"

namespace Decode
{
const std::string &SALT1 = "zEt-*tz";
const std::string &SALT2 = "0Ap";
const std::string &SALT3 = "AsEMh";
const std::string &BASE64_CODES1 = "i0uTnXdWO+K6IpBDSHcse13MmxjyLCrPUY925/lzkJFVAgafEQ7oZGhRNwv4b8qt";
const std::string &BASE64_CODES2 = "WNh3+748zRcCbqwGlM2AIa6FmKTkgSZVfPQLyjD9erY5xuOsnti/1XJpHU0BvEdo";
const std::string &BASE64_CODES3 = "mJ9iMcx5yRfCZldW4VQHw0+7zAO6pEG3sDBnq8LSeThrNPjbUatXkF/voK21guIY";
const std::string &BASE64_CODES4 = "Swlka0UFC3Ht9IZ4mW8oV/vRBdbsqrDTEPhxjpAc5iXy21gGJfu7QL+nKeOYzM6N";

std::string DecryptB64 (std::string s)
    {
        s = s.erase (1, 1);
        s = s.erase(5,1);
        s = base64_decode(s,BASE64_CODES4);
        std::string key = BASE64::xor_encrypt(s.substr(0,4),s.substr(s.length()-5,s.length()-1));
        key.insert(4,1,key.at(0));
        key.erase(0,1);
        s = BASE64::xor_encrypt(s,key);
        s = base64_decode(s,BASE64_CODES3);
        s = BASE64::xor_encrypt(s,SALT2);
        s = BASE64::xor_encrypt(s,SALT3);
        s = base64_decode(s,BASE64_CODES2);
        s = BASE64::xor_encrypt(s,SALT1);
        s = BASE64::xor_encrypt(s,SALT3);
        s = base64_decode(s,BASE64_CODES1);
        s = BASE64::xor_encrypt(s,SALT2);
        s = BASE64::xor_encrypt(s,SALT1);

        return s;
    }

    std::string base64_decode(const std::string &s, const std::string &BASE64_CODES)
    {
        std::string ret;
        std::vector<int> vec(256, -1);
        for (int i = 0; i < 64; i++)
            vec [BASE64_CODES[i]] = i;
        int val = 0, bits = -8;
        for (const auto &c : s)
            {
                if (vec[c] == -1) break;
                val = (val << 6) + vec[c];
                bits += 6;

                if (bits >= 0)
                    {
                        ret.push_back(char((val >> bits) & 0xFF));
                        bits -= 8;
                    }
            }
        return ret;
    }
}
