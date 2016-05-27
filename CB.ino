#include <LiquidCrystal.h>		//Screen library
#include <math.h>
#include <OneWire.h>          	//This is for DS1820 digital temperature sensor
#include <Button.h>            	//This library is for button presses, includes debounce
#include <pitches.h>			//Speaker commands library

int relay1 = 6;
int relay2 = 7;
int flameSense1 = 8;
int flameSense2 = 9;
int spark = 10;
int dTemp1 = 11;
int dTemp2 = 12;
int tTemp1 = A4;
int tTemp2 = A3;
int alarm = A7;

int buttonPin0 = 14;  //connected to arduino uno analogpin 0
int buttonPin1 = 15;  //connected to arduino uno analogpin 1 
int buttonPin2 = 16;  //connected to arduino uno analogpin 2

Button button1 = Button(buttonPin0,PULLUP);
Button button2 = Button(buttonPin1,PULLUP);
Button button3 = Button(buttonPin2,PULLUP);

int temp1 = 50;    //Setpoint for temperature(deg F) probe 1 which is in burner 1(relay 1) tank.
int temp2 = 50;    //Setpoint for temperature(deg F) probe 1 which is in burner 1(relay 1) tank.
int timer1 = 60;   //Setpoint for timer (in minutes) which is timing the burner on time for tank 1.
int timer2 = 60;   //Setpoint for timer (in minutes) which is timing the burner on time for tank 2.

int startCounter = 0;   //Counter for the settings button, as this incremenets as it steps through the setup cycle

int startStop = 0;      //This stores if the start button has been pressed

float elapsedTime = 0;
float countTime = 0;

int setpointTime

void setup() {
  Serial.begin(115000);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(flameSense1, INPUT);
  pinMode(flameSense2, INPUT);
  pinMode(spark, OUTPUT);
}

void loop() {


//How many tanks would you like to heat?
//How much time would you allow for second tank to heat

  
  while(startCounter == 0){    //Input Temperature setting for Temp 1 
    buttonCount(temp1);
    Serial.println(temp1);
    delay(50);
  }
  while(startCounter == 1){    //Input Temperature setting for Temp 2
    buttonCount(temp2);
    Serial.println(temp2);
    delay(50);
  }
  while(startCounter == 2){     //Input Temperature setting for Timer 1 
    buttonCount(timer1);
    Serial.println(timer1);
    delay(50);
  }
  while(startCounter == 3){     //Input Temperature setting for Timer 2
    buttonCount(timer2);
    Serial.println(timer2);
    delay(50);
  }

buttonBool(startStop);  //Once this start button is pressed the operation is running


//Once tank 1 hits setpoint notify the user with cherp, add your water to mash tun, make sure to stir
//Once it hits setpoint again after the water Chirp
//Turn down to 152 and once at setpoint let it sit for 60 minutes

if (startStop % 2 == 0){
    Serial.println("true is on");
    setStartTimes();    //Sets the start time at zero once the tempControl has hit setpoints for both tanks one and 2
    tempControl(relay1, timer1, temp1, tTemp1, flameSense1);

    if (timer1 <=10){
      tempControl(relay2, timer2, temp2, tTemp2, flameSense2);
    }
  }
}
  
  //---------------------------------------------------------------------------------------------------------
//This button switched between on and off
int buttonBool(int &startOrStop){
  if (button3.isPressed()){
    return startOrStop++;
    delay(100);
  }
}
//---------------------------------------------------------------------------------------------------------
//button ++ and button --    
int buttonCount(int &input){
  if(button1.isPressed()){
    delay(100);
    return input++;
  }
  if(button2.isPressed()){
    delay(100);
    return input--;
  }
  if (button3.isPressed()){
    return startCounter++;
    delay(600);
  }
}

//-------------------------------------------------------------------------------------------------------
//Temperature Control 
int tempControl(int &channel, int &timer, int &setpoint,int &realTemp,int &flameMonitor){
  if(timer > 0 || setpoint > realTemp){  
    if (digitalRead(flameMonitor) == 1){
      digitalWrite(spark, LOW);
	  //Add to keep gas on here
    }
    else{
      for (int flameAttempts = 0; flameAttempts < 20; flameAttempts ++){ //TODO: add flame variable and if flame is on then break
        digitalWrite(channel, HIGH);  //Open Valve
        digitalWrite(spark, HIGH);    //Start sparking
        delay(250);
      }
      digitalWrite(channel, LOW); //Ensure flame is = to 0 after channel is shut off  after a few seconds
      //sound alarm
    }
  }
}
//--------------------------------------------------------------------------------------------------
//Call this function once the start button is pressed
float countDownTime(float &timerCountDown){

  elapsedTime = millis() - startTime;
  
  float timeRemaining = ((timerCountDown*60) - (elapsedTime/1000))/60;
  Serial.println(timeRemaining);
  return timeRemaining;
}

//Set Start timers for tanks 1 and tanks 2
void setStartTimes(){
  if (tTemp1 => temp1 && setpointTime1 = 0){    //This start the timer at 0 and only does it the first time
    startTime = millis();
    startpointTime1++;
  }

  if (tTemp2 => temp2 && setpointTime2 = 0){    //This start the timer at 0 and only does it the first time
    startTime = millis();
    startpointTime2++;
  }
}
//--------------------------------------------------------------------------------------------------------
//Takes the ThermstorPINs raw ADC number and translates it to a temperature base on the thermistors data sheet
float Thermistor(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance = thermr*((1024.0 / RawADC) - 1); 
  Temp = 3800/(log(Resistance/(1000*(pow(2.7182818284,(-3800/298.15)))))); //
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  return Temp; // Return the Temperature
}










//Sound alarm high and display something on the screen

