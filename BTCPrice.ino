#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <U8g2lib.h>
#include <WiFiClientSecureBearSSL.h>

#define DebugPrintln(message)    Serial.println(message)

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//===================================
// 连接WiFi名
const char* ssid     = "xianjiaoda";
// 连接WiFi密码
const char* password = "xianjiaoda";
//===================================

void setup() {
  u8g2.enableUTF8Print();
  u8g2.begin();
  Serial.begin(9600);
  Serial.println("");
  // 连接WiFi
  connectWiFi();
}


void loop() {
  TestHttpsAPI();
  Serial.println("在下一轮之前等待 20 秒不要在 API 服务器上被禁止......");
  delay(20000);
}


void TestHttpsAPI() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  if (https.begin(*client, "https://api.coincap.io/v2/assets/bitcoin")) {  // HTTPS
    Serial.println("[HTTPS] GET...");
    int httpCode = https.GET();
    // httpCode 将是负的错误
    if (httpCode > 0) {
      // HTTP 头已经发送并且服务器响应头已经被处理
      Serial.printf("[HTTPS] 获取...代码: %d\n", httpCode);
      // 在服务器上找到的文件?
      if (httpCode == HTTP_CODE_OK) {
        //Serial.println(String("[HTTPS] 已收到: ") + https.getString());
        ProcessAndDisplay(https.getString());
      }
    } else {
      Serial.printf("[HTTPS] GET...失败,错误: %s\n\r", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] 无法连接 \n\r");
  }
}

void ProcessAndDisplay(String data) {
  String tokenName;
  String tokenPrice;
  tokenName =  data.substring(data.length() - 368, data.length() - 371);
  tokenPrice = data.substring(data.length() - 161, data.length() - 166);

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS18_tf);
    u8g2.drawUTF8(35, 43, tokenName.c_str());
    //u8g2.drawUTF8(30, 63, "he");

  } while ( u8g2.nextPage() );
  delay(1000);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS18_tf);
    u8g2.drawUTF8(20, 43, tokenPrice.c_str());
  } while ( u8g2.nextPage() );
  delay(5000);
}

// 连接WiFi
void connectWiFi() {
  Serial.println("====================");
  WiFi.begin(ssid, password);                  // 启动网络连接
  Serial.print("Connecting to ");              // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println(" ...");  // 告知用户NodeMCU正在尝试WiFi连接
  int i = 1;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
    delay(1000);                               // 如果WiFi连接成功则返回值为WL_CONNECTED
    Serial.print("第");
    Serial.print(i++);
    Serial.println("次连接");      // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
  }                                            // 同时NodeMCU将通过串口监视器输出连接时长读秒。
  // 这个读秒是通过变量i每隔一秒自加1来实现的。
  Serial.println("");                          // WiFi连接成功后
  Serial.println("连接成功!");   // NodeMCU将通过串口监视器输出"连接成功"信息。
  Serial.print("IP 地址:");             // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
  Serial.println(WiFi.localIP());              // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。
  Serial.println("====================");
}