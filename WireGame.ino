#include <LiquidCrystal.h>

/**
 * Define all inputs
 */
#define PIN_START   A0
#define PIN_END     A1
#define PIN_CONTACT A2


/**
 * Define all output
 */
#define LED_ON      7
#define LED_CONTACT 10

#define BUZZER      9

#define CONTACT_LED_LENGTH 100

boolean is_running = false;
boolean has_contact = false;
unsigned long contact_time;
unsigned int nb_contact = 0;
unsigned long game_time;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/**
 * Setup environment
 */
void setup()
{
    // Init pins mode
    pinMode(PIN_START, INPUT);
    pinMode(PIN_END, INPUT);
    pinMode(PIN_CONTACT, INPUT);
    pinMode(LED_ON, OUTPUT);
    pinMode(LED_CONTACT, OUTPUT);
  
    // LEDs initial state
    digitalWrite(LED_ON, LOW);
    digitalWrite(LED_CONTACT, HIGH);
  
    // activate pull-up
    digitalWrite(PIN_START, HIGH);
    digitalWrite(PIN_END, HIGH);
    digitalWrite(PIN_CONTACT, HIGH);

    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
    
    // Display welcome message
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bonjour Esteban :-)");
}

/**
 * Main loop
 */
void loop()
{
    // start game
    if (! is_running && digitalRead(PIN_START) == LOW) {
        initGame();
    }
    if (is_running) {
        // End game ?
        if (digitalRead(PIN_END) == LOW) {
            endGame();
        }
        // Contact ?
        if (digitalRead(PIN_CONTACT) == LOW) {
            if (! has_contact) {
                nb_contact++;
            }
            tone(BUZZER, 262, 100);
            has_contact = true;
            contact_time = millis();
            digitalWrite(LED_CONTACT, LOW);
        }
        // End of contact ?
        if (has_contact && (millis() - contact_time > CONTACT_LED_LENGTH)) {
            digitalWrite(LED_CONTACT, HIGH);
            has_contact = false;
        }
        displayScore();
    }
    delay(10);
}


/**
 * Initialize new game, reset values to initial states
 *
 * @return void
 */
void initGame()
{
    is_running = true;
    has_contact = false;
    nb_contact = 0;
    game_time = millis();
    digitalWrite(LED_CONTACT, HIGH);
}  // initGame

/**
 * End game and set data to sleeping mode
 *
 * @return void
 */
void endGame()
{
    is_running = false;
    has_contact = false;
    digitalWrite(LED_CONTACT, HIGH);
    displayScore();
}  // endGame

/**
 * Contact activated
 */
void displayScore()
{
    unsigned long game_length;
    game_length = (millis() - game_time) / 1000;
    lcd.clear();
    lcd.setCursor(0,0);
    // Print a message to the LCD.
    lcd.print("Temps : ");
    lcd.print(game_length);
    lcd.print("s");
    lcd.setCursor(0,2);
    lcd.print("Erreurs : ");
    lcd.print(nb_contact);
}
