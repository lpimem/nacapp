//#include "motion.h"
#include <unistd.h>
#include <wiringPi.h>

namespace ndn {
namespace smarthome {

class Motor{
public:
  Motor(){
    pinMode (2, OUTPUT);
    pinMode (3, OUTPUT);
  }

/*  int set(int opt) {
    if (opt == m_on) {
      return m_on;
    } else {
      m_on = !m_on;
      if (m_on) {
        digitalWrite (2, HIGH);
        digitalWrite (3, LOW);
        //m_motion.forward();
      } else {
        //m_motion.stop();
        digitalWrite (2, LOW);
      }
    }

    return m_on;
  }*/

  int set(int opt) {
    if (m_on == 0) {
      std::cout << "MOTOR ON" << std::endl;
      digitalWrite (2, HIGH);
      digitalWrite (3, LOW);
      m_on = 1;
    } else {  //LED IS ON, TURN IT OFF AND RETURN OFF STATE
      std::cout << "MOTOR OFF" << std::endl;
      digitalWrite(2, LOW);
      m_on = 0;
    }
    return HIGH;

  }

  int get() {
    return m_on;
  }

private:
  int m_on = 0;
  //Motion m_motion;
};

}
}
