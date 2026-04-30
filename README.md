# My Short PhyComp Lesson at IED Milan 2026

Short physical computing lesson materials for IED Milan 2026 (Design for Commons, 4th-5th year).

## Project Structure

- `arduino/` - Arduino IDE example for ESP8266 D1 mini
- `esphome/` - ESPHome YAML example for ESP8266 D1 mini

## Arduino

### Install Arduino IDE

1. Download Arduino IDE from the [official Arduino website](https://www.arduino.cc/en/software).
2. Install it with default settings.
3. Open Arduino IDE once to complete first-time setup.

### Install ESP8266 Board Support

1. Open Arduino IDE and go to `File > Preferences`.
2. In `Additional Boards Manager URLs`, add:
   - `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Go to `Tools > Board > Boards Manager`.
4. Search for `esp8266` and install `esp8266 by ESP8266 Community`.
5. Select your board from `Tools > Board` (for D1 mini, usually `LOLIN(WEMOS) D1 R2 & mini`).

### Example Code

See `arduino/d1mini_a0_serial.ino`.

## ESPHome

### Install ESPHome

You can install ESPHome in two common ways:

- **With Home Assistant Add-on**
  1. Open Home Assistant.
  2. Go to `Settings > Add-ons`.
  3. Install and start the `ESPHome` add-on.

- **With Python (CLI)**
  1. Install Python 3.
  2. Run:
     - `pip install esphome`
  3. Verify installation:
     - `esphome version`

### Example Configuration

See `esphome/d1mini_a0_serial.yaml`.
