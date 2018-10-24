/*
 * Nicholas Carnival
 * Colorado School of Mines
 * Initial Date:  10/15/18 
 * Version: 1.0
 * 
 * Current Date:  10/24/18
 * Version 1.0
 *
 * This Sketch monitors and returns the values of soil water saturation.
 */

//This sensor is basically a resistor


int val = 0;              //value for storing moisture value 
int soilPin = A0;         //Declare a variable for the soil moisture sensor 
int soilPower = 7;        //Variable for Soil moisture Power

//sensor in water range: (550, 590)
//moist dirt: (500, 528)
int saturated = 500;

int iterator = 0;         //this is for testing purposes allowing 
int count = 50;            //the sensor to be shut off after 
                          //a certain amount of data entries
            

int sampleDelay = 500;   //Time interval between scans, recorded in milliseconds

void setup() 
{
  Serial.begin(9600);     // open serial over USB, this is for the "serial monitor"

  pinMode(soilPower, OUTPUT);   //Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
}

void loop() 
{
    int currentVal = (readSoil());

    if(currentVal > saturated)
    {
      Serial.print("High Saturation Detected: ");
      Serial.println(currentVal);
      //Activate a sensor or something
    }else if (currentVal < saturated)
    {
      Serial.print("Saturation Level: ");
      Serial.println(currentVal);
    } //currentVal will never be !< or !>
  
//This delay will need to be changed depending on how frequently we want to scan.

delay(sampleDelay); //takes a reading every .. milliseconds

iterator += 1;

  if(iterator == count)
  {
    Serial.print("ending session");
    delay(15);      //have to delay because failed to print entire string
    exit(0);
  }
  
}

//This is a function used to get the soil moisture content
int readSoil()
{

    digitalWrite(soilPower, HIGH);    //turn D7 "On"
    delay(10);                        //wait 10 milliseconds 
    val = analogRead(soilPin);        //Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);     //turn D7 "Off"
       
    return val;                       //send current moisture value
}
