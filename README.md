# GPSlocator

This project is a library that implements the following functionalities:

- It parses NMEA GGA messages from a GPS module to extract the device position
- It measures the distance from a fixed position (target) to know if the device is within a defined range (100m)


## Clone the repository

To download the repository:

```
$ git clone https://github.com/miguelden/GPSlocator.git
```

## Build the library

Configure and build the library and tests

```
$ cd GPSlocator
$ mkdir -p build
$ cd build
$ cmake ..
$ make all
```

This commands will generate the library in the folder ```build/module```

## Testing

To execute the unit testing, the static code analysis and generate the test coverage report, execute the test script

```
$ cd GPSlocator
$ ./scripts/tests.sh
```

This script provides the unit tests results:
```
[==========] 33 tests from 6 test suites ran. (1 ms total)
[  PASSED  ] 33 tests.
```

The undefined behaviours and dangerous code constructs detected by the static analysis.
```
5/5 files checked 100% done
```

The coverage report in html format will be located at ```reports/html/coverage.html```

## Usage

An example of usage of the library could be to read a byte from the GPS UART, execute the app_step() with the read 
value and update the output signals. 

```c
    while (1)
    {
        uint8_t sz;
        uint8_t b;
        
        /* Read NMEA data from GPS */
        sz = read_uart_data(&b, 1);
        if (sz) {
            app_step(b);
        }
        
        /* Update GPS FIX LED (Blue) */
        GPIO_SetValue(LED_BLUE, userif_get_gps_status()==0? 0 : 1);
        /* Update TARGET On Range LED (Green) */
        GPIO_SetValue(LED_GREEN, userif_get_target_reached()==0? 0 : 1);
    }
```

