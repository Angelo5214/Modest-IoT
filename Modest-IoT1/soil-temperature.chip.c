// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 ANGELO SOLANO ARMAS

#include "wokwi-api.h"
#include <stdlib.h>

typedef struct {
  pin_t pinA0;
  pin_t pinVcc;
  pin_t pinGnd;
  uint32_t temperatureAttribute;
} chip_state_t;

static void chip_timer_event(void* user_data);

void chip_init(void) {
  chip_state_t* chip = malloc(sizeof(chip_state_t));
  chip->pinA0 = pin_init("A0", ANALOG);
  chip->pinVcc = pin_init("VCC", INPUT_PULLDOWN);
  chip->pinGnd = pin_init("GND", INPUT_PULLUP);
  chip->temperatureAttribute = attr_init("temperature", 25);

  const timer_config_t timerConfig = {
    .callback = chip_timer_event,
    .user_data = chip,
  };
  timer_t timerId = timer_init(&timerConfig);
  timer_start(timerId, 200, true);
}

void chip_timer_event(void* user_data) {
  chip_state_t* chip = (chip_state_t*)user_data;
  if (!pin_read(chip->pinVcc) || pin_read(chip->pinGnd)) {
    return;
  }

  float temperature = attr_read_float(chip->temperatureAttribute);
  if (temperature < 0.0f) {
    temperature = 0.0f;
  } else if (temperature > 100.0f) {
    temperature = 100.0f;
  }

  pin_dac_write(chip->pinA0, (temperature / 100.0f) * 3.3f);
}