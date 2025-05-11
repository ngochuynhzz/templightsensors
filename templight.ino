#include <Wire.h>              // Thư viện giao tiếp I2C
#include <LiquidCrystal_I2C.h> // Thư viện điều khiển LCD qua I2C
// SCL A5
// SDA A4 

unsigned long TimeMark;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ LCD I2C là 0x27, kích thước 16x2

int ledPin = 13; 
int light_sensor = 2; // truyền giá trị cảm biến vô chân số 2 của arduino
int light_value;

float temperature; // Biến lưu nhiệt độ
int LM35 = A0;     // Cảm biến LM35 kết nối chân A0
int relay = 4;  // Relay điều khiển quạt kết nối chân số 2


void setup() {
  Serial.begin(9600); 

  lcd.init();                  // Khởi tạo màn hình LCD
  lcd.backlight();             // Bật đèn nền màn hình LCD
  lcd.print("Starting...");    // Hiển thị thông báo bắt đầu trên màn hình LCD

   TimeMark = millis();        // Lưu thời gian bắt đầu
  pinMode(ledPin, OUTPUT);
  pinMode(light_sensor, INPUT);
  pinMode(relay, OUTPUT);     // Chân D5 là output (quạt)

  digitalWrite(relay, LOW); // Khởi tạo trạng thái quạt tắt
  digitalWrite(ledPin, LOW); // Khởi tạo trạng thái đèn tắt
}

void loop() {

  // đọc giá trị từ cảm biến ánh sáng
  light_value = digitalRead(light_sensor); // đọc giá trị cảm biến
  Serial.println(light_value); // in giá trị cảm biến ra màn hình

   // Đọc giá trị nhiệt độ từ cảm biến LM35
  temperature = analogRead(LM35); 
  temperature = (5.0 * temperature * 100.0) / 1024.0; // Chuyển đổi giá trị sang °C
  
  Serial.print("Nhiệt độ: ");
  Serial.println(temperature); // Hiển thị nhiệt độ qua Serial Monitor
  

   if ((millis() - TimeMark) > 2000) {  // Đọc lại mỗi 2 giây
    TimeMark = millis();  // Cập nhật thời gian

    // Kiểm tra nếu không nhận được giá trị từ cảm biến
    if (temperature == NAN || light_value == NAN) {
      Serial.println("không nhận được giá trị cảm biến!");
      lcd.clear();
      lcd.print("Cảm biến lỗi!"); // Hiển thị lỗi trên màn hình LCD
      return; // Dừng chương trình nếu có lỗi
    }
     
    // Hiển thị nhiệt độ và độ ẩm lên màn hình LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("light: ");
    lcd.print(light_value, 1);
    lcd.print(" %");
  }  
  if(light_value==1){ // nếu giá trị cảm biến == 1 (khi trời tối, che quang trở )
    digitalWrite(ledPin, HIGH); // giá trị cảm biến bằng 1 đèn sáng và ngược lại
    Serial.println("đèn sáng khi trời tối");
    
  }else {// ngược lại thì tắt đèn khi trời sáng
    digitalWrite(ledPin, LOW);
    Serial.println("đèn tắt khi trời sáng");
  }

  // Điều khiển quạt dựa vào nhiệt độ
  if (temperature >= 30.0) { // Nếu nhiệt độ lớn hơn hoặc bằng 30°C
    digitalWrite(relay, HIGH); // Bật quạt
    Serial.println("Quạt bật");
    
  } else { // Nếu nhiệt độ thấp hơn hoặc bằng 30°C
    digitalWrite(relay, LOW); // Tắt quạt
    Serial.println("Quạt tắt");
  }
  
  delay(1000); // Chờ 1 giây trước khi lặp lại
}
