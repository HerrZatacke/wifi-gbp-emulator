bool cycle = false;
unsigned long nextMillis = 0;

void wifi_blink_setup() {
  #ifdef LED_BLINK_PIN
  pinMode(LED_BLINK_PIN, OUTPUT);
  #endif
  
  nextMillis = millis() + WIFI_BLINK_DELAY;
}

void wifi_blink_loop() {
  unsigned long currentMillis = millis();

  if (currentMillis >= nextMillis) {
    if (cycle == false) {
      cycle = true;
      nextMillis = currentMillis + 50;
      webSocket.broadcastTXT("# gnip");
    } else {
      cycle = false;
      nextMillis = currentMillis + WIFI_BLINK_DELAY;
    }
    
    #ifdef LED_BLINK_PIN
    digitalWrite(LED_BLINK_PIN, cycle ? HIGH : LOW);
    #endif
  }
}
