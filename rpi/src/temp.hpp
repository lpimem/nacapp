#include "mcp9808.h"
#include <unistd.h>
#include <cstdio>

namespace ndn {
namespace smarthome {

class Temp{
public:
  int getTemp() {
    char const *i2c_device = "/dev/i2c-1";   //added const for c++11 std
    int address = 0x18;
  
    void *s = mcp9808_init(address, i2c_device);
  
    double t = mcp9808_temperature(s);
    int ans = t*1.8+32;
    mcp9808_close(s);

    return ans;
  }
};

}
}
