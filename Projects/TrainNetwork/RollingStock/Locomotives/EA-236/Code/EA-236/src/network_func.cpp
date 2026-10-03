#include "network_func.h"
#include "storage_func.h"
#include "train.h"
#include "config.h"
#include "captive_portals.h"

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Network & Server ---
AsyncWebServer server(80); // Async web server on port 80
DNSServer dns_server; // Handles portal redirection
bool ota_in_progress = false; // Skips DNS while a firmware upload is on

// --- System Status ---
bool app_confirmed = false; // Status flag for handshake
bool ap_active = true; // Status flag for access point

// --------------------------------------------------------------------------
// NETWORK & WI-FI FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

void create_ap() {
	WiFi.onEvent([](WiFiEvent_t event) {
        if (ARDUINO_EVENT_WIFI_STA_GOT_IP == event) {
            WebSerial.println("IP Address: " + WiFi.localIP().toString());
        }
    });
	WiFi.softAP(AP_SSID, AP_PASSWORD);
	
	// Routing all DNS requests to the ESP32's AP IP 
  	// This triggers the device to pop up a captive portal
	dns_server.start(53, "*", WiFi.softAPIP());

	// Enabling local domain name (access via http://ea-236.local)
	MDNS.begin("ea-236");
	ap_active = true;
}

void connect_wifi(const char *ssid, const char *password) {
	WiFi.begin(ssid, password);
}

void connect_wifi_eeprom() {
	// Starting the web serial
	WebSerial.begin(&server);

	String wifi_ssid = EEPROM.readString(SSID_ADDRESS);
	String wifi_password = EEPROM.readString(PASSWORD_ADDRESS);
	
	// Trying to connect to Wi-Fi if something is stored in memory
	if (0 < wifi_ssid.length() && 0 < wifi_password.length()) {
    	connect_wifi(wifi_ssid.c_str(), wifi_password.c_str());
  	}
}

