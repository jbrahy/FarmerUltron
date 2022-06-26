#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

int water_sensor_pin = 1;
File configuration;
char alexa_api_key[] = "";
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // mac address of card
char server[] = "www.google.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

bool network_initialized = false;
bool configuration_read = false;


void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) { ; // wait for serial port to connect. Needed for native USB port only
    }

    // read configuration file
    configuration_read = read_configuration();
    network_initialized = initialize_network();

}

void loop() {

    if (configuration_read && network_initialized) {

        int current_water_level = check_water_level();

        if (current_water_level < 500) {
            alert_alexa("Farm stand needs water");
        }

    } else {
        Serial.println("Can't run without an Alexa API Key");
        exit;
    }

    sleep(1000); // wait a second
}


bool initialize_network() {

    // start the Ethernet connection:
    Serial.println("Initialize Ethernet with DHCP:");

    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            while (true) {
                delay(1); // do nothing, no point running without Ethernet hardware
            }
        }
        if (Ethernet.linkStatus() == LinkOFF) {
            Serial.println("Ethernet cable is not connected.");
        }
        // try to configure using IP address instead of DHCP:
        Ethernet.begin(mac, ip, myDns);
    } else {
        Serial.print("  DHCP assigned IP ");
        Serial.println(Ethernet.localIP());
    }

    return true;
}

void alert_alexa(char message) {
    // send message to notify me

}

int check_water_level() {
    return 100;
}

bool read_configuration() {

    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the SD library functions will not work.
    pinMode(10, OUTPUT);

    if (!SD.begin(4)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");

    if (SD.exists("example.txt")) {
        Serial.println("example.txt exists.");
    } else {
        Serial.println("example.txt doesn't exist.");
    }

    // open a new file and immediately close it:
    Serial.println("Creating example.txt...");
    myFile = SD.open("example.txt", FILE_WRITE);
    myFile.close();

    // Check to see if the file exists:
    if (SD.exists("example.txt")) {
        Serial.println("example.txt exists.");
    } else {
        Serial.println("example.txt doesn't exist.");
    }

    return true;

}