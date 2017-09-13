#include <iostream>
#include <wiringPi.h>

namespace ndn {
namespace smarthome {

class Led
{
public:
  const int pin = 21;

  int
  set(int opt)
  {
    //LED IS OFF, TURN IT ON AND RETURN ON STATE
    if (led_status == 0) {
      std::cout << "LED ON" << std::endl;
      digitalWrite(21, HIGH);
      led_status = 1;
    }
    else { //LED IS ON, TURN IT OFF AND RETURN OFF STATE
      std::cout << "LED OFF" << std::endl;
      digitalWrite(21, LOW);
      led_status = 0;
    }
    return HIGH;
    /*
    std::cout << digitalRead(pin) << std::endl;
    if (opt != digitalRead(pin)) {
      std::cout << "fvsf" << std::endl;
      digitalWrite(pin, HIGH);
      return HIGH;
    } else {
      digitalWrite(pin, LOW);
      return LOW;
    }
    return opt;*/
  }

  int
  get()
  {
    //std::cout << "get" << digitalRead(pin) << std::endl;
    return digitalRead(pin);
  }

private:
  int led_status = 0;
};

} // namespace smarthome
} // namespace ndn
