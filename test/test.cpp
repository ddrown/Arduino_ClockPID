#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

#include "ClockPID.h"

#include "test-ClockPID.cpp"

void setUp() {
  ArduinoFakeReset();
}

int main(int argc, char **argv) {
  UNITY_BEGIN();

  clockPIDTests();

  return UNITY_END();
}

