#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

//温湿度センサ関連の定数
#define DHT_PIN 7
#define DHT_TYPE DHT11
//温湿度センサ関連の定数終わり

//フルカラーLED関連の定数
#define RGBLED_PIN 6 //LEDに出力するピン番号
#define LED_COUNT 1 // ArduinoにつないでいるLEDの個数
//フルカラーLED関連の定数終わり

//LCDディスプレイ関連の定数
#define RS_PIN 8 //制御線1
#define E_PIN 9 //制御線2
#define DB4_PIN 10 //通信線1
#define DB5_PIN 11 //通信線2
#define DB6_PIN 12 //通信線3
#define DB7_PIN 13 //通信線4
//LCDディスプレイ関連の定数終わり

//温湿度センサインスタンスの宣言
DHT dht( DHT_PIN, DHT_TYPE );
//LEDインスタンスの宣言
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel( LED_COUNT, RGBLED_PIN, NEO_RGB + NEO_KHZ800 );
//LCDディスプレイインスタンスの宣言
LiquidCrystal lcd(RS_PIN, E_PIN, DB4_PIN, DB5_PIN, DB6_PIN, DB7_PIN);

void setup() {
  Serial.begin( 9600 ); //通信用待機時間
  dht.begin();
  RGBLED.begin();
  RGBLED.setBrightness( 10 ); //LEDの明るさの指定(0-255)を行う
  lcd.begin(16, 2); //行と列の長さを指定
  lcd.print("hello, Arduino!"); //テストメッセージ表示
  delay(2000); //待機
}

void loop() {
  float temp = dht.readTemperature(); //温度
  float hum = dht.readHumidity(); //湿度

  //温度を表示
  lcd.setCursor( 0, 0 ); //LCDのカーソル位置を1行目に設定
  lcd.print( "Temp:" );
  lcd.print( temp );
  lcd.print( "*C    " );
  //湿度を表示
  lcd.setCursor( 0, 1 ); //LCDのカーソル位置を2行目に設定
  lcd.print( "Hum:" );
  lcd.print( hum );
  lcd.print( "%      " );

  //不快指数表示
  Discom_index_print( temp, hum );
  
  delay( 1000 );
}

//不快指数計算・表示用変数
void Discom_index_print( float temp, float hum ){
  int di_calc = 0; //不快指数計算用変数

  //不快指数を計算
  di_calc = ( 0.81 * temp ) + ( 0.01 * hum ) * ( 0.99 * temp - 14.3 ) + 46.3;

  if( di_calc < 55 ){ //寒い
    RGBLED.setPixelColor( 0, 0, 0, 255 ); //青色に点灯
    RGBLED.show(); //LEDにデータを送る
  }
  else if( di_calc < 60 ){ //肌寒い
  RGBLED.setPixelColor( 0, 0, 128, 128 ); //水色に点灯
  RGBLED.show(); //LEDにデータを送る
  }
  else if( di_calc < 75 ){ //快適
  RGBLED.setPixelColor( 0, 0, 255, 0 ); //緑色に点灯
  RGBLED.show(); //LEDにデータを送る
  }
  else if( di_calc < 80 ){ //やや暑い
  RGBLED.setPixelColor( 0, 128, 128, 0 ); //黄色に点灯
  RGBLED.show(); //LEDにデータを送る
  }
  else if( di_calc < 85 ){ //汗がでる
  RGBLED.setPixelColor( 0, 170, 85, 0 ); //橙色に点灯
  RGBLED.show(); //LEDにデータを送る
  }
  else if( di_calc > 85 ){ //暑くてたまらない
  RGBLED.setPixelColor( 0, 255, 0, 0 ); //赤色に点灯
  RGBLED.show(); //LEDにデータを送る
  }
}

