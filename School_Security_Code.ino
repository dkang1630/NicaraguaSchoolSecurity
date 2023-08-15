// C++ code
const int button1 = 2; 
const int button2 = 3; 
const int buzzer = 5;
const int reed_switch = 6;
const int horn = 7;
int reed_status;
unsigned long previousMillis = 0;
unsigned long interval = 5000;
const int LED = 13;

int password[] = {1,2,2,1,1,2}; //the desired code is entered in this array,
                                //separated by commas
                                //you can change the password in this array

int enteredPassword[7]; //create a new empty array for the code entered by

int alarm_state = 0;
int ctr = 0;
bool doorOpened = false;
bool noise = false;
int timecount = 0;
bool playHorn=false;
unsigned long timeNow = 0;
unsigned long activateTime = 0;
unsigned long currTime = 0;
bool valid = 0;
int readstatusarray[10]={0,0,0,0,0,0,0,0,0,0};
int ctr_read = 0;


void setup(){
  

  pinMode(button1, INPUT_PULLUP); //button 1 is an input
  pinMode(button2, INPUT_PULLUP); //button 2 is an input
  pinMode(buzzer, OUTPUT); 
  pinMode(reed_switch, INPUT);
  pinMode(horn, OUTPUT); 
  Serial.begin(9600); //begin Serial
  pinMode(LED, OUTPUT);
  
  for (int i = 0; i < 6;i++){ //work through numbers 0-3
    Serial.println(password[i]); //print each digit of the code
    Serial.println(enteredPassword[i]); //print each element of the entered[]
                                //array (this was for me to check that it 
                                //started at 0
  }
}

void loop()
{
  if(playHorn==false){
      DeActivateHorn();
   }
   currTime = millis();
   Serial.println("TIME NOW");
   Serial.println(timeNow);
   if(timeNow != 0 && currTime > timeNow + 5000){
    //5000 = 5 seconds here relate to the delay betweem the buzzer and the big horn
    //you can increase the delay by increasing the delay time: eg 10000 = 10 seconds
    playHorn=true;
    Serial.println("BIG HORN PLAYING");
    ActivateHorn();
    }
  Serial.println(timeNow);
  digitalWrite(LED, LOW);
  reed_status = readStatus(digitalRead(reed_switch));
  Serial.print("reed_status: ");
  Serial.println(reed_status);
  valid = compareCode();

  
  if(valid && alarm_state==0){
    //changing it to armed 
     Serial.println("Arming");
     activateTime = millis();
    alarm_state = 1;
    ActivateAlarm();
    
  }
  else if(valid && alarm_state==1){
    //changing it to unarmed 
    alarm_state = 0;
    Serial.println("Unarming");
    deActivateAlarm();
  }

  if(alarm_state==1){
    ActivateAlarm();
  }
  else{
    deActivateAlarm();
  }

  delay(200);
  
}

void ActivateAlarm(){
  //reed_status -1 -> means far apart 
  // reed status -0 -> means together
  //reed_status = digitalRead(reed_switch); for dubugging purpose
  Serial.println("Activate Alarm");
  
  if(currTime >  activateTime + 10000){ 
    //it means the door is getting opened the first time
    // 10000 here means you have 10 seconds after you actiave the alarm for the reed switch to activate
    // if you want to increase the delay, increase the time: eg 60000 = 60 seconds
    if(reed_status == 1  && doorOpened == false){
      doorOpened=true;
      startNoise();
    }

    //closed the door but still havent types in the code to deactivate
    if(reed_status == 0 && doorOpened == true){
      startNoise();
    }

   if(reed_status==0 && doorOpened == false){
    stopNoise();
    Serial.println("door closed");
    }
  }

}
  
void deActivateAlarm(){
  doorOpened = false;
  Serial.println("Deactivate Alarm");
  stopNoise();
  alarm_state = 0;
}

void startNoise(){
  timeNow = millis();
  Serial.println("start Noise");
  tone(buzzer,440);
}

void stopNoise(){

  noTone(buzzer);
  playHorn=false;
  timeNow = 0;
}

bool compareCode(){ //checks if the code entered is correct by comparing the code[] array with the entered[] array
  
   if (digitalRead(button1) == LOW){ //if button1 is pressed
    enteredPassword[ctr]=1;
    ctr+=1; //call checkEntered and pass it a 1
    Serial.print(ctr);
    Serial.println(": 1");
    delay(250);//wait, needed for correct functioning, otherwise
               //buttons are deemed to be pressed more than once
    
  }
  else if (digitalRead(button2) == LOW){ //if button2 is pressed //call checkEntered1 and pass it a 2
    enteredPassword[ctr]=2;
    ctr+=1; //call checkEntered and pass it a 1
    Serial.print(ctr);
    Serial.println(": 2");
    delay(250); //wait
    
  }
  bool flag = false;
  if(ctr==6){
    for(int i =0 ; i < 6;i++){
      Serial.print(enteredPassword[i]);
    }
    if((enteredPassword[0]==password[0]) && (enteredPassword[1]==password[1]) && (enteredPassword[2]==password[2]) && (enteredPassword[3]==password[3]) && (enteredPassword[4]==password[4])&& (enteredPassword[5]==password[5])){
        flag = true;
        blinkOnce();
        Serial.println("CORRECT PASS");
      }
      if(flag==false){
      Serial.println("=========================Blink 3 Times=====================");
      blinkThreeTimes();
      }
     
    }
    else {
    Serial.println("INCORRECT PASS");
    
    }
  //reset
  if(ctr == 6){
    ctr = 0;
  }
  return flag;
}

void ActivateHorn() {
  digitalWrite(horn, LOW);
}


void DeActivateHorn() {
  digitalWrite(horn, HIGH);
}

int readStatus(int readstat){
  if(ctr_read==5){
    ctr_read=0;
    }
    readstatusarray[ctr_read]=readstat;
    int localCtr=0;
    for(int i = 0 ; i < 5  ; i++){
        localCtr+=readstatusarray[i];
      }
    ctr_read=ctr_read+1;
    if(localCtr>3){
      return 1;
      }
    return 0;
     

  }

void blinkOnce(){
  digitalWrite(LED,HIGH); //HIGH is set to about 5V PIN8
  delay(1000);               //Set the delay time, 1000 = 1S
  digitalWrite(LED,LOW);  //LOW is set to about 5V PIN8
}


void blinkThreeTimes(){
  digitalWrite(LED,HIGH); //HIGH is set to about 5V PIN8
  delay(100);               //Set the delay time, 1000 = 1S
  digitalWrite(LED,LOW);  //LOW is set to about 5V PIN8
  delay(100); 
  digitalWrite(LED,HIGH); //HIGH is set to about 5V PIN8
  delay(100);               //Set the delay time, 1000 = 1S
  digitalWrite(LED,LOW);  //LOW is set to about 5V PIN8
  delay(100); 
  digitalWrite(LED,HIGH); //HIGH is set to about 5V PIN8
  delay(100);               //Set the delay time, 1000 = 1S
  digitalWrite(LED,LOW);  //LOW is set to about 5V PIN8
  delay(100);  
  
  
}
