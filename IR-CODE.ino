
      //int ledPin=12;  //choose pin for LED
   
   int inputPin=15;  //choose pin for IRsensor
          
      int val=0;  //variable for reading pin status

void setup()
{
 
  pinMode (LED_BUILTIN,OUTPUT);   //declare LED as output

  pinMode (inputPin,INPUT  );//declare IRsensor as input
 }
void loop()
{
   val = digitalRead(inputPin);  //read input value
   if (val==HIGH)
 {
  digitalWrite(LED_BUILTIN,LOW);
   Serial.println("Led low");//turn LED OFF
 }
    else
   {
   digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("Led high");//LED ON
      }
  }
