#include <Wire.h>
#include <Encoder.h>
 

#define  SLAVE_ADDRESS         0x29  //slave address, any number from 0x01 to 0x7F
#define  REG_MAP_SIZE             32
#define  MAX_SENT_BYTES       3
#define  IDENTIFICATION           0x0D
 
// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder wheel_1(2, 4);
Encoder wheel_2(3, 5);


/********* Global  Variables  ***********/

byte registerMap[REG_MAP_SIZE];
byte registerMapTemp[REG_MAP_SIZE - 1];
byte receivedCommands[MAX_SENT_BYTES];
byte newDataAvailable = 0;
byte useInterrupt = 1;
byte modeRegister = 0;
byte configRegister = 0;
byte zeroB = 0;
byte zeroC = 0;
byte zeroBData = 0;
byte zeroCData = 0;
byte encoderStatus = 1;
 
long wheel_1_delta = 0;
long wheel_2_delta = 0;
long wheel_1_speed = 0;
long wheel_2_speed = 0;

static uint32_t loopTimer;


void setup()
{
     Wire.begin(SLAVE_ADDRESS); 
     Wire.onRequest(requestEvent);
     Wire.onReceive(receiveEvent);
     registerMap[13] = IDENTIFICATION;
    Serial.begin(57600);
    Serial.println("BallanceBot Wheel Encoder Ready");
}



long wheel_1_position  = -999;
long wheel_2_position = -999;

void loop()
{

  if(zeroB || zeroC)
  {
//    Serial.println("Configuration request.");
    newDataAvailable = 0;  //let the master know we don’t have any post correction data yet
    changeModeConfig();  //call the function to make your changes
    return; //go back to the beginning and start collecting data from scratch
  }
  
    long new_1, new_2;
    uint32_t timer = micros();
    
    newDataAvailable=0;
    
    new_1 = wheel_1.read();
    new_2 = wheel_2.read();

    // find elapsed time since last read
    float dt  = (float)(timer - loopTimer) / 1000000.0;

    // prevent a big buildup, read at a least 10hz
    dt = min(dt, .15);

    loopTimer       = timer;

     wheel_1_delta = new_1 - wheel_1_position;
     wheel_2_delta = new_2 - wheel_2_position;

    // Find velocity of rotation
    wheel_1_speed += wheel_1_delta / dt;
    wheel_2_speed += wheel_2_delta / dt; 
//    
//    if (new_1 != wheel_1_position || new_2 != wheel_2_position) {
// 
//    Serial.print("Left = ");
//    Serial.print(new_1);
//    Serial.print(", Right = ");
//    Serial.print(new_2);
//    Serial.print("  v1 = ");
//    Serial.print("  d1 = ");
//    Serial.print(wheel_1_delta);
//    Serial.print("  d2 = ");
//    Serial.print(wheel_2_delta);
//    Serial.print("  dt = ");
//    Serial.print(dt);
//    Serial.print(wheel_1_speed);
//    Serial.print("  v1 = ");
//    Serial.print(wheel_1_speed);
//    Serial.print("  v2 = ");
//    Serial.print(wheel_2_speed);
//    Serial.println();
//    }
//    
    wheel_1_position = new_1;
    wheel_2_position = new_2;
    
   storeData();
   newDataAvailable=1;


}


