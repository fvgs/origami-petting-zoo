#include <CapacitiveSensor.h>

// Pins
#define SEND_PIN 4
#define RECEIVE_PIN 2
#define LED_PIN 13
#define SWITCH_PIN 8

#define SENSOR_SAMPLES 30
#define EVENT_THRESHOLD 4
#define EVENT_HIT_INTERVAL 700

// Configurable constants
#define RECAL_INTERVAL 10000
#define SOUND_DELAY 3000
#define CAPACITANCE_THRESHOLD 30

CapacitiveSensor cs;
long last_recal;
short event_counter;
long time_of_last_event_hit;

void setup()
{
    // Necessary magic
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SWITCH_PIN, OUTPUT);

    // Initialize sensor
    cs = CapacitiveSensor(SEND_PIN, RECEIVE_PIN);

    // Initialize last recalibration time
    last_recal = millis();

    event_counter = 0;
    time_of_last_event_hit = 0;
}

void loop()
{
    long capacitance = cs.capacitiveSensor(SENSOR_SAMPLES);
    long current_time = millis();

    if (current_time - last_recal > RECAL_INTERVAL) {
      cs.reset_CS_AutoCal();
      last_recal = millis();
    }

    // Reset event counter if max event duration exceeded
    if (current_time - time_of_last_event_hit > EVENT_HIT_INTERVAL) {
        reset_event_counter();
    }

    // Register an event hit
    if (capacitance > CAPACITANCE_THRESHOLD) {
        time_of_last_event_hit = current_time;
        event_counter++;
    }

    // Sufficient event hits to perform action
    if (event_counter >= EVENT_THRESHOLD) {
        reset_event_counter();
        makeSound();
    }

    Serial.print("Capacitance:\t");
    Serial.print(capacitance);
    Serial.print("\n");
}

void reset_event_counter()
{
    event_counter = 0;
}

/**
 * Leanna please add the respective sounds
 */
void makeSound()
{
    // Make the sound
    Serial.print("RAAAAAAAAAAAAAWR");

    delay(SOUND_DELAY);
}
