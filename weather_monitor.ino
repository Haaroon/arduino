#include <dht11.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
dht11 DHT11;
#define DHT11PIN 2
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);  //Define the connection LCD pin  

int thermistorPin = 1;           // thermistor connected to analog pin A1

int photocellPin  = 0;           //photoresistor connected to analog pin A0
int photocellReading; // analog reading from the photocell

float global_tempc_therm = 0;
float global_temp_dht = 0;
float global_humid_dht = 0;

void setup()
{ 
     lcd.begin(16, 2);    // set up the LCD's number of columns and rows: 
     lcd.clear();         //Clears the LCD screen and positions the cursor in the upper-left corner 
     delay(1000); //
     Serial.begin(9600);   
}

void clearRow(int row)
{
  lcd.setCursor(0, row);
  lcd.print("                 ");
}

void setupFeels()
{
  lcd.setCursor(0, 0);
  lcd.print("Feels    ");
  lcd.setCursor(14, 0);
  lcd.print(" C "); // Print the unit of the centigrade temperature to the LCD.
}

void printFeels()
{
  // Temp for the termistor
  // Calculate temp   
  float a = analogRead(thermistorPin);
  //the calculating formula of temperature
  float resistor = (1023.0*10000)/a-10000;
  global_tempc_therm = (3435.0/(log(resistor/10000)+(3435.0/(273.15+25)))) - 273.15;
  lcd.setCursor(9, 0);
  lcd.print(global_tempc_therm);// Print a centigrade temperature to the LCD.  
}

void setupDhtTemp()
{
  clearRow(1);
  lcd.setCursor(0, 1); // set the cursor to column 0, line 0
  lcd.print("Temp     ");// Print a message of "Temp: "to the LCD.
  lcd.setCursor(14, 1);
  lcd.print(" C "); // Print the unit of the centigrade temperature to the LCD.
}

void printDhtTemp()
{
   int chk = DHT11.read(DHT11PIN);
   lcd.setCursor(9, 1);
   global_temp_dht = (float)DHT11.temperature;
   lcd.print(global_temp_dht, 2);// Print a centigrade temperature to the LCD.  
}

void setupDhtHumidity()
{
  clearRow(1);
  lcd.setCursor(0, 1);
  lcd.print("Humidity ");// Print a message of "Humidity: "to the LCD.
  lcd.setCursor(14, 1);
  lcd.print(" %"); // Print the unit of the centigrade temperature to the LCD.
}

void printDhtHumidity()
{
  int chk = DHT11.read(DHT11PIN);
  lcd.setCursor(9, 1); // set the cursor to column 0, line 0
  global_humid_dht = (float)DHT11.humidity;
  lcd.print(global_humid_dht, 2);// Print a message of "Humidity: "to the LCD
}

void printSetupLight()
{
  clearRow(1);
  lcd.setCursor(0, 1);
  lcd.print("Light");
}

void printRawLight()
{
  printSetupLight();
  lcd.setCursor(12, 1);
  photocellReading = analogRead(photocellPin);  
  lcd.print((int)photocellReading);// the raw analog reading  
}

void printAssumedLight()
{ 
  // We'll have a few threshholds, qualitatively determined
  lcd.setCursor(5, 1);
  if (photocellReading < 200) {
    lcd.print("       Dark");
  } else if (photocellReading < 450) {
    lcd.print("        Dim");
  } else if (photocellReading < 600) {
    lcd.print("      Light");
  } else if (photocellReading < 950) {
    lcd.print("     Bright");
  } else {
    lcd.print("   V.Bright");
  }
}

void printToSerial(){
  Serial.print(global_tempc_therm);
  Serial.print(",");
  Serial.print(global_temp_dht);
  Serial.print(",");
  Serial.print(global_humid_dht);
  Serial.print("\n");
}

void loop()
{      
    int chk = DHT11.read(DHT11PIN);
    setupFeels();
    setupDhtTemp();
    for (int tempRounds = 0; tempRounds < 15; tempRounds++)
    {  
      printFeels();
      printDhtTemp();
      delay(500);  
    }
    
    setupDhtHumidity();
    for (int tempRounds = 0; tempRounds < 10; tempRounds++)
    {
      printFeels();
      printDhtHumidity();
      delay(500);    
    }
    printToSerial();
    //for (int tempRounds = 0; tempRounds < 6; tempRounds++)
    //{ 
    //  printFeels();    
    //  printRawLight();
    //  delay(2000);
    //  printFeels();
    //  printAssumedLight();
    //  delay(2000);
    //}
}
