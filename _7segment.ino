#define SEGMENT_A 2
#define SEGMENT_B 3
#define SEGMENT_C 4
#define SEGMENT_D 5
#define SEGMENT_E 6
#define SEGMENT_F 7
#define SEGMENT_G 8

#define DIGIT_1 9
#define DIGIT_2 10
#define DIGIT_3 11
#define DIGIT_4 12

#define SOLAR_ADC_PIN 0

#define LASER_CONTROL_PIN 13

#define ON 1
#define OFF 0

#define DISPLAY_DELAY 2
#define SOLAR_READ_DELAY 20

#define DEFAULT_DISPLAY_LOOP 50

void segment_helper(int a, int b, int c, int d, int e, int f, int g)
{
  digitalWrite(SEGMENT_A, a); 
  digitalWrite(SEGMENT_B, b); 
  digitalWrite(SEGMENT_C, c); 
  digitalWrite(SEGMENT_D, d); 
  digitalWrite(SEGMENT_E, e); 
  digitalWrite(SEGMENT_F, f); 
  digitalWrite(SEGMENT_G, g); 
}

void write_num(int val)
{
    switch (val)
    {
      case 0: // A B C D E F -
        segment_helper(HIGH, HIGH, HIGH, HIGH, HIGH, HIGH,  LOW);
        break;
      case 1: // - B C - - - -
        segment_helper( LOW, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW);
        break;
      case 2: // A B - D E - G
        segment_helper(HIGH, HIGH,  LOW, HIGH, HIGH,  LOW, HIGH);
        break;
      case 3: // A B C D - - G
        segment_helper(HIGH, HIGH, HIGH, HIGH,  LOW,  LOW, HIGH);
        break;
      case 4: // - B C - - F G
        segment_helper( LOW, HIGH, HIGH,  LOW,  LOW, HIGH, HIGH);
        break;
      case 5: // A - C D - F G
        segment_helper(HIGH,  LOW, HIGH, HIGH,  LOW, HIGH, HIGH);
        break;
      case 6: // A - C D E F G
        segment_helper(HIGH,  LOW, HIGH, HIGH, HIGH, HIGH, HIGH);
        break;
      case 7: // A B C - - - -
        segment_helper(HIGH, HIGH, HIGH,  LOW,  LOW,  LOW,  LOW);
        break;
      case 8: // A B C D E F G
        segment_helper(HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH);
        break;
      case 9: // A B C D - F G
        segment_helper(HIGH, HIGH, HIGH, HIGH,  LOW, HIGH, HIGH);
        break;
      break;
      case '-':// - - - - - - G
        segment_helper( LOW,  LOW,  LOW,  LOW,  LOW,  LOW, HIGH);
        break;
      default: // - - - - - - -
        segment_helper( LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW);
        break;
      break;
    } 
}

void digit_state(int digit, int state)
{
  if (!(digit == DIGIT_1 || digit == DIGIT_2 || digit == DIGIT_3 || digit == DIGIT_4)) {
    return;
  }
  
  if (state == OFF) {
    pinMode(digit, INPUT);
  } else if (state == ON) {
    pinMode(digit, OUTPUT);
    digitalWrite(digit, LOW); 
  }
}

void display_4(int a, int b, int c, int d)
{
  write_num(a);
  digit_state(DIGIT_1, ON);
  delay(DISPLAY_DELAY);
  digit_state(DIGIT_1, OFF);
  delay(DISPLAY_DELAY);
  
  write_num(b);
  digit_state(DIGIT_2, ON);
  delay(DISPLAY_DELAY);
  digit_state(DIGIT_2, OFF);
  delay(DISPLAY_DELAY);
  
  write_num(c);
  digit_state(DIGIT_3, ON);
  delay(DISPLAY_DELAY);
  digit_state(DIGIT_3, OFF);
  delay(DISPLAY_DELAY);
  
  write_num(d);
  digit_state(DIGIT_4, ON);
  delay(DISPLAY_DELAY);
  digit_state(DIGIT_4, OFF);
  delay(DISPLAY_DELAY);
}

void display_4_time(int a, int b, int c, int d, int time)
{
  for (int i=0; i<time; i++) {
    display_4(a, b, c, d);
  }
}

int read_solar() {
  
  int read_times = 1 << 4; // 16 times
  int result = 0; // 16 bit ints, up to 32k
  
  for (int i=0; i<read_times; i++) {
    result += analogRead(SOLAR_ADC_PIN);
  }
  
  result = result >> 4;
  return result;
}

int display_4_solar() {
  
  int solar;
  int solar_1;
  int solar_2;
  int solar_3;
  int solar_4;

  solar_1 = 0;
  solar_2 = 0;
  solar_3 = 0;
  solar_4 = 0;
  solar = read_solar();
  
  solar_1 = solar % 10;
  solar /= 10;
  solar_2 = solar % 10;
  solar /= 10;
  solar_3 = solar % 10;
  solar /= 10;
  solar_4 = solar % 10;
  
  for (int i=0; i<DEFAULT_DISPLAY_LOOP; i++) {
    display_4(solar_4, solar_3, solar_2, solar_1);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SEGMENT_A, OUTPUT);
  pinMode(SEGMENT_B, OUTPUT);
  pinMode(SEGMENT_C, OUTPUT);
  pinMode(SEGMENT_D, OUTPUT);
  pinMode(SEGMENT_E, OUTPUT);
  pinMode(SEGMENT_F, OUTPUT);
  pinMode(SEGMENT_G, OUTPUT);
  
  pinMode(LASER_CONTROL_PIN, OUTPUT);
  
  digit_state(DIGIT_1, ON);
  digit_state(DIGIT_2, ON);
  digit_state(DIGIT_3, ON);
  digit_state(DIGIT_4, ON);

  display_4_time('-', '-', '-', '-', DEFAULT_DISPLAY_LOOP);
  display_4_time(0, 0, 0, 4, DEFAULT_DISPLAY_LOOP);
  display_4_time(0, 0, 0, 3, DEFAULT_DISPLAY_LOOP);
  display_4_time(0, 0, 0, 2, DEFAULT_DISPLAY_LOOP);
  display_4_time(0, 0, 0, 1, DEFAULT_DISPLAY_LOOP);
  
  digitalWrite(LASER_CONTROL_PIN, HIGH);
  
  for (int i=0; i<5; i++) {
    display_4_solar();
    delay(200);
  }
  
  digitalWrite(LASER_CONTROL_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  display_4_solar();

}
