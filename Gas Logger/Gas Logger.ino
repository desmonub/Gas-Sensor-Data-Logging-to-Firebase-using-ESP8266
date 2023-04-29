#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <TimeLib.h>
#include <SPI.h> 

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

int Gas_analog = A0;

FirebaseData firebaseData;
FirebaseJson firebaseJson;

FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

void setup()
{
  pinMode(Gas_analog, INPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  firebaseConfig.database_url = "database_url";
  firebaseConfig.signer.tokens.legacy_token = "YOUR_LEGACY_TOKEN_HERE";

  Firebase.begin(&firebaseConfig, &firebaseAuth);
}

void loop()
{
  int gassensorAnalog = analogRead(Gas_analog);
  Serial.print("Gas Sensor: ");
  Serial.println(gassensorAnalog);
    
  if (gassensorAnalog > 300) {
    Serial.println("Gas");
  }

  firebaseJson.add("gas_sensor_analog", gassensorAnalog);
  firebaseJson.add("timestamp", String(now()));

  if (Firebase.pushJSON(firebaseData, "/Gas", firebaseJson)) {
    Serial.println("Data sent to Firebase successfully");
  } else {
    Serial.println("Data not sent to Firebase");
    Serial.println(firebaseData.errorReason());
  }
}
