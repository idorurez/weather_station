/**************************************************************************************************************************************
  cencoder.h - c source to a base64 decoding algorithm implementation
  For ESP8266/ESP32 AT-command running shields

  ESP8266_AT_WebServer is a library for the ESP8266/ESP32 AT-command shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_AT_WebServer
  Licensed under MIT license

  Original author:
  @file       Esp8266WebServer.h
  @author     Ivan Grokhotkov

  Version: 1.5.4

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      12/02/2020 Initial coding for Arduino Mega, Teensy, etc
  ...
  1.4.0   K Hoang      14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.4.1   K Hoang      08/12/2021 Add Packages_Patches and instructions for BOARD_SIPEED_MAIX_DUINO
  1.5.0   K Hoang      19/12/2021 Reduce usage of Arduino String with std::string
  1.5.1   K Hoang      24/12/2021 Fix bug
  1.5.2   K Hoang      28/12/2021 Fix wrong http status header bug
  1.5.3   K Hoang      12/01/2022 Fix authenticate issue caused by libb64
  1.5.4   K Hoang      26/04/2022 Use new arduino.tips site. Improve examples
 ***************************************************************************************************************************************/

#pragma once

// Reintroduce to prevent duplication compile error if other lib/core already has LIB64
// pragma once can't prevent that
#ifndef BASE64_CENCODE_H
#define BASE64_CENCODE_H

#define base64_encode_expected_len(n) ((((4 * n) / 3) + 3) & ~3)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
  step_A, step_B, step_C
} base64_encodestep;

typedef struct 
{
  base64_encodestep step;
  char result;
  int stepcount;
} base64_encodestate;

void base64_init_encodestate(base64_encodestate* state_in);

char base64_encode_value(char value_in);

int base64_encode_block(const char* plaintext_in, int length_in, char* code_out, base64_encodestate* state_in);

int base64_encode_blockend(char* code_out, base64_encodestate* state_in);

int base64_encode_chars(const char* plaintext_in, int length_in, char* code_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* BASE64_CENCODE_H */

