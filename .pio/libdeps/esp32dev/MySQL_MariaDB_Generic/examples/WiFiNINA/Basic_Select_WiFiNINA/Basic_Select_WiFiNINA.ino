/*********************************************************************************************************************************
  Basic_Select_WiFiNINA.ino

  Library for communicating with a MySQL or MariaDB Server

  Based on and modified from Dr. Charles A. Bell's MySQL_Connector_Arduino Library https://github.com/ChuckBell/MySQL_Connector_Arduino
  to support nRF52, SAMD21/SAMD51, SAM DUE, STM32F/L/H/G/WB/MP1, ESP8266, ESP32, etc. boards using W5x00, ENC28J60, LAM8742A Ethernet,
  WiFiNINA, ESP-AT, built-in ESP8266/ESP32 WiFi.

  The library provides simple and easy Client interface to MySQL or MariaDB Server.

  Built by Khoi Hoang https://github.com/khoih-prog/MySQL_MariaDB_Generic
  Licensed under MIT license
 **********************************************************************************************************************************/
/*
  MySQL Connector/Arduino Example : basic select

  This example demonstrates how to issue a SELECT query with no parameters
  and use the data returned. For this, we use the Cursor class to execute
  the query and get the results.

  It demonstrates who methods for running queries. The first allows you to
  allocate memory for the cursor and later reclaim it, the second shows how
  to use a single instance of the cursor use throughout a sketch.

  NOTICE: You must download and install the World sample database to run
          this sketch unaltered. See http://dev.mysql.com/doc/index-other.html.

  CAUTION: Don't mix and match the examples. Use one or the other in your
           own sketch -- you'll get compilation errors at the least.

  For more information and documentation, visit the wiki:
  https://github.com/ChuckBell/MySQL_Connector_Arduino/wiki.

  INSTRUCTIONS FOR USE

  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Connect a USB cable to your Arduino
  4) Select the correct board and port
  5) Compile and upload the sketch to your Arduino
  6) Once uploaded, open Serial Monitor (use 115200 speed) and observe

  Note: The MAC address can be anything so long as it is unique on your network.

  Created by: Dr. Charles A. Bell
*/

#include "defines.h"
#include "Credentials.h"

#include <MySQL_Generic.h>

#define USING_HOST_NAME     true

#if USING_HOST_NAME
  // Optional using hostname, and Ethernet built-in DNS lookup
  char server[] = "your_account.ddns.net"; // change to your server's hostname/URL
#else
  IPAddress server(192, 168, 2, 112);
#endif

uint16_t server_port = 5698;    //3306;

char default_database[] = "world";       //"test_arduino";
char default_table[]    = "city";        //"test_arduino";

String default_column   = "population"; 
String default_value    = "Toronto"; 

// Sample query
String query = String("SELECT ") + default_column + " FROM " + default_database + "." + default_table
               + " WHERE name = '" + default_value + "'";

MySQL_Connection conn((Client *)&client);
// Create an instance of the cursor passing in the connection
MySQL_Query sql_query = MySQL_Query(&conn);

int status = WL_IDLE_STATUS;

void printWifiStatus()
{
  // print the SSID and IP address of the network you're attached to:
  MYSQL_DISPLAY3("SSID:", WiFi.SSID(), "IP Address:", WiFi.localIP());

  // print the received signal strength:
  MYSQL_DISPLAY2("Signal strength (RSSI):", WiFi.RSSI(), "dBm");
}

void setup()
{
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect

  MYSQL_DISPLAY1("\nStarting Basic_Select_WiFiNINA on", BOARD_NAME);
  MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    MYSQL_DISPLAY("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    MYSQL_DISPLAY("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    MYSQL_DISPLAY1("Attempting to connect to SSID:", ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
  }

  printWifiStatus();

  // End WiFi section

  MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
  MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);
}

void runQuery()
{
  row_values *row = NULL;
  long head_count = 0;

  MYSQL_DISPLAY("1) Demonstrating using a dynamically allocated query.");
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);
  
  // Execute the query
  MYSQL_DISPLAY(query);

  // Execute the query
  // KH, check if valid before fetching
  if ( !query_mem.execute(query.c_str()) )
  {
    MYSQL_DISPLAY("Querying error");
    return;
  }
  
  // Fetch the columns (required) but we don't use them.
  //column_names *columns = query_mem.get_columns();
  query_mem.get_columns();

  // Read the row (we are only expecting the one)
  do 
  {
    row = query_mem.get_next_row();
    
    if (row != NULL) 
    {
      head_count = atol(row->values[0]);
    }
  } while (row != NULL);

  // Show the result
  MYSQL_DISPLAY1("  Toronto pop =", head_count);

  delay(500);

  MYSQL_DISPLAY("2) Demonstrating using a local, global query.");
  
  // Execute the query
  MYSQL_DISPLAY(query);
  sql_query.execute(query.c_str());
  
  // Fetch the columns (required) but we don't use them.
  sql_query.get_columns();
  
  // Read the row (we are only expecting the one)
  do 
  {
    row = sql_query.get_next_row();
    if (row != NULL) 
    {
      head_count = atol(row->values[0]);
    }
  } while (row != NULL);
  
  // Now we close the cursor to free any memory
  sql_query.close();

  // Show the result but this time do some math on it
  MYSQL_DISPLAY1("  Toronto pop =", head_count);
  MYSQL_DISPLAY1("  Toronto pop increased by 11725 =", head_count + 11725);
}

void loop()
{
  MYSQL_DISPLAY("Connecting...");
  
  //if (conn.connect(server, server_port, user, password))
  if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
  {
    delay(500);
    runQuery();
    conn.close();                     // close the connection
  } 
  else 
  {
    MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration.");
  }

  MYSQL_DISPLAY("\nSleeping...");
  MYSQL_DISPLAY("================================================");
 
  delay(60000);
}
