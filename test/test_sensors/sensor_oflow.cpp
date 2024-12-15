#include <unity.h>
#include "globals.h"
#include "data_struct.h"
#include "SensorWrapperSingleton.h"

#include <unity.h>

void test_example()
{
    TEST_ASSERT_EQUAL(2, 1 + 1); // Ein einfacher Test
}

void test_setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_example);
    UNITY_END();
}

void test_loop() {}

// /**
//  * @brief
//  * Test der SensorWrapperSingleton Klasse
//  *
//  */

// SensorWrapperSingleton *sensors;
// TDataSensors sensorData;
// TSensorCFG sensorCFG;

// uint16_t deltaX, deltaY;

// void setUp()
// {
//     sensors = SensorWrapperSingleton::getInstance(
//         &Serial2,
//         PIN_CS_PMW3901,
//         &sensorCFG);
// }

// void tearDown()
// {
//     delete sensors;
// }

// void test_OFlow()
// {
//     TEST_ASSERT_EQUAL_INT8(16 / 4, 4);
// }

// //--------------------------------------------------------------
// //
// //--------------------------------------------------------------
// void setup()
// {
//     UNITY_BEGIN();
//     RUN_TEST(test_OFlow);
//     UNITY_END();
// }

// void loop()
// {
// }
