#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <U8g2lib.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//===================================
const char* ssid     = "xianjiaoda";        // 连接WiFi名
const char* password = "xianjiaoda";        // 连接WiFi密码
//===================================
const char* host = "api.seniverse.com";     // 将要连接的服务器地址
const int httpPort = 80;                    // 将要连接的服务器端口
String reqUserKey = "SZEF6CCPMTPKXKMtr";    // 私钥
String reqLocation = "Xian";                // 城市
String reqUnit = "c";                       // 摄氏/华氏
String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
                + "&location=" + reqLocation +
                "&language=en&unit=" + reqUnit;
//===================================
String BtcAddress = "https://api.coincap.io/v2/assets/bitcoin";
String EthAddress = "https://api.coincap.io/v2/assets/ethereum";
int ethPrice;
int btcPrice;
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
  String btcToken = TestHttpsAPI(BtcAddress);
  String ethToken = TestHttpsAPI(EthAddress);
  for (int i = 0 ; i < 3; i++) {
    if (btcToken == "") {
      httpRequest(reqRes);
    } else {
      ProcessBtcAndDisplay(btcToken);

    }
    delay(3000);
    if (ethToken == "") {
      httpRequest(reqRes);
    } else {
      ProcessEthAndDisplay(ethToken);
    }
  }
  Serial.println("在下一轮之前等待 20 秒不要在 API 服务器上被禁止......");
  delay(20000);
}

// 连接WiFi
void connectWiFi() {
  Serial.println("====================");
  WiFi.begin(ssid, password);                  // 启动网络连接
  Serial.print("Connecting to ");              // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println(" ...");  // 告知用户NodeMCU正在尝试WiFi连接
  int i = 1;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
    delay(5000);                               // 如果WiFi连接成功则返回值为WL_CONNECTED
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

//https请求
String TestHttpsAPI(String token) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  if (https.begin(*client, token)) {  // HTTPS
    Serial.println("[HTTPS] GET...");
    int httpCode = https.GET();
    // httpCode 将是负的错误
    if (httpCode > 0) {
      // HTTP 头已经发送并且服务器响应头已经被处理
      Serial.printf("[HTTPS] 获取...代码: %d\n", httpCode);
      // 在服务器上找到的文件?
      if (httpCode == HTTP_CODE_OK) {
        //Serial.println(String("[HTTPS] 已收到: ") + https.getString());
        return https.getString().c_str();
        //ProcessAndDisplay();return
      }
    } else {
      Serial.printf("[HTTPS] GET...失败,错误: %s\n\r", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] 无法连接 \n\r");
  }
  return "";
}

//处理并展示btc价格
void ProcessBtcAndDisplay(String data) {
  String tokenName;
  String tokenPrice;
  String starts;
  tokenName =  data.substring(data.length() - 368, data.length() - 371);
  tokenPrice = data.substring(data.length() - 161, data.length() - 166);
  if ( tokenPrice.toInt() > ethPrice ) {
    starts = "UP";
  } else {
    starts = "LOW";
  }
  btcPrice = tokenPrice.toInt();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS24_tf);
    u8g2.drawUTF8(26, 43, tokenName.c_str());
  } while ( u8g2.nextPage() );
  delay(1100);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS24_tf);
    u8g2.drawUTF8(0, 23, starts.c_str());
    u8g2.drawUTF8(3, 63, tokenPrice.c_str());
  } while ( u8g2.nextPage() );
  delay(3000);

}

//处理并展示eth价格
void ProcessEthAndDisplay(String data) {
  String tokenName;
  String tokenPrice;
  String starts;
  tokenName =  data.substring(data.length() - 342, data.length() - 345);
  tokenPrice = data.substring(data.length() - 157, data.length() - 161);
  if (tokenPrice.toInt() > ethPrice) {
    starts = "UP";
  } else {
    starts = "LOW";
  }
  ethPrice = tokenPrice.toInt();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS24_tf);
    u8g2.drawUTF8(26, 43, tokenName.c_str());
  } while ( u8g2.nextPage() );
  delay(1100);
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS24_tf);
    u8g2.drawUTF8(0, 23, starts.c_str());
    u8g2.drawUTF8(16, 63, tokenPrice.c_str());
  } while ( u8g2.nextPage() );
  delay(3000);
}

// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest(String reqRes) {
  WiFiClient client;

  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n";
  Serial.println("");
  Serial.print("Connecting to "); Serial.print(host);

  // 尝试连接服务器
  if (client.connect(host, 80)) {
    Serial.println(" Success!");

    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);

    // 获取并显示服务器响应状态行
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);

    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("Found Header End. Start Parsing.");
    }

    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client);
  } else {
    Serial.println(" connection failed!");
  }
  //断开客户端与服务器连接工作
  client.stop();
}

// 利用ArduinoJson库解析心知天气响应信息
void parseInfo(WiFiClient client) {
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);
  JsonObject results_0 = doc["results"][0];
  JsonObject results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"];
  const char* results_0_now_code = results_0_now["code"];
  const char* results_0_now_temperature = results_0_now["temperature"];
  const char* results_0_last_update = results_0["last_update"];
  String results_0_now_text_str = results_0_now["text"].as<String>();
  int results_0_now_code_int = results_0_now["code"].as<int>();
  int results_0_now_temperature_int = results_0_now["temperature"].as<int>();
  String results_0_last_update_str = results_0["last_update"].as<String>();
  String s = results_0_now_text_str;
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS18_tf);
    u8g2.drawUTF8(15, 43, s.c_str());
  } while ( u8g2.nextPage() );
  delay(1100);

  char m_str[3];
  strcpy(m_str, u8x8_u8toa(results_0_now_temperature_int, 2));
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_luBIS18_tf);
    u8g2.drawStr(33, 43, m_str);
    u8g2.drawStr(66, 43, "'C");
  } while ( u8g2.nextPage() );
  delay(3000);
}
