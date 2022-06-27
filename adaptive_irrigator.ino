const int pumpTimer = 30 ; // Seconds to run the pump

// Pin configuration
const int pumpPin = 11 ;
const int indicatorLED = 13 ; // Built-in led


// Calibrations
// get the max and min values for the soil humidity sensor. Those can be read if you read the sensor output when dry and submerged in water.
const int dry = 835; // value for dry sensor
const int wet = 440; // value for wet sensor

const int pMax = 1023; // Value when potentiometer is at max
const int pMin = 0;


// Initialization
void setup()
{ 
  Serial.begin(9600);
  pinMode(pumpPin,OUTPUT);   // Pump relay
  pinMode(indicatorLED,OUTPUT);
}



int getSoilHumidity()
{
  // Use average of 3 sensor readings
  int soilIterator = 0;
  for (int i=0; i<3; i++)
  {
    soilIterator += analogRead(A0);

    // toggle indicator LED while reading sensor
    for ( int b=0; b<5; b++)
    {
      digitalWrite(indicatorLED, HIGH);
      delay(250);
      digitalWrite(indicatorLED, LOW);
      delay(250);
    }
  }

  int sensorVal = soilIterator/3;
  // Sensor has a range of wet and dry; map the calibrated values betweeen 0 to 100 percent
  int percentageHumididy = map(sensorVal, wet, dry, 100, 0);
  return(percentageHumididy);
}

int readPotentiometer()
{
  // Read the potentiometer to actionLevel,
  // which indicate at what humidity level to start the pump
  int pVal = analogRead(A5);
  int actionLevel= map(pVal, pMin, pMax, 0, 100);
  return(actionLevel);
}

void runPump(int runsec)
{
  if (digitalRead(pumpPin) == LOW)
  {
    Serial.println(". Starting water pump.");

    digitalWrite(pumpPin, HIGH);
    digitalWrite(indicatorLED, HIGH);
  }  
}

void stopPump()
{
  if (digitalRead(pumpPin) == HIGH)
  {
    Serial.println("Stopping water pump");
    digitalWrite(pumpPin, LOW);
    digitalWrite(indicatorLED, LOW);
  }
}

void loop()
{

  int soilHumidity = getSoilHumidity();
  int actionLevel = readPotentiometer();

  if( soilHumidity <= actionLevel)
  {
    Serial.print("Soil still too dry at ");
    Serial.print(soilHumidity);
    Serial.print(", should be at least ");
    Serial.print(actionLevel);
    Serial.println(" %");
    runPump(pumpTimer);
  }
  if( soilHumidity > actionLevel)
  {
    Serial.print("Soil moist enough at ");
    Serial.print(soilHumidity);
    Serial.println(" %. ");
    stopPump();
  }
  delay(pumpTimer*1000);
}