void init_server() {
	// Root of the server. Returns the main config portal HTML page
  	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    	request->send(200, "text/html", config_portal);
  	});

	// Specific route for Android devices for showing the config portal
	server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
    	request->send(200, "text/html", config_portal);
  	});

	// Specific route for IOS devices for showing the config portal
	server.on("/hotspot-detect.html", HTTP_GET,
			  [](AsyncWebServerRequest *request) {
    	request->send(200, "text/html", config_portal);
  	});

	// Retrieves user input from the config portal, saves credentials to
    // EEPROM and attempts a Wi-Fi connection
	server.on("/config", HTTP_POST, [](AsyncWebServerRequest *request) {
    	String wifi_ssid = request->arg("wifiSSID");
		String wifi_password = request->arg("wifiPassword");
		if (SSID_ADDRESS == wifi_ssid.length() ||
			PASSWORD_ADDRESS <= wifi_ssid.length() ||
			SSID_ADDRESS == wifi_password.length() ||
			PASSWORD_ADDRESS <= wifi_password.length()) {
			request->send(400, "text/plain", "SSID or invalid password!");
        	return;
    	}
		store_credentials(wifi_ssid.c_str(), wifi_password.c_str());
    	connect_wifi(wifi_ssid.c_str(), wifi_password.c_str());
    	request->send(200, "text/html", config_portal);
  	});

	// Displays the connection confirmation page after a successful join
	server.on("/confirm", HTTP_GET, [](AsyncWebServerRequest *request) {
    	request->send(200, "text/html", confirm_portal);
	});

	// Utility endpoint used by the web interface to check if the ESP32 
    // successfully connected to the router
	server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    	if (WL_CONNECTED == WiFi.status()) {
      		request->send(200, "text/plain", WiFi.localIP().toString());
    	} else {
      		request->send(200, "text/plain", "Connection failed!");
    	}
  	});

	// Displays the HTML portal for uploading a .bin firmware file
	server.on("/ota", HTTP_GET, [](AsyncWebServerRequest *request) {
    	if(!request->authenticate(OTA_NAME, OTA_PASSWORD)) {
      		return request->requestAuthentication();
 		}
    	request->send(200, "text/html", ota_portal); 
  	});

	// Processes the uploaded firmware file
	// Response handler triggers when the upload completes entirely
	// Upload handler executes iteratively as data chunks arrive
	server.on("/upload", HTTP_POST,
    	[](AsyncWebServerRequest *request) {
			if (!request->authenticate(OTA_NAME, OTA_PASSWORD)) {
        		return request->requestAuthentication();
    		}
        	bool update_failed = Update.hasError();
        	AsyncWebServerResponse* response = request->beginResponse(
            update_failed ? 500 : 200,
            "text/plain", update_failed ? "Upload failed!" : "Restarting...");

        	response->addHeader("Connection", "close");
        	request->send(response);
        	delay(1000);

        	if (!update_failed) {
            	ESP.restart();
       		}
		},
    	nullptr,
    	[](AsyncWebServerRequest *request, uint8_t *data, size_t len,
       	size_t index, size_t total) {
        	if (!request->authenticate(OTA_NAME, OTA_PASSWORD)) return;

			if (!index) {
				if (ota_in_progress) {
           			Update.abort();
            		WebSerial.println("Previous incomplete upload aborted");
        		}
				ota_in_progress = true;
				WebSerial.printf("Upload started, total size: %u bytes\n",
					(unsigned)total);
				if (!Update.begin(total)) {
                Update.printError(WebSerial);
            }
        }
        	if (Update.write(data, len) != len) {
            	Update.printError(WebSerial);
        	}
        	if (index + len == total) {
            	if (Update.end(true)) {
                	WebSerial.println("Upload finished!");
            	} else {
                	Update.printError(WebSerial);
				}
				ota_in_progress = false;
			}
    	}
	);

	// Gets the target throttle
	server.on("/throttle", HTTP_GET, [](AsyncWebServerRequest* request) {
		if (request->hasParam("throttle")) {
			int raw_throttle = request->getParam("throttle")->value().toInt();
			raw_throttle = constrain(raw_throttle, 0, 1023);
			target_throttle = raw_throttle;
    	}
		request->send(200, "text/plain", "Throttle updated");
  	});

	// Initiates the braking sequence. Sets the changing flag to true, 
    // forcing the train to safely stop before applying the pending direction
	server.on("/direction", HTTP_GET,
		[](AsyncWebServerRequest* request) {
        	if (request->hasParam("direction")) {
			String raw_direction = request->getParam("direction")->value();
			if ((raw_direction == "F" && 'F' != direction) ||
				(raw_direction == "B" && 'B' != direction)) {
				changing_direction = true;
                pending_direction = (raw_direction == "F") ? 'F' : 'B';
            }
        }
		request->send(200, "text/plain", "Direction updated");
    });

	// Enables or disables the lights
	server.on("/lights", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("lights")) {
			lights = request->getParam("lights")->value() == "on";
		}
		request->send(200, "text/plain", "Lights updated");
  	});

	// Enables or disables the beams
	server.on("/beams", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("beams")) {
      		beams = request->getParam("beams")->value() == "on";
    	}
		request->send(200, "text/plain", "Beams updated");
  	});

	// Starts the track mapping sequence
	server.on("/mapping", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("mapping")) {
      		mapping = request->getParam("mapping")->value() == "on";
    	}
		request->send(200, "text/plain", "Mapping updated");
  	});

	// Starts the RFID reading/writing sequence
	server.on("/rfid", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("reading")) {
      		reading = request->getParam("reading")->value() == "on";
    	}
		request->send(200, "text/plain", "RFID updated");
  	});

	// Starts the gyroscope integration
	server.on("/gyro", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("gyro")) {
      		gyro = request->getParam("gyro")->value() == "on";
    	}
		request->send(200, "text/plain", "Gyro updated");
  	});

	// Starts the train engines
	server.on("/motors_on", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("motors_on")) {
      		motors_on = request->getParam("motors_on")->value() == "on";
    	}
		request->send(200, "text/plain", "Motors updated");
  	});

	// Starts the train horn sound
	server.on("/horn", HTTP_GET, [](AsyncWebServerRequest* request) {
    	if (request->hasParam("horn")) {
      		horn = request->getParam("horn")->value() == "on";
    	}
		request->send(200, "text/plain", "Horn updated");
  	});

	// Starts the asynchronous server
	server.begin();
}

void process_server() {
	if (!ota_in_progress) {
		dns_server.processNextRequest();
	}
}