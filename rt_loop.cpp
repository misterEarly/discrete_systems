#include "rt_loop.h"
#include "LinearCharacteristics.h"
#include "mbed.h"
#include <cstdint>

#define PI 3.1415927

extern DigitalOut led;

extern EncoderCounter enc1;
extern EncoderCounter enc2;

extern AnalogOut i_des1;
extern AnalogOut i_des2;

extern DigitalIn big_button;
extern DigitalOut i_enable;

rt_loop::rt_loop(float Ts) {

  this->Ts = Ts;

  dif1.setup(0.01, Ts);
  dif2.setup(0.01, Ts);

  i2u.setup(-0.8, 0.8, 0, 1);
}

void rt_loop::theloop(void) {
  uint8_t k = 0;
  while (true) {
    ThisThread::flags_wait_any(threadFlag);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // -

    short c1 = enc1;
    short c2 = enc2;

    float phi1 = (float)c1 * 2 * PI / 4000.0f;
    float phi2 = (float)c2 * 2 * PI / 4000.0f;

    float w1 = dif1(phi1);
    float w2 = dif2(phi2);

    if (k < 128) {
      i_des1 = i2u(0.25);
      i_des2 = i2u(0);
    } else {
      i_des1 = i2u(0);
      i_des2 = i2u(0.25);
    }

    i_enable = big_button;

    if (++k == 0) {
      printf("%d %d %f %f %f %f\r\n", c1, c2, phi1, phi2, w1, w2);
      led = !led;
    }
  }
}

void rt_loop::start_theloop() {
  thread.start(callback(this, &rt_loop::theloop));
  ticker.attach(callback(this, &rt_loop::sendSignal), Ts);
}

rt_loop::~rt_loop() {}

void rt_loop::sendSignal() { thread.flags_set(threadFlag); }