
// ce code permet grâce à un module GPS d'allumer des led lorsqu'on qu'on de trouve à un endroit cible dont les coordonnées ont été prédéfinies


//-----------------------------------------------------------------------------------------------------------------------------------------------
//                                           les bibliotèques 
//-----------------------------------------------------------------------------------------------------------------------------------------------
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
Adafruit_GPS GPS(&Serial1);


//----------------------------------------------------------------------------------------------------------------------------------------------------
//                                           les variables
//----------------------------------------------------------------------------------------------------------------------------------------------------
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;

//--------------------------------------------------|
//                    WAYPOINTS                     |
//--------------------------------------------------|
//Please enter the latitude and longitude of your   |
//desired destination:                              |
  #define GEO_LAT                48.125140
  #define GEO_LON               -1.623490
//--------------------------------------------------|

//--------------------------------------------------|
//                    DISTANCE                      |
//--------------------------------------------------|
//Please enter the distance (in meters) from your   |
//destination that you want your LEDs to light up:  |
  #define DESTINATION_DISTANCE   10
//--------------------------------------------------|


// Navigation location
float targetLat = GEO_LAT;
float targetLon = GEO_LON;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1);


uint8_t LED_Breathe_Table[]  = {   80,  87,  95, 103, 112, 121, 131, 141, 151, 161, 172, 182, 192, 202, 211, 220,
              228, 236, 242, 247, 251, 254, 255, 255, 254, 251, 247, 242, 236, 228, 220, 211,
              202, 192, 182, 172, 161, 151, 141, 131, 121, 112, 103,  95,  87,  80,  73,  66,
               60,  55,  50,  45,  41,  38,  34,  31,  28,  26,  24,  22,  20,  20,  20,  20,
               20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,
               20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  20,  22,  24,  26,  28,
               31,  34,  38,  41,  45,  50,  55,  60,  66,  73 };


#define BREATHE_TABLE_SIZE (sizeof(LED_Breathe_Table))
#define BREATHE_CYCLE    5000      /*breathe cycle in milliseconds*/
#define BREATHE_UPDATE    (BREATHE_CYCLE / BREATHE_TABLE_SIZE)
uint32_t lastBreatheUpdate = 0;
uint8_t breatheIndex = 0;
//--------------------------------------------------------------------------------------------------
//                                            le setup
//--------------------------------------------------------------------------------------------------

//-------------------parmètrage du GPS-------------------------------------------------------------
void setup()  
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  delay(1000);
  // Ask for firmware version
  Serial1.println(PMTK_Q_RELEASE);
  
    // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}

uint32_t timer = millis();

//-----------------------------------------------------------------------------------------------------------------------------------------*
//                                         le loop
//-----------------------------------------------------------------------------------------------------------------------------------------

//--------------------------lecture et affichage des données GPS------------------------------
void loop()                     // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
      if (c) Serial.print(c);
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 2); Serial.print(GPS.lat);
      Serial.print(", "); 
      Serial.print(GPS.longitude, 2); Serial.println(GPS.lon);
      
      float fLat = decimalDegrees(GPS.latitude, GPS.lat);
      float fLon = decimalDegrees(GPS.longitude, GPS.lon);
      

    }
  
}
//-----------------------------------------------------------------------------------------------------------------------------------
//                                                      les fonctions
//-----------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------conersion des donnée NMEA en degré decimal----------------------------------------

// Convert NMEA coordinate to decimal degrees
float decimalDegrees(float nmeaCoord, char dir) {
  uint16_t wholeDegrees = 0.01*nmeaCoord;
  int modifier = 1;

  if (dir == 'W' || dir == 'S') {
    modifier = -1;
  }
  
  return (wholeDegrees + (nmeaCoord - 100.0*wholeDegrees)/60.0) * modifier;
}
//---------------------------------------------------------------------------------------------------------------------------

void breath()
{
  uniformBreathe(LED_Breathe_Table, BREATHE_TABLE_SIZE, BREATHE_UPDATE, 127, 127, 127);
}

void uniformBreathe(uint8_t* breatheTable, uint8_t breatheTableSize, uint16_t updatePeriod, uint16_t r, uint16_t g, uint16_t b)
{
  int i;

  uint8_t breatheBlu;
  
  if ((millis() - lastBreatheUpdate) > updatePeriod) {
    lastBreatheUpdate = millis();

    
    for (i=0; i < strip.numPixels(); i++) {
      breatheBlu = (b * breatheTable[breatheIndex]) / 256;
      strip.setPixelColor(i, 0, 0, breatheBlu);
    }
    strip.show();   
    
    breatheIndex++;
    if (breatheIndex > breatheTableSize) {
      breatheIndex = 0;
    }   
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
