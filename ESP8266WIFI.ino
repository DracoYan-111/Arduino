#include <ESP8266WiFi.h>
const char* name = "xianjiaoda"; //这里改成你的设备当前环境下要连接的接入点名字
const char* password = "xianjiaoda"; //这里改成你的设备当前环境下要连接的接入点密码
void setup(void)
{
  Serial.println("\u672a\u8fde\u63a5");     // 未连接
  Serial.begin(115200);   //启动串口通讯，波特率设置为115200
  Serial.println("\u5f00\u59cb\u8fde\u63a5");   //开始连接
  //调用 WiFi.begin()函数，开始连接接入点
  WiFi.begin(name , password);
  Serial.println("\u6b63\u5728\u8fde\u63a5\u5230");   //正在连接到
  Serial.println(name);
  while (WiFi.status() != WL_CONNECTED) //判定网络状态
  {
    Serial.println("\u7f51\u7edc\u8fde\u63a5\u6210\u529f");   //网络连接成功
    Serial.println("\u8fde\u63a5\u5230\u7684\u63a5\u5165\u70b9\u540d\u5b57:");    //连接到的接入点名字:
    Serial.println(name );    //告知用户建立的接入点WiFi名
    Serial.println("\u8fde\u63a5\u5230\u7684\u63a5\u5165\u70b9\u5bc6\u7801:");    //连接到的接入点密码:
    Serial.println(password);   //告知用户建立的接入点WiFi密码
    Serial.println("\u65e0\u7ebf\u6a21\u5f0f\u6210\u529f\u5f00\u542f\uff0c\u7f51\u7edc\u8fde\u63a5\u6210\u529f");   //无线模式成功开启，网络连接成功
    delay(5000);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\u65e0\u7ebfIP\u5730\u5740\u4e3a:");    //无线IP地址为:
    Serial.println(WiFi.localIP());
  }
}
void loop() {

}