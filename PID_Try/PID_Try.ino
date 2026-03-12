
const int water_height_lower_bound_cm = 20;
//const int pump_pin = 9;
const int trig_pin = 25;
const int echo_pin = 33;
const int sampletime = 1000;
const int LED_PIN = 21;


unsigned long last_time;
double kp, ki, kd;
double integral, previous_data_real, output;
double outputMIN, outputMAX;
double data_real;


void distance_calculator_cm() {

digitalWrite(trig_pin, LOW);
delayMicroseconds(2);
digitalWrite(trig_pin, HIGH);
delayMicroseconds(10);
digitalWrite(trig_pin, LOW);

long duration = pulseIn(echo_pin, HIGH, 30000);
data_real = duration * 0.034 / 2;
 
Serial.print("Distance :"); Serial.println(data_real);
}

void PID () {
unsigned long time_now = millis();
double dt = time_now - last_time;

if(dt >= sampletime)
{ 
  double error = water_height_lower_bound_cm - data_real;
  
  integral += (ki * error);
  if(integral > outputMAX) integral = outputMAX;
  if(integral < outputMIN) integral = outputMIN;

  double dDt_rl = (data_real - previous_data_real);

  output = (kp * error) + integral + (kd * dDt_rl);
  if(output > outputMAX) output = outputMAX;
  if(output < outputMIN) output = outputMIN;


last_time = time_now;
previous_data_real = data_real;
 
}

}

void setup() {
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(LED_PIN, INPUT);
  //pinMode(pump_pin, OUTPUT);

  outputMIN = 0;
  outputMAX = 100;

  kp = 3;
  ki = 0.001;
  kd = 1;

}

void loop() {
  distance_calculator_cm();
  PID();
  
  Serial.print("PID :"); Serial.println(output);
  int pump_input = map(output, 0, 100, 0, 127);
  Serial.print("PWM :"); Serial.println(pump_input);
  analogWrite(LED_PIN, pump_input);
  
  delay(60);
}
