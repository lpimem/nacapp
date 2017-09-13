
#include <iostream>
#include <sstream>
#include <fstream>
#include <string> 

#include "gpio.hpp"

namespace rpi
{

   /* int tellGPIO(std::string action, std::string payload){
        const std::string fd = "/sys/class/gpio/"+action;
        std::ofstream of(fd.c_str());
        if (of < 0){
            std::cerr << "[ERROR] GPIO cannot proceed with "<< action<<" + " <<payload<< "." << std::endl;
            return -1;
        }
        of << payload;
        of.close();
        return 0;
    }


    int askGPIO(std::string action, std::string& ans){
        const std::string fd = "/sys/class/gpio/"+action;
        std::ifstream ifs(fd.c_str());
        if (ifs < 0){
            std::cerr << "[ERROR] GPIO cannot proceed with "<< action<<"." << std::endl;
            return -1;
        }
        ifs >> ans;
        ifs.close();
        return 0;
    }

    GPIOPin::GPIOPin(std::string pin){
        this->pinNum = pin;
    }

    int GPIOPin::exportPin(){
        return tellGPIO("export", this->pinNum);
    }

    int GPIOPin::unexportPin(){
        return tellGPIO("unexport", this->pinNum);
    }

    int GPIOPin::setDirection(std::string dir){
        return tellGPIO("gpio"+this->pinNum+"/direction", dir);
    }

    int GPIOPin::setValue(std::string v){
        return tellGPIO("gpio"+this->pinNum+"/value", v);
    }

    int GPIOPin::getValue(std::string& v){
        return askGPIO("gpio" + this->pinNum + "/value", v);
    }

    std::string GPIOPin::getPinNum(){
        return this->pinNum;
    }
 */
}
