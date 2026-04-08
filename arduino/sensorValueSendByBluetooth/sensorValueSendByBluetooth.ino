#include <SoftwareSerial.h> 
#include <DHT.h>            // DHT 라이브러리 
#include <Wire.h>           // I2C 라이브러리
#include <BH1750.h>         // BH1750 라이브러리 
#include <MQ135.h>          // MQ135 라이브러리 

#define DHT11_PIN 2         // DHT 센서가 연결된 핀 번호
#define DHT11_TYPE DHT11    // 사용 중인 DHT 센서의 종류(DHT11)
#define MQ135_PIN A12       // MQ135 센서가 연결된 핀 번호
#define DFR0034_PIN A6      // DFR0034 센서가 연결된 핀 번호

DHT dht11(DHT11_PIN, DHT11_TYPE); // DHT 센서 객체 생성
BH1750 bh1750;                    // BH1750 센서 객체 생성
MQ135 mq135(MQ135_PIN);           // MQ135 센서 객체 생성

int TEMP = 0;                     // 온도 변수
int RH = 0;                       // 상대습도 변수 
int LUX = 0;                      // 조도 변수 
int GAS = 0;                      // 유해가스 변수 
float SIP = 0;                    // 소리 변수
int adc = 0;                      // DFR0034 ADC 입력값 변수
int adc_max = 0;                  // DFR0034 최대 ADC 입력값 (초기 0 설정)
String data = "";                 // 센서 데이터 저장용 문자열 변수
unsigned long t = 0;              // 현재 동작시간(ms)
unsigned long t_prev = 0;         // 이전 동작시간(ms)
unsigned long t_prev_DFR0034 = 0; // DFR0034 이전 동작시간(ms)
int period_ms = 2000;             // 동작 주기 설정(ms)
int period_ms_DFR0034 = 100;      // DFR0034 동작주기 설정(ms)

void setup() {
  Serial.begin(9600);             // PC- 아두이노 시리얼 통신 속도 설정
  Serial1.begin(9600);           
  dht11.begin();                  // DHT 센서 초기화
  Wire.begin();                   // I2C 통신 시작
  bh1750.begin();                 // BH1750 센서 초기화
  t_prev = millis();              // 이전 동작시간(ms) 입력
}

void loop() {
  t = millis();                                   // 이전 동작시간(ms) 입력

  if (t - t_prev_DFR0034 >= period_ms_DFR0034) {  // DFR0034 동작주기마다 동작
    adc = analogRead(DFR0034_PIN);                // 소리 세기에 따른 ACD 값 저장
    if (adc > adc_max) {                          // ADC 값이 저장된 최대 ADC 값보다 크면
      adc_max = adc;                              // 최대 ADC 값으로 저장 
    }
    t_prev_DFR0034 = t;                           // DFR0034 이전 동작시간(ms) 입력
  }

  if(t - t_prev >= period_ms){             // 동작주기마다 동작
    TEMP = dht11.readTemperature();        // 온도 값 변수에 저장
    RH = dht11.readHumidity();             // 상대습도 값 변수에 저장
    LUX = bh1750.readLightLevel();         // 조도값 변수에 저장
    GAS = mq135.getCorrectedPPM(TEMP, RH); // 유해가스 농도 변수에 저장
    SIP = (adc_max/1023.0)*100;            // 최대 소리 세기 대비 백분율 변수에 저장
    
    data = String(TEMP) + "," +            // 센서 측정값 문자열로 변환
           String(RH) + "," +
           String(LUX) + "," +
           String(GAS) + "," +
           String(SIP,1);                  // 소수점 첫째 자리까지 출력("45.8" 등)

    Serial.println(data);                  // 시리얼 모니터 출력
    Serial1.println(data);
    adc_max = 0;                           // 최대 ADC 값 초기화
    t_prev = t;                            // 이전 동작시간(ms) 입력
  }
}
