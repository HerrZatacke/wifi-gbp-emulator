bool cycle = false;
unsigned long blink_nextMillis = 0;

void wifi_blink_setup() {
  #ifdef LED_BLINK_PIN
  pinMode(LED_BLINK_PIN, OUTPUT);
  #endif

  blink_nextMillis = millis() + WIFI_BLINK_DELAY;
}

void wifi_blink_loop() {
  unsigned long currentMillis = millis();

  if (currentMillis >= blink_nextMillis) {
    if (cycle == false) {
      cycle = true;
      blink_nextMillis = currentMillis + 50;
    } else {
      cycle = false;
      blink_nextMillis = currentMillis + WIFI_BLINK_DELAY;
    }

    #ifdef LED_BLINK_PIN
    digitalWrite(LED_BLINK_PIN, cycle ? LOW : HIGH);
    #endif
  }
}
