#include <SoftwareSerial.h>   // 소프트웨어시리얼 라이브러리 불러오기

#define LED_R_PIN 9       // LED R(Red) 핀 번호
#define LED_G_PIN 10      // LED G(Green) 핀 번호
#define LED_B_PIN 11      // LED B(Blue) 핀 번호

SoftwareSerial BTSerial(3,2);   // 소프트웨어시리얼 객체를 3(RX),2(TX)로 생성, BTSerial(RX 핀,TX 핀

String data = "";         // CSV 데이터 저장 변수
int idx = 0;              // CSV 데이터 쉼표(구분자) 위치 변수
String LED_CLR = "";      // LED 색상 제어(R, G, B, W) 변수
String duty_str = "";     // 듀티 사이클 문자열 저장 변수
int LED_DUTY = 0;         // LED 밝기 제어(듀티 사이클, 0-100%) 변수
int pwm = 0;              // PWM 제어값 변수

void RGB_LED_ctrl(String color, int pwm) {  // RGB LED 색상/밝기 제어 함수
  int led_r_ctrl = 255;                     // LED R PWM 제어값 초기화
  int led_g_ctrl = 255;                     // LED G PWM 제어값 초기화
  int led_b_ctrl = 255;                     // LED B PWM 제어값 초기화

  if (color == "R") {                       // “R” 입력 시
    led_r_ctrl = 255 - pwm;                 // LED R에만 PWM 제어값 설정
  }
  else if (color == "G") {                  // “G” 입력 시
    led_g_ctrl = 255 - pwm;                 // LED G에만 PWM 제어값 설정
  }
  else if (color == "B") {                  // “B” 입력 시
    led_b_ctrl = 255 - pwm;                 // LED B에만 PWM 제어값 설정
  }
  else if (color == "W") {
    led_r_ctrl = 255 - pwm;                 // LED R에 PWM 제어값 입력
    led_g_ctrl = 255 - pwm;                 // LED R에 PWM 제어값 입력
    led_b_ctrl = 255 - pwm;                 // LED R에 PWM 제어값 입력
  }

  analogWrite(LED_R_PIN, led_r_ctrl);       // LED R에 설정한 PWM 제어값 입력
  analogWrite(LED_G_PIN, led_g_ctrl);       // LED G에 설정한 PWM 제어값 입력
  analogWrite(LED_B_PIN, led_b_ctrl);       // LED B에 설정한 PWM 제어값 입력
}

void setup() {
  Serial.begin(9600);                                  // 아두이노-PC 시리얼 통신 속도 설정   
  BTSerial.begin(9600);                              // 아두이노-HC-06 시리얼 통신 속도 설정
  pinMode(LED_R_PIN, OUTPUT);               // LED R 연결 핀 출력 설정
  pinMode(LED_G_PIN, OUTPUT);               // LED G 연결 핀 출력 설정
  pinMode(LED_B_PIN, OUTPUT);               // LED B 연결 핀 출력 설정
  analogWrite(LED_R_PIN, 255 - pwm);        // LED R 초기화(off)
  analogWrite(LED_G_PIN, 255 - pwm);        // LED G 초기화(off)
  analogWrite(LED_B_PIN, 255 - pwm);        // LED B 초기화(off)
}

void loop() {
  if (BTSerial.available() > 0) {               // 시리얼 모니터에 입력된 데이터가 있다면
    data = BTSerial.readStringUntil('\n');      // 줄바꿈 문자('\n') 전까지 입력된 문자열 저장
    idx = data.indexOf(',');                  // 문자열에서 쉼표 위치 추출
    if (idx != -1) {                          // 쉼표가 있는 경우에만 처리
      LED_CLR = data.substring(0, idx);       // 문자열에서 쉼표 앞 색상 값 추출
      duty_str = data.substring(idx + 1);     // 문자열에서 쉼표 뒤 듀티 사이클 값 추출
      LED_DUTY = duty_str.toInt();            // 문자열 숫자를 int 로 변환
      pwm = map(LED_DUTY, 0, 100, 0, 255);    // 듀티 사이클(0-100) PWM 제어값(0-255)으로 변환
      RGB_LED_ctrl(LED_CLR, pwm);             // 함수를 활용한 LED 색상 및 밝기 제어
    
      Serial.print("[LED control] color: ");  // 시리얼 모니터 출력
      Serial.print(LED_CLR);
      Serial.print(" | duty: ");
      Serial.print(LED_DUTY);
      Serial.println("%");
    } 
  }
}