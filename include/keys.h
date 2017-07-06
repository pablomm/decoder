#ifndef KEYS_H
#define KEYS_H

#include <map>

#ifdef _WIN32

#include <windows.h>
#else

#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_CAPITAL 0x14
#define VK_RSHIFT 0xA0
#define VK_LSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_NUMLOCK 0x90

#endif /* _WIN32 */

namespace KEYS
{

class KeyPair
    {
    public:
        KeyPair (const std::string &vk = "", const std::string &name = "",
                 const std::string &stdname = "", const std::string &capname = "",
                 const std::string &shiftname = "", const std::string &capshiftname = "",
                 const std::string &controlname = "") :
                     VKName (vk), Name (name),
                     StdName(stdname), CapName(capname),
                     ShiftName(shiftname), CapShiftName(capshiftname),
                     ControlName(controlname) {}
        std::string VKName;
        std::string Name;
        std::string StdName;
        std::string CapName;
        std::string ShiftName;
        std::string CapShiftName;
        std::string ControlName;
    };

class Keys
    {
    public:
        static std::map<int, KeyPair> KEYS;
    };

std::string readable_name_str(std::string vknumber);
std::string readable_name(int vknumber_string,bool pressed);
std::string readable_string(std::string str);
std::string vk_string(std::string str);
std::string names_string(std::string str);

}

#endif /* KEYS_H */
