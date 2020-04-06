#include <nvs.h>
#include <nvs_flash.h>

void setup() {
  clearNVS();
}

void loop() {
  
}

void clearNVS() {
    int err;
    err=nvs_flash_init();
    Serial.println("nvs_flash_init: " + err);
    err=nvs_flash_erase();
    Serial.println("nvs_flash_erase: " + err);
 }
