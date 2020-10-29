#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
unsigned long setup_time;

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
  
  // Initialize the INA219.
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  // Starting time
  setup_time = millis();
}

// Variables for sum
float sum_shuntvoltage = 0;
float sum_busvoltage = 0;
float sum_current_mA = 0;
float sum_loadvoltage = 0;
float sum_power_mW = 0;

// Count loop times to get average
int times = 0;

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  // Run for 10 seconds
  if (millis() - setup_time < 10000) {
    times += 1;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    /*
    Serial.print(busvoltage); Serial.print(", ");
    Serial.print(shuntvoltage); Serial.print(", ");
    Serial.print(loadvoltage); Serial.print(", ");
    Serial.print(current_mA); Serial.print(", ");
    Serial.print(power_mW); Serial.println("");
    */

    sum_shuntvoltage += shuntvoltage;
    sum_busvoltage += busvoltage;
    sum_current_mA += current_mA;
    sum_loadvoltage += loadvoltage;
    sum_power_mW += power_mW;

    delay(10);
  } else {
    // Get average
    float avg_shuntvoltage = sum_shuntvoltage/times;
    float avg_busvoltage = sum_busvoltage/times;
    float avg_current_mA = sum_current_mA/times;
    float avg_loadvoltage = sum_loadvoltage/times;
    float avg_power_mW = sum_power_mW/times;

    Serial.println("Bus(V), Shunt(mV), Load(V), Current(mA), Power(mW)");
    Serial.print(avg_busvoltage); Serial.print(", ");
    Serial.print(avg_shuntvoltage); Serial.print(", ");
    Serial.print(avg_loadvoltage); Serial.print(", ");
    Serial.print(avg_current_mA); Serial.print(", ");
    Serial.print(avg_power_mW); Serial.println("");

    delay(10);
    exit(0);
  }
}
