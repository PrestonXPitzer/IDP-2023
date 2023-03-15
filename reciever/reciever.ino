void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT); //This is the input for the filter with frequency 2295
  pinMode(A1, INPUT); //This is the input for the filter with frequency 2125  
}
//*
//output, the signal that is high, if any (1:freq = 2125, 0:freq=2295, -1:freq=unknown)
//*
int doesTheSignalExist(){
  int tolerance = 224;
  int acceptedThreshold = 1024-tolerance;
  if (analogRead(A0) > acceptedThreshold){ //If the input signal is not attenuated beyond the threshold
    return 0;                              //Then the bit we are reading is 0 (2295 Hz)
  }
  if (analogRead(A1) > acceptedThreshold) { //If the input signal is not attenuated at this frequency
    return 1;                               //Then we are reading a 1 (2125 Hz)
  }
  return -1;                                //If the signal is attenuated, then it is outside of the 
                                            //passband for either center frequency
  
}
//*
//input, the data frame to check
//output, if the frame has an undefined value
//*
bool containsInvalid(int dataFrame[]){
  for (int i = 0;i<8;i++){
    if(dataFrame[i] == -1){
      return true;
    }
  }
  return false;
}
//*
//input, the 8 bits comprising a UART data frame
//output, the character represented by the data in the frame
//if the data is not a valid UART, then return a -1
//if it is a valid UART frame, then return the character
//corresponding to the data in the frame
//*
char decodeUART(int dataFrame[]){
  if (dataFrame[0] == 1){ //the first invalid frame case, the first bit should always be 0 in UART
    return -1;
  }
  if (dataFrame[9] == 0){ //the second invalid case, the last bit should always be 1
    return -1;
  }
  char reconstitutedChar = 0; //this is the eventual output, we take advantage of the numeric nature of c characters
  reconstitutedChar += (dataFrame[6]*1); //add the value of the LSB
  reconstitutedChar += (dataFrame[5]*2); //continue to add the rest of the bits
  reconstitutedChar += (dataFrame[4]*4); 
  reconstitutedChar += (dataFrame[3]*8); 
  reconstitutedChar += (dataFrame[2]*16); 
  reconstitutedChar += (dataFrame[1]*32); //add the value of the MSB in the data
  return reconstitutedChar;
} 
int getRelativeTimestamp(int start, int current){
  return current-start;                           
}
String output = ""; //define the output string in the global scope
void loop() {
  if (
}
