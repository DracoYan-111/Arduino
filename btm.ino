#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char* ssid     = "xianjiaoda";          // 连接WiFi名（此处使用taichi-maker为示例）
const char* password = "xianjiaoda";          // 连接WiFi密码（此处使用12345678为示例）
const char* host = "api.coincap.io";       //请求HTTP头
const char *fingerprint = "62914576dc0afac83c4804bcc2c1b700a61139fe";
const int httpsPort = 443; 

void setup() {
  u8g2.enableUTF8Print();
  u8g2.begin();
  Serial.begin(9600);
  connectWiFi();                               //连接WiFi方法
}

void loop() {
  
  String reqRes = "/v2/assets/bitcoin";
  httpRequest(reqRes);
  delay(5000);
}

// 连接WiFi
void connectWiFi() {
  WiFi.begin(ssid, password);                  // 启动网络连接
  Serial.println("输出网络连接信息: ");              // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println("...");   // 告知用户NodeMCU正在尝试WiFi连接

  int i = 0;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
    delay(5000);                               // 如果WiFi连接成功则返回值为WL_CONNECTED
    Serial.print("第"); Serial.print(i++); Serial.println("次尝试");
    // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
  }                                            // 同时NodeMCU将通过串口监视器输出连接时长读秒。

  // 这个读秒是通过变量i每隔一秒自加1来实现的。
  Serial.println("");                          // WiFi连接成功后
  Serial.println("连接成功!");                  // NodeMCU将通过串口监视器输出"连接成功"信息。
  Serial.print("IP地址为:  ");                  // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
  Serial.println(WiFi.localIP());              // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。
}

// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest(String reqRes) {
  WiFiClient client;

  // 建立http请求信息
//  String httpRequest = String("GET ") + reqRes + " HTTPS/1.1\r\n" +
//                       "Host: " + host + "\r\n" +
//                       "Connection: close\r\n\r\n";
 String httpRequest = String("GET ") + "api.coincap.io/v2/assets/bitcoin";

                       
  Serial.println("");
  Serial.println("连接到:");
  Serial.print(host);
  Serial.println("");

  // 尝试连接服务器
  if (client.connect(host, 80)) {
    Serial.println("成功!");            //成功
    // 向服务器发送http请求信息
    client.print(httpRequest);          //发送请求
    Serial.println("发送请求: ");
    Serial.println(httpRequest);
    // 获取并显示服务器响应状态行
    String status_response = client.readStringUntil('\n');
    Serial.print("状态响应: ");          //状态响应
    Serial.println(status_response);
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("开始解析");        //获取到数据
    }
    // 利用ArduinoJson库解析心知天气响应信息
    // parseInfo(client);
  } else {
    Serial.println("连接失败！");        //连接失败
  }
  //断开客户端与服务器连接工作
  client.stop();
}

void httpsCom(){
  WiFiClientSecure httpsClient;    //建立WiFiClientSecure对象
  
  Serial.println(host);
  httpsClient.setBufferSizes(16000, 512); 
  
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);  
  delay(1000);
  
  Serial.println("HTTPS Connecting");Serial.println("");
  int r=0;  // 尝试连接服务器并等待
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  // 连接超时后输出"连接失败"信息并返回
  if(r==30) { 
    Serial.println("Connection failed");
    return;
  } else { // 连接成功则输出“连接成功”信息
    Serial.println("Connected...");
  }

  Serial.print("requesting: ");
  Serial.println(host);
  
  // 建立HTTPS请求信息字符串
  String request =  String("GET /a/check") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
  // 向服务器发送请求                   
  httpsClient.print(request);
  
  Serial.println("request sent");

  // 检查服务器响应信息。通过串口监视器输出服务器状态码和响应头信息
  // 从而确定ESP8266已经成功连接服务器
  while (httpsClient.connected()) {
    if (httpsClient.available()){
      if (httpsClient.find("Date")){
        Serial.println("Found Date Info in Server Response.");
        int day = httpsClient.parseInt();       
        Serial.print("day = ");Serial.println(day);        
        break;
      }
    }
  }

  Serial.println("Now disconect from the server..."); 
  
  //操作结束，断开服务器连接 
  httpsClient.stop();
  Serial.println("closing connection");  
}
