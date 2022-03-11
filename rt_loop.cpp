#include "rt_loop.h"
#include "mbed.h"

#define PI 3.1415927

extern DigitalOut led;

extern EncoderCounter enc1;
extern EncoderCounter enc2;

rt_loop::rt_loop(float Ts) { this->Ts = Ts; }

void rt_loop::theloop(void) {
  while (true) {
    ThisThread::flags_wait_any(threadFlag);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // -
    led = !led;

    short c1 = enc1;
    short c2 = enc2;

    float phi1 = (float)c1 * 2 * PI / 4000.0f;
    float phi2 = (float)c2 * 2 * PI / 4000.0f;

    printf("%d %d %f %f\r\n", c1, c2, phi1, phi2);
  }
}

void rt_loop::start_theloop() {
  thread.start(callback(this, &rt_loop::theloop));
  ticker.attach(callback(this, &rt_loop::sendSignal), Ts);
}

rt_loop::~rt_loop() {}

void rt_loop::sendSignal() { thread.flags_set(threadFlag); }