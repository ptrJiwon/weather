#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// 텍스트 작성 함수에서 사용 writeText(textside(1~5),color(TFT_BLACK),"String",CenterUp)
#define CenterUp 1
#define CenterCenter 2
#define CenterDown 3
#define LeftUp 4
#define LeftCenter 5
#define LeftDown 6
#define RightUp 7
#define RightCenter 8
#define RightDown 9

//객체 생성
HTTPClient http;
TFT_eSPI tft = TFT_eSPI();

// GLOBAL VARIABLES
// WIFI
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";



//Functions

// 텍스트 작성 함수 (정렬용)
void writeText(uint8_t T_size, int color, String title, uint8_t i)
{
  tft.setTextSize(T_size);
  tft.setTextColor(color);

  short textWidth = tft.textWidth(title);
  short textHeight = tft.fontHeight();
  
  short x, y;

  if (i == 1)
  {
    x = (tft.width() - textWidth) / 2; 
    y = 20;
  }
  else if (i == 2)
  {
    x = (tft.width() - textWidth) / 2; 
    y = (tft.height() - textHeight) / 2;
  }
  else if (i == 3)
  {
    x = (tft.width() - textWidth) / 2; 
    y = tft.height() - (textHeight + 20);
  }
  else if (i == 4)
  {
    x = 20; 
    y = 20;
  }
  else if (i == 5)
  {
    x = 20; 
    y = (tft.height() - textHeight) / 2;
  }
  else if (i == 6)
  {
    x = 20; 
    y = tft.height() - (textHeight + 20);
  }
  else if (i == 7)
  {
    x = tft.width() - (textWidth + 20); 
    y = 20;
  }
  else if (i == 8)
  {
    x = tft.width() - (textWidth + 20);
    y = (tft.height() - textHeight) / 2;
  }
  else if (i == 9)
  {
    x = tft.width() - (textWidth + 20);
    y = tft.height() - (textHeight + 20);
  }

  tft.setCursor(x, y);
  tft.println(title);

}

// API 데이터 수집
String getData()
{
  http.begin("http://api.openweathermap.org/data/2.5/weather?lat=90&lon=135&appid=YOUR_API_KEY&units=metric");
  // (https://openweathermap.org/)에서 API KEY를 받고 YOUR_API Replace하기 
  
  int httpCode = http.GET(); // 서버에 HTTP GET 요청 보내기
  // 200 = 성공, 404 = URL 잘못됨, 401 = API Key 문제 등등


  if(httpCode == 200) // 요청 성공 시 데이터 저장
  {
    String payload = http.getString(); // HTTP에서 받은 내용을 문자열 형태로 가져오기
    Serial.print(payload); // 시리얼 모니터로 JSON 데이터 확인
    //JSON = JavaScript Object Notation
    return payload;
  }
  else
  {
    Serial.print("Failed to get Data");
    return "";
  }
}

// 데이터 파싱 후 저장
void parseData()
{
  String payload = getData(); // getData에서 받아온 데이터를 저장

  DynamicJsonDocument doc(1024); // 1024바이트의 메모리 공간 생성
  // deserializeJson(doc, payload); // payload에 저장된 API데이터를 doc에 파싱 후 저장
  // deserializeJson will return
  // - DeserializationError::Ok (성공 시)
  // - DeserializationError::InvalidInput (JSON 형식이 잘못됨)
  // - DeserializationError::NoMemory (버퍼 크기 부족)
  // - DeserializationError::IncompleteInput (JSON 문자열 다 안들어옴)

  // 파싱 성공 여부 확인
  DeserializationError err = deserializeJson(doc, payload); // Deserialization타입의 err변수에 성공 실패 여부 저장
  if (err)
  {
    Serial.print("JSON 파싱 실패");
    Serial.println(err.c_str()); // 에러 메세지를 char 문자열로 변환 후 출력
    // 실패 시 정지하기 ( 추가 코딩 필요 )
  }
  else
  {
    Serial.print("JSON 파싱 성공");
  }
  

  /*
  // JSON 데이터 예시
  {
    "weather":[{"main":"Clear","description":"clear sky"}],
    "main":{"temp":25.3,"humidity":60},
    "name":"Seoul"
  }
  */

  // main이라는 객체에서 temp를 가져온 후 변수 temp에 저장, 값이 없으면 0.0으로 초기화
  float temp = doc["main"]["temp"] | 0.0; 
  // | - 비트 연산자 or 이 아닌
  //     JsonVariant 또는 JsonVariant로 변환 가능한 타입의 값과 대체 할 값을 피연산자로 쓰며
  //     왼쪽 값이 존재하지 않을 시 오른쪽 값을 사용하도록 하는 연산자 (Arduino Json.h)

  int humidity = doc["main"]["humidity"] | 0; // 위와 동일하게 humidity에 저장
  String city = doc["name"] | "blank";

  showData(temp, humidity, city);

  return;
}


// 화면에 데이터 출력
void showData(float temp, int humidity, String city)
{
  tft.fillScreen(TFT_BLACK);
  String temp_S = String(temp,1);
  String humidity_S = "Humidity : " + String(humidity) + "%";
  
  // 화면 출력
  Serial.println("Temp: ");
  Serial.println(temp, 1); // 소숫점 한자리까지 출력
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("City: ");
  Serial.println(city);
  
  writeText(5, TFT_RED, temp_S, RightCenter);
  writeText(2, TFT_BLUE, humidity_S, CenterDown);
  writeText(2, TFT_WHITE, city, CenterUp);
  


  return;
}


void setup() 
{
  // 1. 시리얼 모니터 초기화
  Serial.begin(115200);
  
  // 2. TFT 디스플레이 초기화
  tft.init();
  tft.setRotation(1); // 가로 모드
  tft.fillScreen(TFT_BLACK);
  delay(100);
  
  // 3. WiFi 연결
  WiFi.begin(ssid, password);
  Serial.print("Wi-Fi 연결 중");
  writeText(3, TFT_WHITE, "WIFI CONNECTING", CenterCenter);

  String str_buffer = ".";
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("."); 
    str_buffer = str_buffer + ".";
    writeText(2, TFT_WHITE, str_buffer, LeftDown);
  }
  Serial.println("Wi-Fi 연결 완료");
  tft.fillScreen(TFT_BLACK); 
  writeText(2, TFT_GREEN, "WIFI CONNECTED", LeftCenter);
  delay(2000);
}

void loop() 
{
  parseData();
  delay(20000); // 20초마다 갱신
}