#include<Wire.h> // I2C 통신을 이용하기 위한 헤더파일

#define PI 3.141592653589

const int MPU_ADDR = 0x68; // I2C통신을 위한 MPU6050의 주소
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // 7가지 의 값을 받기 위한 변수

double angleAcX, angleAcY, angleAcZ;  // X,Y,Z 가속도
double angleGyX, angleGyY, angleGyZ;  // X,Y,Z 각속도
double angleFiX, angleFiY, angleFiZ;  // 필터를 통해 얻어진 각도

const double RADIAN_TO_DEGREE = 180 / PI; // 라디안 파이값 도 변환
const double DEGREE_PER_SECOND = 32767 / 250; // 1초에 회전하는 각도
const double ALPHA = 1 / (1 + 0.04);

unsigned long now = 0; // 현재 시간 저장용 변수
unsigned long past = 0; // 이전 시간 저장용 변수
double dt = 0; // 한 사이클 동안 걸린 시간 변수

double baseAcX, baseAcY, baseAcZ;
double baseGyX, baseGyY, baseGyZ;

void setup() {
  initSensor();
  Serial.begin(115200);
  Serial.println("The device started, now you can pair it with bluetooth!");
  calibrateSensor(); //  초기 센서 캘리브레이션 함수 호출
  past = millis(); // past에 현재 시간 저장
}

void loop() {
  int btn = digitalRead(23);
  getData();
  getDT(); // loop 문이 한번 돌 때 걸리는 시간 계산

  angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2))); // 가속도 센서값 각도로 변환
  angleAcX *= RADIAN_TO_DEGREE;

  angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
  angleAcY *= RADIAN_TO_DEGREE;
  // 가속도 센서로는 Z축 회전각 계산 불가함

  // 가속도 현재 값에서 초기평균값을 빼서 센서값에 대한 보정
  angleGyX = ((GyX - baseGyX) / DEGREE_PER_SECOND) * dt; //각속도 센서값 각도로 변환
  angleGyY = ((GyY - baseGyY) / DEGREE_PER_SECOND) * dt;

  // 상보필터 처리를 위한 임시각도 저장
  double angleTmpX = (angleFiX + angleGyX);
  double angleTmpY = (angleFiY + angleGyY);
  angleFiX = ALPHA * angleTmpX + (1.0 - ALPHA) * angleAcX;
  angleFiY = ALPHA * angleTmpY + (1.0 - ALPHA) * angleAcY;



  if (btn == 1) {
    int sec = millis() / 1000;
    Serial.print("X축 각도 :");
    Serial.print(angleFiX); Serial.print("\t");
    Serial.print("Y축 각도 :");
    Serial.print(angleFiY); Serial.print("\t");
    Serial.print("몇초동안먹닝 :");
    Serial.println(sec);

  }
}
void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // I2C 통신용 어드레스(주소)
  Wire.write(0x6B); // MPU6050과 통신을 시작하기 위해서는 0x6B번지에
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // AcX 레지스터 위치(주소)를 지칭합니다
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); // AcX 주소 이후의 14byte의 데이터를 요청

  AcX = Wire.read() << 8 | Wire.read(); //두 개의 나뉘어진 바이트를 하나로 이어 붙여서 각 변수에 저장
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

// loop 한 사이클동안 걸리는 시간을 알기위한 함수
void getDT() {
  now = millis();
  dt = (now - past) / 1000.0;
  past = now;
}

// 센서의 초기값을 10회 정도 평균값으로 구하여 저장하는 함수 (측정값-초기값 하기위해)
void calibrateSensor() {
  double sumAcX = 0, sumAcY = 0, sumAcZ = 0;
  double sumGyX = 0, sumGyY = 0, sumGyZ = 0;
  getData();
  for (int i = 0; i < 10 ; i++) {
    getData();
    sumAcX += AcX; sumAcY += AcY; sumAcZ += AcZ;
    sumGyX += GyX; sumGyY += GyY; sumGyZ += GyZ;
    delay(100);
  }
  baseAcX = sumAcX / 10;
  baseAcY = sumAcY / 10;
  baseAcZ = sumAcZ / 10;
  baseGyX = sumGyX / 10;
  baseGyY = sumGyY / 10;
  baseGyZ = sumGyZ / 10;
  delay(10);
}
