unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
int temp_read_Delay = 100;

//PID variables
float elapsedTime, Time, timePrev;
float errorR=0;
float errorPR=0;
float IntegralToplamR=0;
float DerivR=0;
float ProportionalPartR=0;
float IntegralPartR=0;
float DerivativePartR=0;
float OutR1=0;
int OutR=0;


float KP=300;
float KI=500;
float KD=25;
int range=60;

int pwm = 3;
float sys_out = A0;

float real_temperature = 0;
int setpoint = 30;

void setup() {
  Serial.begin(9600);
  pinMode(sys_out, INPUT);
  pinMode(pwm, OUTPUT);

  attachInterrupt(0, zero_crossing, RISING);
  
}




void loop() {
  
  // put your main code here, to run repeatedly:
  currentMillis = millis();           //Save the value of time before the loop
  /*  We create this if so we will read the temperature and change values each "temp_read_Delay"
      value. Change that value above iv you want. The MAX6675 read is slow. Tha will affect the
      PID control. I've tried reading the temp each 100ms but it didn't work. With 500ms worked ok.*/
  if (currentMillis - previousMillis >= temp_read_Delay) {
    previousMillis += temp_read_Delay;              //Increase the previous time for next loop
    float sys_out = analogRead(A0);  //get the real temperature in Celsius degrees
    real_temperature = 25+sys_out/29.42;

    errorR = setpoint - real_temperature;    //Calculate the pid ERROR
    timePrev = Time;                    // the previous time is stored before the actual time read
    Time = millis();                    // actual time read
    elapsedTime = (Time - timePrev) / 1000;

    IntegralToplamR = IntegralToplamR + elapsedTime * (errorR + errorPR) / 2.00;
    if (IntegralToplamR >= 10000.0) {
      IntegralToplamR = 10000.0;
    }
    if (IntegralToplamR <= -10000.0) {
      IntegralToplamR = -10000.0;
    }

    DerivR = (errorR - errorPR) / elapsedTime;

    ProportionalPartR = KP * errorR;

    IntegralPartR = KI * IntegralToplamR;

    DerivativePartR = KD * DerivR;

    OutR1 = ProportionalPartR + IntegralPartR + DerivativePartR;

    if (OutR1 >= 7500) {
      OutR = 7500;
    }
    else if (OutR1 <= 0) {
      OutR = 1000;
    }
    else {
      OutR = OutR1;
    }

    errorPR = errorR;
    Serial.println(real_temperature);

    
    while(!Serial.available()) {}

    
    if (Serial.available() >0)
    {
      setpoint = Serial.read();
      
    }
   
    
  }


}

void zero_crossing(){
    delayMicroseconds(10000 - OutR); //This delay controls the power
      //Serial.println("AAAA");
      //Serial.println(OutR);
      digitalWrite(3,HIGH);         
      delayMicroseconds(100);
      digitalWrite(3,LOW);
}

















  //
  //
  //
  //                               //integral constant will only affect errors below 30ºC
  //    PID_i = 0;
  //
  //    PID_p = kp * PID_error;                         //Calculate the P value
  //
  //    PID_i = PID_i + (ki * PID_error);               //Calculate the I value
  //
  //
  //    PID_d = kd * ((PID_error - previous_error) / elapsedTime); //Calculate the D value
  //
  //    PID_value = PID_p + PID_i + PID_d;                      //Calculate total PID value
  //
  //    if (PID_value < 0)
  //    {
  //      PID_value = 0;
  //    }
  //    if (PID_value > 10000)
  //    {
  //      PID_value = 10000;
  //    }
  //    previous_error = PID_error; //Remember to store the previous error.
  //  }
  //
  //  analogWrite(pwm, PID_value);
  //  //If the zero cross interruption was detected we create the 100us firing pulse
  //
  //  Serial.println(real_temperature);



//}
