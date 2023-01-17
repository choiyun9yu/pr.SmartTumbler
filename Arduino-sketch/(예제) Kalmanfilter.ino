#include <Wire.h>
#include “Kalman.h”  // 소스:  https://github.com/TKJElectronics/KalmanFilter
 
Kalman kalmanX;        // Kalman 인스턴스 생성, X-축
Kalman kalmanY;        // Kalman 인스턴스 생성, Y-축
 
// IMY data
int16_t    accX, accY, accZ;         // x, y, z 축에 대한 가속도
int16_t    tempRaw;                  // 온도에 대한 raw 데이터
int16_t    gyroX, gyroY, gyroZ;      // gyro에 대한 값: x,y,z
 
double    accXangle, accYangle;      // 가속도계를 이용하여 각도 계산, X, Y, Z
double    temp;                      // 온도
double    gyroXangle, gyroYangle;    // 자이로 를 이용하여 각도 계산
double    compAngleX, compAngleY;    // complementary 필터를 이용하여 각도 계산
double    kalAngleX, kalAngleY;      // Kalman 필터를 이용하여 각도 계산
 
uint32_t    timer;
uint8_t    i2cData[14];        // I2C 데이터를 위한 버퍼: 14-바이트
 
void setup()
{
    Serial.begin(9600);        // 시리얼 통신, 9600bps
    Wire.begin();              // I2C 초기화
    
    i2cData[0] = 7;       // 샘플링 속도를 1000Hz - 8Hz/(7+1) = 1000 Hz 로 설정
    i2cData[1] = 0x00;    // FSYNC를 정지, Acc 필터: 260Hz, Gyro 필터: 256Hz, 샘플링 8KHz
    i2cData[2] = 0x00;    // Gyro를 풀 스케일 범위 ±250도/s 로 설정
    i2cData[3] = 0x00;    // 가속도계를 풀 스케일 범위  ±2g 로 설정
    while(i2cWrite(0x19, i2cData, 4, false);    // 4개의 레지스터에 동시에 기록
                                                // 0x19 = sampling rate 설정 레지스터
                                                // 0x1A = config 레지스터
                                                // 0x1B = Gyro config 레지스터
                                                // 0x1C = Accel config 레지스터
    while(i2cWrite(0x6B, 0x01, true));   // PLL을 X축 gyro 레퍼런트로, 슬리모드 해제
                                         // 0x6B = PWR_MGMT_1 레지스터
    while(i2cRead(0x75, 92cData, 1));    // 0x75 = WHO_AM_I 레지스터
    if(i2cData[0] != 0x68) {             // 주소값이 0x68이 아니면
    {
        Serial.print(F(“Error reading sensor”));    // 주소가 0x68이면, AD0가 GND 된 상태
        while(1);                                     // AD0가 VLOGIC(+3.3V) 이면 0x69
    }
 
    delay(100);        // 센서가 안정화 되기를 기다림
 
    // kalman과 gyro의 시작 각도 설정
    while(i2cRead(0x3B, i2cData, 6));          // 0x3B 는 저장된 측정 값의 시작 레지스터, HIGH -> LOW
    accX = ((i2cData[0] <<8) | i2cData[1]);    // Accel X-축 값: HIGH | LOW
    accY = ((i2cData[2] <<8) | i2cData[3]);    // Accel Y-축 값: HIGH | LOW
    accZ = ((i2cData[4] <<8) | i2cData[5]);    // Accel Z
 
    // atan2 는 -π~  π (라디안)  값을 출력 : 참고:  http://en.wikipedia.org/wiki/Atan2
    // 여기서는 이것을 0 ~ 2π로 변환하고,  라디안에서 각도로 바꾼다.
    // atan2(높이, 밑변) => 각도(라디안)
    accYangle = (atan2(accX, accZ) + PI) * RAD_TO_DEG; // roll, Y축으로 회전
    accXangle = (atan2(accY, accZ) + PI) * RAD_TO_DEG; // pitch, X축으로 회전
    // accZangle = (atan2(accZ, accY) + PI) * RAD_T)_DEG; // yaw ??
    
    kalmanX.setAngle(accXangle);    // 시작 각도 설정
    kalmanY.setAngle(accYangle);    //
    gyroXangle   = accXangle;
    gyroYangle   = accYangle;
    compAngleX   = accXangle;
    compAngleY   = accYangle;
 
    timer = micros();
}
 
void loop()
{
    // 모든 데이터를 업데이트
    while(i2cRead(0x3B, i2cData, 14));              // 0x3B 주소에는 데이터가 있는 레지스터, 14바이트를 읽는다
    accX = ((i2cData[0] << 8) | i2cData[1]);        // x축 가속도 값, Hi, Lo 바이트 값을 조합
    accY = ((i2cData[2] << 8) | i2cData[3]);        // y
    accZ = ((i2cData[4] << 8) | i2cData[5]);        // z
    tempRaw = ((i2cData[6] << 8) | i2cData[7]);     // 온도 값
    gyroX = ((i2cData[8] << 8) | i2cData[9]);       // x축 자이로 값
    gyroY = ((i2cData[10] << 8) | i2cData[11]);     // y축 자이로 값
    gyroZ = ((i2cData[12] << 8) | i2cData[13]);     // z축
 
    // atan2는 각도 계산 (atan2(높이, 밑변))으로 -π ~ π 의 라디안 값을 반환한다.
    // 그리고 값을 각도로 바꾼다. http://en.wikipedia.org/wiki/Atan2
    accXangle = (atan2(accY, accZ) + PI) * RAD_TO_DEG;   // pitch
    accYangle = (atan2(accX, accZ) + PI) * RAD_TO_DEG;   // roll
 
    double gyroXrate = (double)gyroX/131.0;    // gyro의 X축 각도 변화량
    double gyroYrate = (double)gyroY/131.0;    // Y축
 
    gyroXangle += gyroXrate*((double)(micros()-timer)/1000000); // 필터 보정 없이 gyro 각도 계산
    gyroYangle += gyroYrate*((double)(micros()-timer)/1000000);
 
    //gyroXangle += kalmanX.getRate()*((double)(micros()-timer)/1000000); // unbiased rate 를 이용하여 gyro 각도 계산
    //gyroYangle += kalmanY.getRate()*((double)(micros()-timer)/1000000);
 
    // Complimentary 필터를 이용하여 각도 계산
    // 값의 계수는 실험적으로 얻을 수 있다. 
    compAngleX = (0.93*(compAngleX+(gyroXrate*(double)(micros()-timer)/1000000)))+(0.07*accXangle); 
    compAngleY = (0.93*(compAngleY+(gyroYrate*(double)(micros()-timer)/1000000)))+(0.07*accYangle);
 
    // Kalman 필터로 각도 계산
    kalAngleX = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros()-timer)/1000000); 
    kalAngleY = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros()-timer)/1000000);
    timer = micros();
 
    temp = ((double)tempRaw + 12412.0) / 340.0;
 
    // 데이터 프린트
    display_formatted_float(accX, 5, 0, 3, false);         // 가속도계에서 직접 얻은 X-축 가속도 raw 값
    display_formatted_float(accY, 5, 0, 3, false);         // Y-축
    display_formatted_float(accZ, 5, 0, 3, false);         // Z-축
    display_formatted_float(gyroX, 5, 0, 3, false);        // 자이로에서 얻은 X-축 위치에 대한 raw 값
    display_formatted_float(gyroY, 5, 0, 3, false);        // Y-축
    display_formatted_float(gyroZ, 5, 0, 3, false);        // Z-축
    
    Serial.print("\t");    // 탭으로 간격을 띄우고
 
    display_formatted_float(accXangle, 5, 2, 3, false);     // 가속도계를 이용하여 계산한 X-축 각도 값
    display_formatted_float(gyroXangle, 5, 2, 3, false);    // 자이로를 이용하여 계산한 X-축 위치 값
    display_formatted_float(compAngleX, 5, 2, 3, false);    // complementary 필터로 계산한 X-축 각도 값
    display_formatted_float(kalAngleX, 5, 2, 3, false);     // Kalman 필터로 계산한 X-축 각도 값
 
    Serial.print("\t");
 
    display_formatted_float(accYangle, 5, 2, 3, false);     // 가속도계를 이용하여 계산한 Y-축 각도 값
    display_formatted_float(gyroYangle, 5, 2, 3, false);    // 자이로를 이용하여 계산한 Y-축 위치 값을
    display_formatted_float(compAngleY, 5, 2, 3, false);    // complementary 필터로 계산한 Y-축 각도 값
    display_formatted_float(kalAngleY, 5, 2, 3, false);     // Kalman 필터로 계산한 Y-축 각도 값
 
    //Serial.print(temp);Serial.print("\t");
 
    Serial.print("\r\n");
    delay(1);
}
 
void display_formatted_float(double val, int characteristic, int mantissa, int blank, boolean linefeed) {
  char outString[16];
  int len;
 
  dtostrf(val, characteristic, mantissa, outString);
  len = strlen(outString);
  for(int i = 0; i < ((characteristic+mantissa+blank)-len); i++) Serial.print(F(" "));
  Serial.print(outString);
  if(linefeed)
    Serial.print(F("\n"));
}
