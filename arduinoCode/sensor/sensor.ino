/*
 * Nicholas Carnival
 * Colorado School of Mines
 * 10/15/18
 * 
 * This Sketch monitors and returns the values of soil water saturation.
 */

//This is very ugly code, but it works

//Check here to see where the sensors need to go

//-----Saturation Variables
int saturationVal = 0;    //value for storing moisture value //if the soil sensor fails this is likely why!
int soilPin = A0;         //Declare a variable for the soil moisture sensor 
int soilPower = 7;        //Variable for Soil moisture Power
int saturated = 600;      //moist dirt: (500, 528)    //sensor in water range: (550, 590)

//-----Flow Meter Variables
int flowPin = 2;          //Setting the flow meter sensor yellow pin
double flowRate;          //This will be the value that the flow meter returns
volatile int flowCount;   //volative to ensure it updates correctly during the interrupt process
double maxFlow = 500.0;

//------Depth Variables
int depthPower = 4;       //Sets depth power to the 4 pin
int depthPin = A1;        //
int depthVal = 0;
int tooDeep = 100;

int iterator = 0;         //initial iteration value, leave this as 0!
int count = 100;           //the amount of times the sensor will take readings
            

int sampleDelay = 500;    //-----Time interval between scans, recorded in milliseconds

//-------Setup Function-------

void setup() 
{
  pinMode(soilPower, OUTPUT);         //Set D7 as an OUTPUT for the soil sensor
  digitalWrite(soilPower, LOW);       //Set to LOW so no power is flowing through the sensor
  
  pinMode(flowPin, INPUT);
  attachInterrupt(0, Flow, RISING);   //Configures interrup 0 (pin 2 on the Arduino Uno) to run the function FLOW

  pinMode(depthPower, OUTPUT);          //Sets teh depth pin power
  digitalWrite(depthPower, LOW);        //Sets to LOW so no power goes through sensor
  Serial.begin(9600);                 // open serial over USB, this is for the "serial monitor"
}

//-------Loop Function-------

void loop() 
{

     
    //-----Flow Meter

      if(flowRate >= maxFlow)
      {
        Serial.print("High Flow Detected: ");
        Serial.println(flowRate);
      }
      else if (flowRate < maxFlow)
      {
        flowCount = 0;                    //Resets the counter
        interrupts();                     //Enabls interrupts on the Arduino
        delay(1000);                      //Delays for 1 second
        noInterrupts();                   //Disable the interrupts on the Arduino
    
        //Converting the flow into Litres / Minute
        flowRate = (flowCount * 2.25);    //Take counted pulses in the last second and multiply by 2.25mL 
        flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
        flowRate = flowRate / 1000;       //Convert mL to Liters, giving you Liters / Minute

        Serial.print("Flow Rate in Litres/Minute: ");
        Serial.println(flowRate);         //Prints flowRate to Serial
      }


      
    
    //-----Soil Saturation
    
      //Checks if the soil is too saturated
      if(saturationVal > saturated)
     {
       Serial.print("High Saturation Detected: ");
       Serial.println(saturationVal);
         
     }       //activates the sensor if the soil is not overly saturated
     else if (saturationVal < saturated)
     {
        Serial.print("Saturation Level: ");
        Serial.println(readSoil());
     }   
      
    //-----Depth Sensor

      if(depthVal >= tooDeep)
      {
        Serial.print("Depth 1 Detected: ");
        Serial.println(depthVal);
      }
      else if (depthVal < tooDeep)
      {
        Serial.print("Depth Value: ");
       Serial.println(readDepth());
      }

    //-----ITERATOR-----
      delay(sampleDelay);           //takes a reading every sampleDelay milliseconds
      iterator += 1;  
      
    //---------------------END THE SESSION---------------------
     if(iterator == count)
     {
      Serial.print("ending session");
      delay(40);                     //have to delay because failed to print entire string
      exit(0);
     }

      
}

//-----Flow Meter Function
void Flow()
{
  flowCount ++;       //adds 1 to flow count
}


//-----Soil Saturation Function
int readSoil()
{
    
    digitalWrite(soilPower, HIGH);          //turn D7 "On"
    delay(10);                              //wait 10 milliseconds 
    saturationVal = analogRead(soilPin);       //Read the SIG value form sensor 
    digitalWrite(soilPower, LOW);           //turn D7 "Off"
       
    return saturationVal;                      //send current moisture value
}

//-----Flow Meter Function
int readDepth()
{
    digitalWrite(depthPower, HIGH);          //turn D7 "On"
    delay(10);                              //wait 10 milliseconds 
    depthVal = analogRead(depthPin);       //Read the SIG value form sensor 
    digitalWrite(depthPower, LOW);           //turn D7 "Off"
       
    return depthVal;                      //send current moisture value
}
