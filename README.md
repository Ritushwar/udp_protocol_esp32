# ESP32 UDP Examples

This repository contains small tests and examples that demonstrate sending and receiving data over UDP using ESP32 devices connected over Wi‑Fi. The tests live in the `test/` folder and are arranged to help you validate hardware (LEDs / buttons), networking (finding recipients, sending/receiving UDP packets), and simple time-stamping using NTP.

## What is UDP?

UDP (User Datagram Protocol) is a simple transport protocol that provides connectionless, best-effort delivery of datagrams between hosts. Key points:

- Connectionless: no setup/teardown handshake is required. Each packet (datagram) is sent independently.
- Low overhead and low latency: good for simple status messages, telemetry, and streaming where occasional packet loss is acceptable.
- No delivery guarantees: packets can be lost, duplicated, or arrive out of order. If you need reliability, you must add it at the application level.

Because of its simplicity, UDP is commonly used on microcontrollers (like the ESP32) when you want minimal latency and small code size.

## How ESP32s send data over UDP using Wi‑Fi

Typical flow when using UDP on an ESP32 (Arduino/PlatformIO style):

1. Connect to a Wi‑Fi network (station mode) with `WiFi.begin(ssid, password)` or set up an access point.
2. Create a UDP object (e.g. `WiFiUDP udp;`).
3. To send data: call `udp.beginPacket(remoteIP, port)`, `udp.write()` (or `udp.print()`), then `udp.endPacket()`.
4. To receive data: call `udp.parsePacket()` to check for a new packet, then read with `udp.read()` or `udp.readBytes()` and inspect `udp.remoteIP()` / `udp.remotePort()` for sender info.
5. Optionally use broadcast or multicast addresses for discovery, or maintain a configuration of receiver IP/port for direct unicast.

Error modes and considerations:

- Since UDP doesn't guarantee delivery, include sequence numbers or acknowledgements if you need to detect loss.
- Packets should be kept small to avoid fragmentation (typical safe size is below the LAN MTU, e.g., < 1400 bytes).
- Use retry/backoff or simple handshakes at the application layer when necessary.

## Tests in the `test/` folder (sequential explanations)

Below are short descriptions of each file inside the `test/` folder in the sequence you requested. I have kept explanations concise; if you want a line-by-line walkthrough I can open each file and expand these summaries.

- `test/led_test.cpp`
  - Purpose: Verify the ESP32 board and its onboard LED (or a connected LED) work correctly.
  - Typical behavior: configure an output pin in `setup()` and toggle it in `loop()` with delays to blink the LED. Useful as a quick hardware sanity check before networking tests.

- `test/button_test.cpp`
  - Purpose: Verify a button input and its interaction with the MCU.
  - Typical behavior: configures a GPIO as input (with or without pull-up), debounces button presses, and reacts to presses. Often used to trigger an action such as sending a UDP packet or toggling the LED.

- `test/checking_receiver_ip.cpp`
  - Purpose: Help discover or validate the IP address of the UDP receiver.
  - Typical behavior: either prints the device's local IP (to know where other nodes can reach it) or listens for discovery packets and reports sender IP/port. This is useful when the receiver's address may change (DHCP) and you need to confirm where to send packets.

- `test/sending_data_udp.cpp` ("sending_data")
  - Purpose: Send UDP datagrams to a known receiver IP and port.
  - Typical behavior: connects to Wi‑Fi, creates a `WiFiUDP` instance, and periodically builds/sends packets with status `beginPacket()` / `print()` / `endPacket()`.

- `test/receiving_data.cpp` ("receiving_data")
  - Purpose: Receive and process incoming UDP datagrams.
  - Typical behavior: binds to a local port using `udp.begin(localPort)`, calls `udp.parsePacket()` in `loop()`, reads the packet payload with `udp.read()`and prints/logs the contents over serial or triggers actions based on the data.

- `test/testing_time_with_ntp_server.cpp` ("testing_time")
  - Purpose: Demonstrate getting the current time from an NTP server.
  - Typical behavior: synchronizes the system time via NTP (e.g., using `configTime()`), queries an NTP pool (like `pool.ntp.org`), and prints a human-readable timestamp. Useful to ensure the ESP32 has a correct clock before sending time-stamped messages.

- `test/time_stamp_sender.cpp`
  - Purpose: Send a timestamped UDP message to a receiver.
  - Typical behavior: gets the current time (possibly from NTP) and sends it in a UDP datagram to a configured receiver IP/port. Can be used to measure latency or sync events across devices in future test.

- `test/time_stamp_receiver.cpp`
  - Purpose: Receive timestamped UDP messages and process or display them.
  - Typical behavior: listens on a UDP port, reads incoming timestamp binary timestamps, prints them to serial, and print in serial monitor.

- `test/connect_with_ros_node.cpp` 
  In this testing i connect the esp with ros2 node using udp protocol, required ros2 node code is at raspberry_pi repository of mine github inside the demo_pkg.
  - Purpose: Connect with ros2 node and receive the string message send by the ros2 node and blink the led based on the string length received
  - Typical Behaviour: Listem on a UDP port, reads incoming string, print in the serial monitor and blink the led.


