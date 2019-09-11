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
#define HUMIDIFIER 1  //가습기 모듈
#define LightSensor 6 //광측센서
int Weathertem;
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

  for (int j = 0; j < 10; j++)
  {
    Light = analogRead(LightSensor);
    Serial.print("Light");
    Serial.println(Light);
  }

  unsigned int returnCode = GetForecastByCoordinatesChoreo.run();
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
            //	"clear": "맑음",
          }
          else
            wearherDataError();
          break;
        case 'f':
          if (!data.compareTo("fog"))
          {
            //"fog": "안개",
          }
          else
            wearherDataError();
          break;
        case 'h':
          if (!data.compareTo("heavy-sleet"))
          {
            //"heavy-sleet": "강한 진눈깨비",
          }
          else if (data.compareTo)
            break;
        case 'l':
          break;
        case 'm':
          break;
        case 'n':
          if (!data.compareTo("no-precipitation"))
          {
            // "no-precipitation": "강수 없음",
          }
          else
            wearherDataError();
          break;
        case 'p':
          break;
        case 't':
          if (!data.compareTo("thunderstorm"))
          {
            //"thunderstorm": "뇌우",
          }
          else
            wearherDataError();
        }
        if (data == "dry" || data == "breezy" || data == "no precipitation" || data == "clear")
        {
          Weather0 = 1; //Sunny
          Serial.print("Sunny=");
          Serial.println(Weather0);
          Sunny();
        }
        else if (data == "overcast" || data == "mostly cloudy" || data == "partly cloudy" || data == "foggy" || data == "humid" || data == "dangerously windy" || data == "windy" || data == "mixed precipitation")
        {
          Weather0 = 2; //Cloudy
          Serial.print("Cloudy=");
          Serial.println(Weather0);
          Cloudy();
        }
        else if (data == "rain" || data == "light rain" || data == "possible light rain" || data == "drizzle" || data == "possible drizzle" || data == "thunderstorms" || data == "possible thunderstorms" || data == "heavy snow" || data == "snow" ||
                 data == "light snow" || data == "possible light snow" || data == "flurries" || data == "possible flurries" || data == "heavy sleet" || data == "sleet" || data == "light sleet" || data == "possible light precipitation" || data == "possible light sleet" ||
                 data == "light precipitation" || data == "possible light precipitation" || data == "light precipitation" || data == "precipitation" || data == "heavy precipitation")
        {
          Weather0 = 3; //Rain
          Serial.print("Rain=");
          Serial.println(Weather0);
          Rain();
        }
        else
        {
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
