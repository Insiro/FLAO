#include <Bridge.h>
#include <Temboo.h>
#include "utility/TembooGPIO.h"
#include "TembooAccount.h" // Contains Temboo account information
#define RED 9
#define GREEN 10
#define BLUE 11
#define PUMP1 2       //oil PUMP1
#define PUMP2 4       //oil PUMP2
#define PUMP3 7       //oil PUMP3
#define PUMP4 8       //oil PUMP4
#define PUMP5 12      //water PUMP1
#define HUMIDIFIER 13 //가습기 모듈
#define LightSensor 6 //광측센서
int Weatherdump;
int maxCalls = 10;
int calls = 0;
int Light = 0;
int Timenow = 0;
int Weather0 = 0;
void wearherDataError()
{
  Serial.println("Fail");
}
void setup()
{
  Serial.begin(9600);
  delay(4000);
  while (!Serial)
    ;
  Bridge.begin();

  pinMode(RED, OUTPUT);   //R
  pinMode(GREEN, OUTPUT); //G
  pinMode(BLUE, OUTPUT);  //B
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);
  pinMode(PUMP5, OUTPUT);
  pinMode(HUMIDIFIER, OUTPUT);
  pinMode(LightSensor, INPUT);

  Serial.println("Setup complete.\n");
}

void loop()
{
  if (calls < maxCalls)
  {
    Serial.println("Calling GetForecastByCoordinates Choreo...");
    runGetForecastByCoordinates();
    calls++;
    Serial.print("calls=");
    Serial.println(calls);
  }
  else
  {
    Serial.println("Skipping to save Temboo calls. Adjust maxCalls as required.");
  }
  delay(30000);
}

