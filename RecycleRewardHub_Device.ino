/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

/**************************CrowPanel ESP32 HMI Display Example Code************************
Version     :	1.0
Suitable for:	CrowPanel ESP32 HMI Display
Product link:	https://www.elecrow.com/esp32-display-series-hmi-touch-screen.html
Code	  link:	https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File
Lesson	link:	https://www.youtube.com/watch?v=WHfPH-Kr9XU
Description	:	The code is currently available based on the course on YouTube, 
				        if you have any questions, please refer to the course video: Introduction 
				        to ask questions or feedback.
**************************************************************/




#include <Wire.h>
#include <SPI.h>
#include <VL53L0X.h>
#include <s3servo.h>

// #include <ESP32_Servo.h>
#include <WiFi.h>
#include <WebServer.h>


/*******************************************************************************
   Config the display panel and touch panel in gfx_conf.h
 ******************************************************************************/
#include "gfx_conf.h"

#define I2C_SDA 19
#define I2C_SCL 20

#define IR_sensor 38

// Coordinates for displaying the range data
#define DATA_X 100
#define DATA_Y 300

// #define servoPin1 43  //use the GPIO numbers!
// #define servoPin2 44  //use the GPIO numbers!

static const int servoPin1 = 43;  //use the GPIO numbers!
static const int servoPin2 = 44;  //use the GPIO numbers!
// Published values for SG90 servos; adjust if needed
int minUs = 30;
int maxUs = 150;

VL53L0X sensor;

s3servo servo1;
s3servo servo2;
// Network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

// Create a WebServer object
WebServer server(80);

// Function prototypes
void handleRoot();
void handleNotFound();
void handlePost();


void setup() {
  Serial.begin(115200);

  /* Display Prepare strat */
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  delay(100);
  tft.fillScreen(TFT_BLUE);
  delay(1000);
  tft.fillScreen(TFT_YELLOW);
  delay(1000);
  tft.fillScreen(TFT_GREEN);
  delay(1000);
  tft.fillScreen(TFT_WHITE);
  delay(1000);
  water_Mark();
  tft.setCursor(100, 200);
  tft.print("Ir sensor Init");
  delay(4000);
  /* Display Prepare end */

  /*Wi-fi*/
  // Set up access point
  WiFi.softAP(ssid, password);

  // Set up station mode
  WiFi.begin("R&D HW R", "robot0@Petpooja");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/post", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");

  /*Wi-fi*/

  /* IR_sensor start*/
  pinMode(IR_sensor, INPUT_PULLUP);
  /* IR_sensor end*/
  /* Servo Start */
  servo1.attach(servoPin1);
  
  delay(10);
  servo2.attach(servoPin2);
  // servo2.attach(servo2Pin, minUs, maxUs);

  delay(10);

  servo2.attach(servo2Pin, minUs, maxUs);

  water_Mark();
  tft.setCursor(100, 200);
  tft.print("ServoMotor Init at 0 Deg");
  servo1.write(0);
  delay(4000);
  servo2.write(0);
  delay(4000);
  /* Servo End */

  /* VL53L0x sensor strat */
  Wire.begin(19, 20);
  sensor.setTimeout(1000);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }
  sensor.startContinuous();
  /* VL53L0x sensor end */
  water_Mark();
  tft.setCursor(100, 200);
  tft.print("Please give me Plastic Bottle!!");
}
bool state = false;
void loop() {
  // Reading range data
  int read_IR = digitalRead(IR_sensor);
  if (read_IR && !state) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(DATA_X, DATA_Y);
    tft.print("ObjectDetected");
    tft.setCursor(DATA_X, DATA_Y + 50);
    tft.print("Wait untill door Open");
    for (int i = 30; i <= 150; i++) {
      servo2.write(i);
      delay(22);
    }
    state = true;
  } else if (state == 1) {
    delay(3000);
    for (int i = 150; i >= 30; i--) {
      servo2.write(i);
      delay(22);
    }
    int range = sensor.readRangeContinuousMillimeters();
    // Check for sensor timeout
    if (sensor.timeoutOccurred()) {
      Serial.print(" TIMEOUT");
    }
    Serial.println(range);
    water_Mark();
    tft.setCursor(DATA_X, DATA_Y);
    tft.print("Bottle_Height = ");
    tft.print(range);
    tft.print(" mm");

    // http get call

    delay(1000);
    // servo2
    for (int k = 30; k <= 150; k++) {
      servo1.write(k);
      delay(22);
    }
    for (int k = 150; k >= 30; k--) {
      servo1.write(k);
      delay(22);
    }
    delay(1000);
    state = false;
  }
}

void water_Mark() {
  tft.setTextSize(4);
  tft.fillScreen(TFT_BLACK);
  tft.fillCircle(50, 0, 50, TFT_RED);
  tft.fillCircle(100, 0, 50, TFT_BLUE);
  tft.fillCircle(200, 0, 50, TFT_YELLOW);
  tft.fillCircle(300, 0, 50, TFT_RED);
  tft.fillCircle(400, 0, 50, TFT_BLUE);
  tft.fillCircle(500, 0, 50, TFT_YELLOW);
  tft.fillCircle(600, 0, 50, TFT_RED);
  tft.fillCircle(700, 0, 50, TFT_BLUE);
  tft.fillCircle(800, 0, 50, TFT_YELLOW);
  tft.setCursor(130, 100);
  tft.setTextColor(TFT_WHITE);  // Set text color to background color
  tft.print("||Jai Shree Krishana||");
  tft.setTextSize(3);
  tft.setCursor(450, 400);
  tft.print("Auth: Raj Mehta");
  delay(500);
}