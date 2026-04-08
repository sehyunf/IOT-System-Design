void setup() {    
  Serial.begin(9600);   // 아두이노-PC 시리얼 통신 속도 설정
  Serial1.begin(9600); // 아두이노-HC-06 시리얼 통신 속도 설정
}

void loop() { 
  if (Serial1.available()){      // HC-06에 수신된 데이터가 있다면
    Serial.write(Serial1.read());// 시리얼 모니터에 데이터 출력
  }
  
  if (Serial.available()){      // 시리얼 모니터에 입력된 데이터가 있다면
  Serial1.write(Serial.read()); // HC-06를 통해 입력된 데이터 송신
  }
}