void storeData()
{
  byte * bytePointer;  //we declare a pointer as type byte
  byte arrayIndex = 0x10; //we need to keep track of where we are storing data in the array
  registerMapTemp[0] = encoderStatus;  //no need to use a pointer for EncoderStatus
  bytePointer = (byte*)&wheel_1_position; //wheel_1_delta is 4 bytes
  for (int i = 0; i < 5; i++)
  {
    registerMapTemp[arrayIndex] = bytePointer[i];  //increment pointer to store each byte
    arrayIndex++;
  }
  arrayIndex=0x14;
  bytePointer = (byte*)&wheel_2_position; //wheel_2_delta is 4 bytes
  for (int i = 0; i < 5; i++)
  {
    registerMapTemp[arrayIndex] = bytePointer[i];  //increment pointer to store each byte
    arrayIndex++;  
}
  arrayIndex=0x18;
  bytePointer = (byte*)&wheel_1_speed; //wheel_1_speed is 4 bytes
  for (int i = 0; i < 5; i++)
  {
    registerMapTemp[arrayIndex] = bytePointer[i];  //increment pointer to store each byte
    arrayIndex++;
  }
    arrayIndex=0x1c;
    bytePointer = (byte*)&wheel_2_speed; //wheel_2_speed is 4 bytes
  for (int i = 0; i < 5; i++)
  {
    registerMapTemp[arrayIndex] = bytePointer[i];  //increment pointer to store each byte
    arrayIndex++;
  }
  registerMapTemp[arrayIndex] = modeRegister;
  arrayIndex++;
  registerMapTemp[arrayIndex] = configRegister;
}

//void storeData()
//{
//  byte * bytePointer;
//  byte arrayIndex = 1;
//  bytePointer = (byte*)&wheel_1_position;
//  for (int i = 0; i < 5; i++)
//  {
//    registerMapTemp[arrayIndex] = bytePointer[i];
//    arrayIndex++;
//  }
//}
 
void changeModeConfig()
{
  /*Put your code here to evaluate which of the registers need changing
   And how to make the changes to the given device.  For our GPS example
   It could be issuing the commands to change the baud rate, update rate,
   Datum, etc…
   We just put some basic code below to copy the data straight to the registers*/
   if(zeroB)
      {
        modeRegister = zeroBData;
        Serial.println("Resetting position");
        wheel_1.write(0);
        wheel_2.write(0);
        zeroB = 0;  //always make sure to reset the flags before returning from the function
        zeroBData = 0;
      }
      if(zeroC)
      {
        configRegister = zeroCData;
        zeroC = 0;  //always make sure to reset the flags before returning from the function
        zeroCData = 0;
      }

}

//void requestEvent()
//{
//
//     Wire.write(registerMap, REG_MAP_SIZE);  //Set the buffer up to send all 14 bytes of data
//
//}

void requestEvent()
{
  if(newDataAvailable)
  {
    for (int c = 0; c < (REG_MAP_SIZE-1); c++)
    {
      registerMap[c] = registerMapTemp[c];
    }
  } 
  newDataAvailable = 0;
  //Set the buffer to send all 14 bytes
  Wire.write(registerMap + receivedCommands[0], REG_MAP_SIZE); 
} 


void receiveEvent(int bytesReceived)
{
  for (int a = 0; a < bytesReceived; a++)
  {
    if ( a < MAX_SENT_BYTES)
    {
      receivedCommands[a] = Wire.read();
    }
    else
    {
      Wire.read();  // if we receive more data then allowed just throw it away
    }
  }
  if(bytesReceived == 1 && (receivedCommands[0] < REG_MAP_SIZE))
  {
    return; 
  }
  if(bytesReceived == 1 && (receivedCommands[0] >= REG_MAP_SIZE))
  {
    receivedCommands[0] = 0x00;
       return;
  }
  switch(receivedCommands[0]){
  case 0x0B:
    zeroB = 1; //this variable is a status flag to let us know we have new data in register 0x0B
    zeroBData = receivedCommands[1]; //save the data to a separate variable
    bytesReceived--;
    if(bytesReceived == 1)  //only two bytes total were sent so we’re done
    {
      return;
    }
    zeroC = 1;
    zeroCData = receivedCommands[2];
    return; //we can return here because the most bytes we can receive is three anyway
    break;
  case 0x0C:
    zeroC = 1;
    zeroCData = receivedCommands[1];
    return; //we can return here because 0x0C is the last writable register
    break;
  default:
    return; // ignore the commands and return
  }
}