///////////////////////////////////////////////////////////////////////////////////
void runGetForecastByCoordinates()
{
  TembooChoreo GetForecastByCoordinatesChoreo;
  GetForecastByCoordinatesChoreo.begin();
  GetForecastByCoordinatesChoreo.setAccountName(TEMBOO_ACCOUNT);
  GetForecastByCoordinatesChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  GetForecastByCoordinatesChoreo.setAppKey(TEMBOO_APP_KEY);
  GetForecastByCoordinatesChoreo.addInput("APIKey", "885e13b31d6091898c992ca562403711");
  GetForecastByCoordinatesChoreo.addInput("Latitude", "37.5642135");
  GetForecastByCoordinatesChoreo.addInput("Longitude", "127.0016985");
  GetForecastByCoordinatesChoreo.addInput("Units", "si");
  GetForecastByCoordinatesChoreo.setChoreo("/Library/DarkSky/GetForecastByCoordinates");
  /**
  for (int j = 0; j < 10; j++)
  {
    Light = analogRead(LightSensor);
    Serial.print("Light");
    Serial.println(Light);
  }
  **/
  unsigned int returnCode = GetForecastByCoordinatesChoreo.run();
  Serial.println(GetForecastByCoordinatesChoreo.run());
  Serial.println(GetForecastByCoordinatesChoreo.readStringUntil('\x1F'));
  if (returnCode == 0)
  {
    while (GetForecastByCoordinatesChoreo.available())
    {
      String name = GetForecastByCoordinatesChoreo.readStringUntil('\x1F');
      name.trim();
      String data = GetForecastByCoordinatesChoreo.readStringUntil('\x1E');
      data.trim();
      if (!name.compareTo("Summary"))
      {
        switch (data.charAt(0))
        {
        case 'c':
          if (!data.compareTo("clear"))
          {
            Sunny(); //	"clear": "맑음",
          }
          else
            wearherDataError();
          break;
        case 'd':
          if (!data.compareTo("dry"))
            Sunny();
          else
            wearherDataError();
        case 'f':
          if (!data.compareTo("fog"))
            Cloudy(); //"fog": "안개",
          else
            wearherDataError();
          break;
        case 'h':
          Weatherdump = data.compareTo("heavy-sleet");
          if (!Weatherdump)
            Rain(); //"heavy-sleet": "강한 진눈깨비",
          else if (Weatherdump < 0)
          {
            Weatherdump = data.compareTo("heavy-precipitation");
            if (!Weatherdump)
              Rain(); //    "heavy-precipitation": "강한 강수",
            else if (Weatherdump < 0 && !data.compareTo("heavy-clouds"))
              Cloudy(); //    "heavy-clouds": "흐림",
            else if (!data.compareTo("heavy-sleet"))
              Rain(); //       "heavy-sleet": "강한 진눈깨비",
            else
              wearherDataError();
          }
          else
          {
            Weatherdump = data.compareTo("heavy-wind");
            if (!Weatherdump)
              Cloudy(); //"heavy-wind": "강한 바람",
            else if (Weatherdump < 0 && !data.compareTo("heavy-snow"))
              Rain(); //"heavy-snow": "강한 눈",
            else if (!data.compareTo("high-humidity"))
            {
              //"high-humidity": "습함",
            }
            else
              wearherDataError();
          }
          break;
        case 'l':
          Weatherdump = data.compareTo("light-sleet");
          if (!Weatherdump)
            Rain(); //"light-sleet": "약한 진눈깨비",
          else if (Weatherdump < 0)
          {
            Weatherdump = data.compareTo("light-precipitation");
            if (!Weatherdump)
              Rain(); //"light-precipitation": "약한 강수",
            else if (Weatherdump < 0 && !data.compareTo("light-clouds"))
              Cloudy(); //"light-clouds": "약간 흐림",
            else if (!data.compareTo("light-rain"))
              Rain(); //"light-rain": "약한 비",
            else
              wearherDataError();
          }
          else
          {
            Weatherdump = data.compareTo("light-wind");
            if (!Weatherdump)
              Sunny(); //"light-wind": "약한 바람",
            else if (Weatherdump < 0 && !data.compareTo("light-snow"))
              Rain(); //"light-snow": "약한 눈",
            else if (!data.compareTo("low-humidity"))
              Sunny(); // "low-humidity": "건조",
            else
              wearherDataError();
          }
          break;
        case 'm':
          Weatherdump = data.compareTo("medium-sleet");
          if (!Weatherdump)
            Rain(); //"medium-sleet": "진눈깨비",
          if (Weatherdump < 0)
          {
            Weatherdump = data.compareTo("medium-precipitation");
            if (!Weatherdump)
              Rain(); //"medium-precipitation": "강수",
            else if (Weatherdump < 0 && !data.compareTo("medium-clouds"))
              Cloudy(); //    "medium-clouds": "흐림",
            else if (!data.compareTo("medium-rain"))
              Rain(); //"medium-rain": "비",
            else
              wearherDataError();
          }
          else
          {
            Weatherdump = data.compareTo("medium-wind");
            if (!Weatherdump)
            {
              Cloudy();
              //  "medium-wind": "바람",
            }
            else if (Weatherdump < 0 && !data.compareTo("medium-snow"))
              Rain(); //"medium-snow": "눈",
            else if (!data.compareTo("mixed-precipitation"))
              Rain(); // "mixed-precipitation": "진눈깨비",
            else
              wearherDataError();
          }
          break;
        case 'n':
          if (!data.compareTo("no-precipitation"))
          {
            Sunny(); // "no-precipitation": "강수 없음",
          }
          else
            wearherDataError();
          break;
        case 'p':
          Weatherdump = data.compareTo("possible-thunderstorm");
          if (!Weatherdump)
            Rain(); //"possible-thunderstorm": "뇌우 가능성",
          else if (Weatherdump < 0)
          {
            Weatherdump = data.compareTo("possible-light-sleet");
            if (!Weatherdump)
              Rain(); // "possible-light-sleet": "약한 진눈깨비 가능성",
            else if (Weatherdump < 0)
            {
              if (!data.compareTo("possible-light-precipitation"))
                Rain(); //"possible-light-precipitation": "약한 강수 가능성",
              else if (!data.compareTo("possible-light-rain"))
                Rain(); //"possible-light-rain": "가랑비 가능성",
              else
                wearherDataError();
            }
            else if (!data.compareTo("possible-light-snow"))
              Rain(); //"possible-light-snow": "약한 눈 가능성",
            else
              wearherDataError();
          }
          else
          {
            Weatherdump - data.compareTo("possible-very-light-sleet");
            if (!Weatherdump)
              Rain(); // "possible-very-light-sleet": "아주 약한 진눈깨비 가능성",
            else if (Weatherdump < 0)
            {
              if (!data.compareTo("possible-very-light-precipitation"))
                Rain(); //  "possible-very-light-precipitation": "아주 약한 강수 가능성",
              else if (!data.compareTo("possible-very-light-rain"))
                Rain(); // "possible-very-light-rain": "이슬비 가능성",
              else
                wearherDataError();
            }
            else if (!data.compareTo("possible-very-light-snow"))
              Rain(); //  "possible-very-light-snow": "흩뿌리는 눈 가능성",
            else
              wearherDataError();
          }
          break;
        case 't':
          if (!data.compareTo("thunderstorm"))
            Rain(); //"thunderstorm": "뇌우",
          else
            wearherDataError();
        case 'v':
          Weatherdump = data.compareTo("very-light-rain");
          if (!Weatherdump)
            Rain(); //"very-light-rain": "이슬비",
          else if (Weatherdump > 0)
          {
            if (!data.compareTo("very-light-sleet"))
              Rain(); // "very-light-sleet": "아주 약한 진눈깨비",
            else if (!data.compareTo("very-light-snow"))
              Rain(); //"very-light-snow": "아주 약한 눈"
            else
              wearherDataError();
          }
          else if (!data.compareTo("very-light-precipitation"))
            Rain(); //"very-light-precipitation": "아주 약한 강수",
          else
            wearherDataError();
          break;
        default:
          wearherDataError();
        }
      }
    }
  }
  Serial.print("out");
  GetForecastByCoordinatesChoreo.close();
}
///////////////////////////////////////////////////////////////////////////////////
void Sunny()
{
  Weather0 = 1; //Sunny
  Serial.println("Sunny");
  digitalWrite(PUMP1, HIGH);
  delay(5000);
  digitalWrite(PUMP1, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);

  int blue_count = 0;
  int green_count = 0;
  int green_now = 0;
  for (blue_count = 0; blue_count < 255; blue_count++)
  {
    analogWrite(BLUE, blue_count);
    delay(20);
    green_now++;
    if (green_now == 2)
    {
      green_now = 0;
      green_count++;
      delay(20);
      analogWrite(GREEN, green_count);
    }
  }
  green_now = 0;
  for (blue_count = 255; blue_count > 0; blue_count--)
  {
    analogWrite(BLUE, blue_count);
    delay(20);
    green_now++;
    if (green_now == 2)
    {
      green_now = 0;
      green_count--;
      analogWrite(GREEN, green_count);
      delay(20);
    }
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////////

void Cloudy()
{
  Weather0 = 2; //Cloudy
  Serial.println("Cloudy");
  digitalWrite(PUMP2, HIGH);
  delay(5000);
  digitalWrite(PUMP2, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);

  int blue_count = 0;
  int red_count = 0;
  int red_now = 0;

  for (blue_count = 0; blue_count < 255; blue_count++)
  {
    analogWrite(BLUE, blue_count);
    delay(20);
    red_now++;
    if (red_now == 1)
    {
      red_now = 0;
      red_count++;
      delay(20);
      analogWrite(RED, red_count);
    }
  }
  red_now = 0;
  for (blue_count = 255; blue_count > 0; blue_count--)
  {
    analogWrite(BLUE, blue_count);
    delay(20);
    red_now++;
    if (red_now == 1)
    {
      red_now = 0;
      red_count--;
      analogWrite(RED, red_count);
      delay(20);
    }
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////////
void Rain()
{
  Weather0 = 3; //Rain
  Serial.println("Rain");
  digitalWrite(PUMP3, HIGH);
  delay(5000);
  digitalWrite(PUMP3, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);

  int rain_color = 0;
  for (rain_color = 0; rain_color <= 100; rain_color++)
  {
    analogWrite(RED, rain_color);
    analogWrite(GREEN, rain_color);
    analogWrite(BLUE, rain_color);
    delay(100);
  }
  delay(500);
  for (rain_color = 100; rain_color >= 1; rain_color--)
  {
    analogWrite(RED, rain_color);
    analogWrite(GREEN, rain_color);
    analogWrite(BLUE, rain_color);
    delay(100);
  }
  return;
}
///////////////////////////////////////////////////////////////////////////////////
void Sleep()
{
  digitalWrite(PUMP4, HIGH);
  delay(5000);
  digitalWrite(PUMP4, LOW);
  digitalWrite(PUMP5, HIGH);
  delay(5000);
  digitalWrite(PUMP5, LOW);
}
