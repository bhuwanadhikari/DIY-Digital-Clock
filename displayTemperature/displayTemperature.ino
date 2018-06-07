

#define UP  8
#define DOWN  7
#define DIVIDER 13  
int led[7] = {6, 5, 4, 3, 2, 1, 0}; // serially a, b, c, d, e, f, g
int delayInMs = 3000;
  
int selector[4] = {9, 10, 11, 12}; // four digit selectors: first, second, third, fourth
int digit[10][7] = {    {1, 1, 1, 1, 1, 1, 0},
                        {0, 1, 1, 0, 0, 0, 0},
                        {1, 1, 0, 1, 1, 0, 1},
                        {1, 1, 1, 1, 0, 0, 1},
                        {0, 1, 1, 0, 0, 1, 1},
                        {1, 0, 1, 1, 0, 1, 1},
                        {1, 0, 1, 1, 1, 1, 1},
                        {1, 1, 1, 0, 0, 0, 0},
                        {1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 0, 1, 1}
                       };

int tempPin = A2;
int lightPin = A3;
int outPin = A1;
int temp, light;

void setup() {
  for (int i = 0; i<= 13; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(tempPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(outPin, OUTPUT);
  

}

void loop() {
  if(millis()%2000<20){
    temp = analogRead(tempPin);
    temp = (5.0*temp*1000.0)/(1023*10);
  }

    autoSetDivider();
    select(1);
    echo(digit[(temp/10)%10]);
      delayMicroseconds(delayInMs/1.5);
    reset();

    select(2);
    echo(digit[temp%10]);
      delayMicroseconds(delayInMs/1.5);
    reset();

    displayC();
      delayMicroseconds(delayInMs/1.5);
    reset();


}

//Helper functions

//HELPER FUNCTIONS-----------------------------------------
void echo(int arr[]){
  for(int i = 0; i<7; i++){
    if(arr[i] == 1){
      high(led[i]);
    } else{
      low(led[i]);
    }
  }
}



//------------------------------------------
void select(int x){
  for(int i = 0; i<4; i++){
    if(i == x-1){
      high(selector[i]);
    }else{
      low(selector[i]);
    }
  }
}
//-------------------------------------------
  void high( int x){
    digitalWrite(x, HIGH);
  }
//-------------------------------------------
  void low(int x){
    digitalWrite(x, LOW);
  }
//-------------------------------------------
  void reset(){
    for(int i = 0; i<=6; i++){
      low(i);
    }
     for(int i = 9; i<=13; i++){
      low(i);
    }
  }
//-------------------------------------------------
void autoSetDivider(){
    high(DIVIDER);
    high(UP);
    low(DOWN);
    delayMicroseconds(delayInMs);
    low(DIVIDER);
    low(UP);
    low(DOWN);
}
//-------------------------------------------------
  void displayC(){
    select(3);
    high(led[0]);
    low(led[1]);
    low(led[2]);
    high(led[3]);
    high(led[4]);
    high(led[5]);
    low(led[6]);
    delayMicroseconds(delayInMs);
    reset();
  }
