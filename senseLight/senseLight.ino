float lux;
int sensePin = A1;


void setup(){
pinMode(A1, INPUT);
pinMode(8, OUTPUT);
Serial.begin(9600);
}

void loop(){
  lux = analogRead(A1);
  Serial.println(lux);

  if(lux<750){
    digitalWrite(8, HIGH);
  } else{
    digitalWrite(8, LOW);
     }  
}

