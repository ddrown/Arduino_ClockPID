#include "ClockPID.h"

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

ClockPID_c ClockPID;

float ClockPID_c::average(int32_t *points) {
  float sum = 0;

  for(uint32_t i = 0; i < count; i++) {
    sum += points[i];
  }
  return sum / count;
}

static int float_compare(const void *e1, const void *e2) {
  float f1 = *(float*)e1;
  float f2 = *(float*)e2;
  if(f1 > f2) return 1;
  if(f1 < f2) return -1;
  return 0;
}

struct linear_result ClockPID_c::theil_sen(float avg_ts, float avg_out) {
  struct linear_result r;
  float slopes[(NTPPID_MAX_COUNT-1)*NTPPID_MAX_COUNT/2];
  uint32_t slopes_count = 0;
  uint32_t median_slope_idx;

  for(uint32_t i = 0; i < (count - 1); i++) {
    for(uint32_t j = i+1; j < count; j++) {
      slopes[slopes_count] = (raw_offsets[j] - raw_offsets[i]) / (float)(timestamps[j] - timestamps[i]);
      slopes_count++;
    }
  }

  if(slopes_count == 0) {
    r.a = 0;
    r.b = avg_out;
  } else {
    qsort(slopes, slopes_count, sizeof(float), float_compare);
    median_slope_idx = lround(slopes_count / 2);
    r.a = slopes[median_slope_idx];
    r.b = avg_out - r.a*avg_ts;
  }

  return r;
}

float ClockPID_c::chisq(struct linear_result lin) {
  float chisq_r = 0;

  for(uint32_t i = 0; i < count; i++) {
    float expected = timestamps[i]/1000.0*lin.a + lin.b/1000.0;
    chisq_r += pow((raw_offsets[i]/1000.0 - expected), 2) / expected;
  }

  return chisq_r;
}

struct deriv_calc ClockPID_c::calculate_d() {
  float avg_ts, avg_off;
  struct linear_result lin;
  struct deriv_calc d;

  avg_ts = average(timestamps);
  avg_off = average(raw_offsets);

  lin = theil_sen(avg_ts, avg_off);
  d.d = lin.a;
  d.d_chisq = chisq(lin);

  return d;
}

float ClockPID_c::calculate_i() {
  float result = 0;

  for(uint32_t i = 0; i < count; i++) {
    result += corrected_offsets[i] / 1000.0;
  }
  return result;
}

// 500 ppm
float ClockPID_c::limit_500(float factor) {
  if(factor > 0.000500) {
    return 0.000500;
  } else if(factor < -0.000500) {
    return -0.000500;
  }
  return factor;
}

void ClockPID_c::make_room() {
  for(uint32_t i = 0; i < count-1; i++) {
    timestamps[i] = timestamps[i+1];
    raw_offsets[i] = raw_offsets[i+1];
    corrected_offsets[i] = corrected_offsets[i+1];
  }

  count--;
}

// TODO: deal with timestamp wraps
float ClockPID_c::add_sample(int32_t timestamp, int32_t raw_offset, int32_t corrected_offset) {
  last_p = corrected_offset / 1000.0;
 
  if(count == NTPPID_MAX_COUNT) {
    make_room();
  }
  timestamps[count] = timestamp;
  raw_offsets[count] = raw_offset;
  corrected_offsets[count] = corrected_offset;
  count++;

  last_d = calculate_d();
  last_i = calculate_i();

  last_out_p = last_p * NTPPID_KP;
  last_out_i = last_i * NTPPID_KI;
  last_out_d = last_d.d * NTPPID_KD;

  last_out_p = limit_500(last_out_p);
  last_out_i = limit_500(last_out_i);
  last_out_d = limit_500(last_out_d);
  last_out = last_out_p + last_out_i + last_out_d;
  last_out = limit_500(last_out);

  return last_out;
}