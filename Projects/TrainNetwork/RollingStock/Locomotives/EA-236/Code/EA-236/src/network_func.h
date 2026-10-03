#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework
#include <EEPROM.h>	// Non-volatile memory

// --- Network & Web Server --
#include <WiFi.h>
#include <Update.h>	// OTA update support
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <WebSerial.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>

// --------------------------------------------------------------------------
// NETWORK & WI-FI FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Creates an Access Point and starts the DNS server.
 */
void create_ap();

/**
 * @brief Attempts to connect to an existing Wi-Fi network.
 *
 * @param ssid Wi-Fi SSID.
 * @param password Wi-Fi password.
 */
void connect_wifi(const char *ssid, const char *password);

/**
 * @brief Reads saved Wi-Fi credentials and attempts to connect if present.
 */
void connect_wifi_eeprom();

/**
 * @brief Initializes all Web Server routes and API endpoints.
 *        Binds HTTP GET/POST requests to specific lambda functions.
 */
void init_server();

/**
 * @brief Gets new server requests.
 */
void process_server();