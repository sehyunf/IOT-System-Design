import bluetooth                                                                               # 블루투스 라이브러리 불러오기

def connect_bluetooth(device_name, port=1):                                         # 장치명을 사용한 블루투스 기기 검색 및 연결 함수
    print("블루투스 기기 검색 중...")                                                        
    device_mac = None                                                                        # IoT 센서 모듈 MAC 주소
    bt_dev = bluetooth.discover_devices(duration=8, lookup_names=True)   # 주변 블루투스 기기 검색(duration은 검색 시간(초))
    for addr, name in bt_dev:                                                                 # 검색된 블루투스 기기들을 대상으로
        print(f"장치 이름: {name}, MAC 주소: {addr}")                                  # 각 기기의 이름과 MAC 주소 출력
        if name == device_name:                                                            # IoT 센서 모듈의 장치명을 찾았을 경우
            device_mac = addr                                                                 # 해당 장치의 MAC 주소 저장
            break                                                                                   # 반복문 종료
    if not device_mac:                                                                           # 장치를 찾지 못한 경우
        print(f"장치 '{device_name}'를 찾을 수 없습니다.")
        return None
    try:                                                                                               # 장치를 찾았을 경우
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)                   # 블루투스 소켓 생성
        sock.connect((device_mac, port))                                                   # 블루투스 연결 시도
        print(f"블루투스 기기 연결 성공: {device_name}")                        
        return sock                                                                                # 연결된 블루투스 소켓 반환
    except bluetooth.btcommon.BluetoothError as e:                                 # 블루투스 관련 오류 발생 시
        print("블루투스 기기 연결 실패:", e)
        return None

def receive_data(sock, buffer_size = 1024, decode_format = "utf-8"):           # 블루투스 기기에서 데이터 수신 함수    
    try:
        data = sock.recv(buffer_size).decode(decode_format).strip()               # 최대 1024 byte 데이터 수신 후 utf-8 규칙에 따라 문자열로 변환
        return data                                                                                # 문자열 데이터 반환
    except Exception as e:                                                                     # 모든 종류의 오류 상황 발생 시
        print(f"데이터 수신 오류: {e}")                                                       # 오류 상황 출력
        return None

def parse_data(data, feature):                # 수신된 CSV 데이터 파싱 후 딕셔너리로 반환하는 함수
    try:
        values = data.split(",")                  # 수신된 CSV 문자열을 쉼표 기준으로 분리하여 리스트로 변환
        data_dic = {}                              # 센서 데이터를 저장할 빈 딕셔너리 생성
        for i in range(len(feature)):           # feature 리스트 길이만큼 반복
            key = feature[i]                      # 현재 인덱스에 해당하는 센서 변수명 선택
            value = values[i].strip()            # 해당 값의 양쪽 공백 제거
            try:                                       
                converted = int(value)         # 문자열 값을 정수형(int)으로 변환 시도                                          
            except ValueError:                  # 정수형 변환 실패 시
                try:
                    converted = float(value)  # 실수형(float)으로 변환 시도
                except ValueError:              # 실수형 변환도 실패하면
                    converted = value          # 그대로 문자열로 유지
            data_dic[key] = converted       # 키-값 쌍을 딕셔너리에 저장
        return data_dic                          # 딕셔너리 데이터 반환
    except Exception as e:                    # 전체 처리 과정에서 예외 발생 시
        print(f"데이터 파싱 오류: {e}")      # 오류 메시지 출력
        return None                             # None 반환하여 오류 처리

sensor_name = "KJP05"                                    # IoT 센서 모듈 장치명

sensor_sock = connect_bluetooth(sensor_name)   # 장치명을 사용한 블루투스 기기 검색 및 연결 함수 실행
feature = ['TEMP', 'RH', 'LUX', 'GAS', 'SIP']            # 센서 측정값 변수 설정
feature_unit = ['°C', '%', 'lux', 'ppm', '%']

if sensor_sock:                                                # 블루투스 소켓이 연결되어 있다면
    try:
        while True:                                            # 무한 루프
            data = receive_data(sensor_sock)          # 블루투스 기기에서 데이터 수신 함수 실행
            sensor_data = parse_data(data, feature)
            if sensor_data:
                result = " | ".join(
                    f"{f}: {d} {u}"
                    for f, u, d in zip(feature, feature_unit, sensor_data.values())
                )
                print('[Received Data]', end=" ")
                print(result)# 수신된 데이터 출력
    except KeyboardInterrupt:                           # Ctrl + C를 눌러 프로그램을 강제로 중단할 경우
        print("데이터 수신 중단")
    finally:                                                     # 종료 시
        sensor_sock.close()                                # 블루투스 연결 종료
        print("블루투스 연결 종료")