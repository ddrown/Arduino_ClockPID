void test_addsample() {
  ClockPID.reset_clock();
  float expected[NTPPID_MAX_COUNT] = {
    0.000000,
    0.000006,
    0.000061,
    0.000068,
    0.000075,
    0.000083,
    0.000090,
    0.000099,
    0.000108,
    0.000118,
    0.000127,
    0.000138,
    0.000149,
    0.000160,
    0.000173,
    0.000185
  };

  for(int i = 0; i < NTPPID_MAX_COUNT; i++) {
    float sample = ClockPID.add_sample(100000*i, 5*i, 5*i);
    TEST_ASSERT_FLOAT_WITHIN(0.000001, expected[i], sample);
    if(i > 1) {
      TEST_ASSERT_FLOAT_WITHIN(0.000001, 0.00005, ClockPID.d());
      TEST_ASSERT_FLOAT_WITHIN(1e-9, 0.0, ClockPID.d_chi());
    }
  }
}

void clockPIDTests() {
  RUN_TEST(test_addsample);
}
