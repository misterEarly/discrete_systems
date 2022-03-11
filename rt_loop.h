#pragma once

#include "EncoderCounter.h"
#include "IIR_filter.h"
#include "Signal.h"
#include "ThreadFlag.h"
#include "mbed.h"


class rt_loop {
public:
  rt_loop(float);
  void start_theloop(void);
  virtual ~rt_loop();

private:
  void theloop(void);
  Thread thread;
  Ticker ticker;
  ThreadFlag threadFlag;
  Signal signal;
  void sendSignal();
  Timer ti;
  float Ts;

  IIR_filter dif1;
  IIR_filter dif2;
};
