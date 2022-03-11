#include "rt_loop.h"
#include "mbed.h"

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

    printf("%d %d\r\n", c1, c2);
  }
}

void rt_loop::start_theloop() {
  thread.start(callback(this, &rt_loop::theloop));
  ticker.attach(callback(this, &rt_loop::sendSignal), Ts);
}

rt_loop::~rt_loop() {}

void rt_loop::sendSignal() { thread.flags_set(threadFlag); }