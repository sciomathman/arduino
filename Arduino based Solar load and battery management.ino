const int r1 =  3; /*battery relay control assigned to D3 */
const int r2 =  4; /* load relay control assigned to D4 */
boolean newcycle = false; /* starts with battery charging : Morning sunlight */
unsigned long counter;
unsigned long loadOn;
unsigned long loadOffBatteryOff;
unsigned long oneDay;
boolean loadFlag;
boolean loadBatFlag;
unsigned long currentmillis;
unsigned long lastmillis;
unsigned long startupOffset;

void setup() {
  // put your setup code here, to run once:
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  digitalWrite(r1, HIGH);
  digitalWrite(r2, LOW);
  oneDay = hourstomillis(24.0);
  loadFlag = false;
  loadBatFlag = false;
  loadOn = hourstomillis(12.0);
  loadOffBatteryOff = hourstomillis(13.5);
  startupOffset = 21600000; //system started at 1.00 pm
  counter = 0;
  Serial.begin(9600);
  Serial.flush();
  Serial.println(oneDay);
  Serial.println(loadOn);
  Serial.println(loadOffBatteryOff);
}

void loop() {
  // put your main code here, to run repeatedly:  
  currentmillis = millis();
  if(counter == 0 && newcycle == false) {
    newcycle = true;
    if(startupOffset != 0) {
      counter = startupOffset;
      startupOffset = 0;
    }
  }
  
  if(newcycle == true) {
    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
    newcycle = false;
    loadFlag = false;
    loadBatFlag = false;
    Serial.println("Bat On Load Off");
  }
  
  if(newcycle == false && counter >= loadOn && loadFlag == false) {
    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);  
    loadFlag = true;
    Serial.println("Bat On Load On");
  }

  if(newcycle == false && counter >= loadOffBatteryOff && loadBatFlag == false){
    digitalWrite(r1, LOW);
   digitalWrite(r2, LOW); 
   loadBatFlag = true;
   Serial.println("Bat Off Load Off");
  }  
  
  if(currentmillis < lastmillis ) {
    lastmillis = currentmillis + 5;
  }
  
  counter = counter + currentmillis - lastmillis;   
    
  if(counter >= oneDay) {
    newcycle = true; 
    counter = 0;
  }

  lastmillis = currentmillis;
  
}

unsigned long hourstomillis(float hours) {
  unsigned long result;
  result = hours*3600000;
  return result;
}
