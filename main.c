#include <stdio.h>
#include <stdint.h>
#include "ringbuffer.h"

typedef struct {
    uint8_t latitude_valid;
    double latitude;
    uint8_t longitude_valid;
    double longitude;
    uint32_t timestamp;
} thingiq_location_data_t;

typedef struct {
    uint16_t fuelSystemStatus; /*!< 0x03 */
    uint32_t engineSpeed; /*!< 0x0C */
    uint64_t runTime; /*!< 0x1F */
    uint8_t engineLoad; /*!< 0x04 */
    uint8_t coolantTemperature; /*!< 0x05 */
    uint8_t fuelPressure; /*!< 0x0A */
    uint8_t vehicleSpeed; /*!< 0x0D */
    uint8_t fuelRailPressure; /*!< 0x22 */
    uint8_t fuelRailGaugePressure; /*!< 0x23 */
    uint8_t fuelLevel; /*!< 0x2F */
    uint32_t timestamp;
} DataSTN1110_t;


typedef struct {
    thingiq_location_data_t location;
    DataSTN1110_t stn1110;
} thingiq_data_t;


#define RING_BUFFER_SIZE 100

struct ring_buf ring_buffer;
thingiq_data_t ring_buffer_data[RING_BUFFER_SIZE];


int main(void) {
    ring_buf_init(&ring_buffer, sizeof(ring_buffer_data), (uint8_t *) ring_buffer_data);


    // // Create fake data
    // thingiq_data_t fake_data;
    //
    // // Initialize location data
    // fake_data.location.latitude_valid = 1;
    // fake_data.location.latitude = 21.028511;  // Hanoi coordinates
    // fake_data.location.longitude_valid = 1;
    // fake_data.location.longitude = 105.804817;
    // fake_data.location.timestamp = 1715432700;  // Current timestamp
    //
    // // Initialize STN1110 data
    // fake_data.stn1110.fuelSystemStatus = 0x0201;
    // fake_data.stn1110.engineSpeed = 2500;
    // fake_data.stn1110.runTime = 3600;
    // fake_data.stn1110.engineLoad = 65;
    // fake_data.stn1110.coolantTemperature = 82;
    // fake_data.stn1110.fuelPressure = 45;
    // fake_data.stn1110.vehicleSpeed = 60;
    // fake_data.stn1110.fuelRailPressure = 70;
    // fake_data.stn1110.fuelRailGaugePressure = 72;
    // fake_data.stn1110.fuelLevel = 75;
    // fake_data.stn1110.timestamp = 1715432700;
    //
    //
    // // Put data into ring buffer
    // uint32_t bytes_written = ring_buf_put(&ring_buffer, (uint8_t *)&fake_data, sizeof(fake_data));
    // if (bytes_written != sizeof(fake_data)) {
    //     printf("Failed to write to ring buffer. Bytes written: %u\n", bytes_written);
    //     return -1;
    // }
    // printf("Successfully written %u bytes to ring buffer\n", bytes_written);
    //
    // // Get data from ring buffer
    // thingiq_data_t read_data;
    // uint32_t bytes_read = ring_buf_get(&ring_buffer, (uint8_t *)&read_data, sizeof(read_data));
    // if (bytes_read != sizeof(read_data)) {
    //     printf("Failed to read from ring buffer. Bytes read: %u\n", bytes_read);
    //     return -1;
    // }
    //
    // // Print retrieved data
    // printf("\n--- Retrieved Data ---\n");
    // printf("Location:\n");
    // printf("  Latitude valid: %u\n", read_data.location.latitude_valid);
    // printf("  Latitude: %.6f\n", read_data.location.latitude);
    // printf("  Longitude valid: %u\n", read_data.location.longitude_valid);
    // printf("  Longitude: %.6f\n", read_data.location.longitude);
    // printf("  Timestamp: %u\n", read_data.location.timestamp);
    //
    // printf("\nSTN1110 Data:\n");
    // printf("  Fuel System Status: 0x%04X\n", read_data.stn1110.fuelSystemStatus);
    // printf("  Engine Speed: %u RPM\n", read_data.stn1110.engineSpeed);
    // printf("  Run Time: %llu seconds\n", read_data.stn1110.runTime);
    // printf("  Engine Load: %u%%\n", read_data.stn1110.engineLoad);
    // printf("  Coolant Temperature: %u°C\n", read_data.stn1110.coolantTemperature);
    // printf("  Fuel Pressure: %u kPa\n", read_data.stn1110.fuelPressure);
    // printf("  Vehicle Speed: %u km/h\n", read_data.stn1110.vehicleSpeed);
    // printf("  Fuel Rail Pressure: %u kPa\n", read_data.stn1110.fuelRailPressure);
    // printf("  Fuel Rail Gauge Pressure: %u kPa\n", read_data.stn1110.fuelRailGaugePressure);
    // printf("  Fuel Level: %u%%\n", read_data.stn1110.fuelLevel);
    // printf("  Timestamp: %u\n", read_data.stn1110.timestamp);
    //
    // return 0;

    printf("Testing ring buffer with %d elements\n", RING_BUFFER_SIZE);

    // Create and push 100 fake data entries
    for (int i = 0; i < 200; i++) {
        thingiq_data_t data;

        // Initialize fake location data
        data.location.latitude_valid = 1;
        data.location.latitude = 40.0 + (i * 0.1);
        data.location.longitude_valid = 1;
        data.location.longitude = -74.0 - (i * 0.1);
        data.location.timestamp = 1000000 + i;

        // Initialize fake STN1110 data
        data.stn1110.fuelSystemStatus = 0x0200 + (i % 3); // Varying system status
        data.stn1110.engineSpeed = 2000 + i;
        data.stn1110.runTime = 3600 + (i * 10); // Increasing runtime
        data.stn1110.engineLoad = 60 + (i % 30); // Varying engine load between 60-89%
        data.stn1110.coolantTemperature = 80 + (i % 20);
        data.stn1110.fuelPressure = 40 + (i % 15); // Varying fuel pressure
        data.stn1110.vehicleSpeed = 50 + (i % 30);
        data.stn1110.fuelRailPressure = 65 + (i % 25); // Varying fuel rail pressure
        data.stn1110.fuelRailGaugePressure = 70 + (i % 20); // Varying gauge pressure
        data.stn1110.fuelLevel = 75 - (i % 50);
        data.stn1110.timestamp = 1000000 + i;
        // Put data into ring buffer
        uint32_t bytes_written = ring_buffer_put_overwrite(&ring_buffer, (uint8_t *) &data, sizeof(data));
        if (bytes_written != sizeof(data)) {
            printf("Failed to write to ring buffer. Bytes written: %u\n", bytes_written);
            return -1;
        }
        printf("Successfully written %u bytes to ring buffer\n", bytes_written);
        // printf("ring_buf_space_get returned %d\n", ring_buf_space_get(&ring_buffer));
        printf("Successfully pushed %d elements\n", i + 1);
        printf("Sample data: lat=%.2f, long=%.2f, timestamp=%d, speed=%d, fuel level=%d, fuel pressure=%d, fuel rail pressure=%d, fuel rail gauge pressure=%d, engine load=%d, coolant temp=%d, runtime=%llu, fuel system status=0x%04X, engine speed=%d, timestamp=%d\n",
               data.location.latitude,
               data.location.longitude,
               data.location.timestamp,
               data.stn1110.vehicleSpeed,
               data.stn1110.fuelLevel,
               data.stn1110.fuelPressure,
               data.stn1110.fuelRailPressure,
               data.stn1110.fuelRailGaugePressure,
               data.stn1110.engineLoad,
               data.stn1110.coolantTemperature,
               data.stn1110.runTime,
               data.stn1110.fuelSystemStatus,
               data.stn1110.engineSpeed,
               data.stn1110.timestamp);
    }

    printf("Finished pushing elements. Now retrieving...\n");

    // Retrieve all 100 elements
    for (int i = 0; i < RING_BUFFER_SIZE; i++) {
        thingiq_data_t read_data;
        if (ring_buf_get(&ring_buffer, (uint8_t *) &read_data, sizeof(read_data)) != sizeof(read_data)) {
            printf("Failed to read element %d from ring buffer\n", i);
            break;
        }
        printf("Successfully retrieved %d elements\n", i + 1);
        printf("Sample data: lat=%.2f, long=%.2f, timestamp=%d, speed=%d, fuel level=%d, fuel pressure=%d, fuel rail pressure=%d, fuel rail gauge pressure=%d, engine load=%d, coolant temp=%d, runtime=%llu, fuel system status=0x%04X, engine speed=%d, timestamp=%d\n",
               read_data.location.latitude,
               read_data.location.longitude,
               read_data.location.timestamp,
               read_data.stn1110.vehicleSpeed,
               read_data.stn1110.fuelLevel,
               read_data.stn1110.fuelPressure,
               read_data.stn1110.fuelRailPressure,
               read_data.stn1110.fuelRailGaugePressure,
               read_data.stn1110.engineLoad,
               read_data.stn1110.coolantTemperature,
               read_data.stn1110.runTime,
               read_data.stn1110.fuelSystemStatus,
               read_data.stn1110.engineSpeed,
               read_data.stn1110.timestamp);
    }

    printf("Ring buffer test completed\n");
    return 0;
}
