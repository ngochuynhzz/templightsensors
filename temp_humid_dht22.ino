#include <SimpleTBDevice.h>
#include <ESP8266WiFi.h>
#include "DHTesp.h"

DHTesp dht;

// Tên và mật khẩu wifi.
#define SSID "Namewifi"
#define PASSWORD "Passwifi"

// Địa chỉ máy chủ thingsboard
#define SERVER_ADDR "demo.thingsboard.io" //ví dụ example.com:8080
// Access token của device
#define ACCESS_TOKEN "accesstokeninthingsboard"
#define relay D2 
unsigned long TimeMark=0;

TBDevice device(ACCESS_TOKEN);

void setup() {
  Serial.begin(115200);
  dht.setup(D1, DHTesp::DHT22);
  TimeMark=millis();
  pinMode(leddo, OUTPUT);
  pinMode(relay, OUTPUT);

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Bắt đầu kết nối wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Đã kết nối wifi.");
  
  WiFiClient client;
  device.begin(SERVER_ADDR, client);


}

void loop() {
// đặt giá trị ngưỡng 
  int Ntemp1 ; //20
  int Ntemp2;// = 32;
  int Nhumi1 ;//= 30; 
  int Nhumi2 ;//= 80;
  bool Mode; 
  int Mode_sel;

  Mode_sel = device.read("Mode").asBoolean();

  // nhận giá trị ngưỡng và so sánh
  // ngưỡng so sánh nhiệt độ
  Ntemp1 = device.read("refT1").asInt();//đọc lên things 
  Ntemp2 = device.read("refT2").asInt();//đọc lên things

  // ngưỡng so sánh độ ẩm

  Nhumi1 = device.read("refH1").asInt();//đọc lên things
  Nhumi2 = device.read("refH2").asInt();//đọc lên things
  
    if ((millis() - TimeMark) > 200) {
      TimeMark = millis();
    }
        delay(dht.getMinimumSamplingPeriod());
        float humidity = dht.getHumidity();
        float temperature = dht.getTemperature();

        Serial.print("Humidity:");
        Serial.println(humidity);

        Serial.print("Temperature:");
        Serial.println(temperature);

        device.write("Do_am", humidity);
        device.write("Nhiet_do", temperature);
       // TimeMark = millis();
    //}
        // chế độ Auto

    if(Mode_sel == true){ //true = auto
      // so sánh nhiệt độ
      if(temperature > Ntemp2){ // nhiệt độ  lớn hơn 32 độ thì đèn bật
        digitalWrite(leddo, HIGH);
        Serial.println("den_bat");
        }else if(temperature < Ntemp1){
          digitalWrite(leddo, HIGH);// ngược lại đèn tắt
          Serial.println("den_bat");
      //if(temperature < Ntemp1){ //nhiệt độ nhỏ hơn 20 độ thì đèn bật
       // digitalWrite(leddo, LOW);
       // Serial.println("den_bat");
        }else{
          digitalWrite(leddo, LOW);// ngược lại đèn tắt
          Serial.println("den_tat");
          }
      //so sánh độ ẩm
      if(humidity > Nhumi2){ // độ ẩm lớn hơn 80%
        digitalWrite(relay, LOW); // độ ẩm lớn hơn ngưỡng nên quạt bật
        Serial.println("quat_bat");
        }else if(humidity < Nhumi1){
          digitalWrite(relay, LOW);// ngược lại quạt tat
          Serial.println("quat_bat"); 
          //}
      //if(humidity < Nhumi1){// độ ẩm nhỏ hơn 30%
        //digitalWrite(relay, HIGH);// quạt bật
        //Serial.println("quat_bat");
        }else{
          digitalWrite(relay, HIGH); // ngược lại quạt tắt 
          Serial.println("quat_tat");
            }
  }
  // chế độ Manual
  else{
    if(device.read("button_led").asBoolean()){
      digitalWrite(leddo, HIGH);
      Serial.println("den_bat");
      }else{
        digitalWrite(leddo, LOW); 
        Serial.println("den_bat"); 
      }

    delay(200);

    if(device.read("button_fan").asBoolean()){
      digitalWrite(relay, LOW);
      Serial.println("quat_bat");
      }else{
        digitalWrite(relay, HIGH); 
        Serial.println("quat_tat"); 
      }

    delay(200);

  }
}



