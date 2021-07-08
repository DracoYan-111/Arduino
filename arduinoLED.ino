/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

void setup() {
  //首选运行，初始化函数
  //pinMode：设置引脚模式
  //LED_BUILTIN:led引脚
  //OUTPUT：输出模式
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  //其次执行
  //digitalWrite：引脚写入电频
  //LOW：写入低电频，不通电
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  
  //delay：延时函数，毫秒为单位
  delay(1000);                      // Wait for a second

  //HIGH：写入高电频，通电
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
