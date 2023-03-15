#define TOLERANCE 50
#define LOW 0
void setup() {
  // put your setup code here, to run once:
  pinMode(0, INPUT);
  Serial.begin(9600);

}
/**
 * This code assumes that the input will be a clean digial square wave
 * @return the binary value of the decoded input
 */
int readInput(){
  float halfPeriod = pulseIn(0,HIGH); //this function reads the amount of time the signal is high
  float fullPeriod = halfPeriod*2; //this accounts for the part of the wave where it is low
  float freqApprox = 1/fullPeriod; //frequency is the inverse of period
  if ((freqApprox >= 2125-TOLERANCE) && (freqApprox <= 2125+TOLERANCE)){ //the 1 case
    return 1;
  }
  else if ((freqApprox >= 2295-TOLERANCE) && (freqApprox <= 2295+TOLERANCE)){ //the 0 case
    return 0;
  }
  else { //undefined case
    return -1;
  }
}
/**
 * @param the input data frame
 * @return the character in the frame
 * takes a byte of binary data from the uart and returns the ascii char 
 */
char reconstituteData(byte inByte){
  int inInt = inByte; //implicit cast from Byte to Integer is lossless and therefore can be used here
  return inInt; //implicit cast from Int to Character is lossless and is also used 
}

void loop() {
  // put your main code here, to run repeatedly:
  String output = "";
  if (digitalRead(0) == 0){ //when the data is pulled down for the start bit, begin the reading process
    Serial.print("reading 0");
    byte frameData;
    frameData = frameData^frameData; //make sure that all the bits in the byte are zero to start
    for (int i=0;i<7;i++){
      delay(22); //wait 22 ms for the start bit to end
      int d0 = digitalRead(0);
      if (d0 != -1){
       frameData |= (d0 << i);
      }
    }
    //delay(22); //we should wait another 22 ms for the stop bit to fully transmit before we 
               //start polling for another frame
    char outChar = reconstituteData(frameData); //get the data from the input
    output.concat(outChar); //add the character we pulled to the output
    Serial.println(output); //output everything we've got the serial monitor
    Serial.println("Ladies and Gentlemen, We Got 'Em");
  }
  else if (readInput() == 1){
    Serial.print("reading 1");
  }
}
