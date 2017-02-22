#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <NTPtimeESP.h> 

#define SSID "***"             // WORK
#define PASS "***"                    // WORK
//#define SSID "***"                    // Home SSID
//#define PASS "***"                    // HOME PASS

NTPtime NTPch("gb.pool.ntp.org");

strDateTime dateTime;

int pinCS = D3; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

String dayTime;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 75; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels



void setup(void) {

  matrix.setIntensity(7); // Use a value between 0 and 15 for brightness

  matrix.setPosition(0, 3, 0); // The first display is at <0, 0>
  matrix.setPosition(1, 2, 0); // The second display is at <1, 0>
  matrix.setPosition(2, 1, 0); // The third display is at <2, 0>
  matrix.setPosition(3, 0, 0); // And the last display is at <3, 0>

  matrix.setRotation(0, 3);    // The first display is position upside down
  matrix.setRotation(1, 3);    // The first display is position upside down
  matrix.setRotation(2, 3);    // The first display is position upside down
  matrix.setRotation(3, 3);    // The same hold for the last display
  
  //ESP.wdtDisable();                             // used to debug, disable wachdog timer, 
  Serial.begin(115200);                           // full speed to monitor
                               
  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("SSID : ");                        // prints SSID in monitor
  Serial.println(SSID);                           // to monitor             
  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
  Serial.println("Connected!");
  

}
void loop() {
   // first parameter: Time zone in floating point (for India); second parameter: 1 for European summer time; 2 for US daylight saving time (not implemented yet)
  dateTime = NTPch.getNTPtime(0, 1);
  NTPch.printDateTime(dateTime);

  String actualHour      = String(dateTime.hour);
  String actualMinute    = String(dateTime.minute);
  String actualsecond    = String(dateTime.second);
  int    actualyear      = dateTime.year;
  String actualMonth     = String(dateTime.month);
  String actualday       = String(dateTime.day);
  String actualdayofWeek = String(dateTime.dayofWeek);

  dayTime = actualHour + ":" + actualMinute + " "  + actualday + "-" + actualMonth  + "-" + actualyear;
  
  // Serial.println(dayTime); // Just to double check
  
  for ( int i = 0 ; i < width * dayTime.length() + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);
               
    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < dayTime.length() ) {
        matrix.drawChar(x, y, dayTime[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display

    delay(wait);
  }
}
