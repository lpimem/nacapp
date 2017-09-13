#include <string> 

namespace rpi
{
    class GPIOPin{
        public:
        GPIOPin(std::string pin);

        int 
        exportPin();

        int 
        unexportPin();

        int 
        setDirection(std::string dir);

        int 
        setValue(std::string v);

        int getValue(std::string& v);

        std::string getPinNum();
        private:
        std::string pinNum;
    };
}