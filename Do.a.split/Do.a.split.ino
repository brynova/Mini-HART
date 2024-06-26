#include <DynamixelSDK.h>

#define ADDR_AX_TORQUE_ENABLE          24
#define ADDR_AX_GOAL_POSITION          30
#define ADDR_AX_MOVING_SPEED           32
#define ADDR_AX_PRESENT_POSITION       36  // Address to read the current position
#define PROTOCOL_VERSION               1.0
#define BAUDRATE                       1000000
#define DEVICENAME                     "3"

dynamixel::PortHandler *portHandler;
dynamixel::PacketHandler *packetHandler;
uint8_t dxl_error = 0; // Variable to hold Dynamixel error codes
int dxl_comm_result = COMM_TX_FAIL; // Variable to hold the result of Dynamixel communication operations

void enableTorque(int id);
void disableTorque(int id);
void setMovingSpeed(int id, int speed);
void moveMotor(int id, float angle, int time);
int angleToPosition(float angle, int id);
int getCurrentPosition(int id); // Function to read the current position of a motor

void setup() {
    Serial.begin(57600);
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);

    if (!portHandler->openPort() || !portHandler->setBaudRate(BAUDRATE)) {
        Serial.println("Failed to open port or set baud rate");
        return;
    }

    for (int id = 1; id <= 12; id++) {
        enableTorque(id);
        moveMotor(id, 0, 200); // Initialize motors to stand position
    }
    delay(1000);

     // Begin stationary walk movement
    // Step 1: Squat Down - Aligning HP and AP
    moveMotor(  1,     0, 400); // HY Right
    moveMotor(  2,     0, 400); // HR Right
    moveMotor(  3,    40, 400); // HP Right
    moveMotor(  4,   -80, 400); // KP Right
    moveMotor(  5,   -40, 400); // AP Right
    moveMotor(  6,     0, 400); // AR Right
    moveMotor(  7,     0, 400); // HY Left
    moveMotor(  8,     0, 400); // HR Left 
    moveMotor(  9,    40, 400); // HP Left
    moveMotor( 10,   -80, 400); // KP Left
    moveMotor( 11,   -40, 400); // AP Left
    moveMotor( 12,     0, 400); // AR Left
    delay(500);

    // Step 2: Rotate Right Roll motors to shift COB to right leg, Raise left left and twiat right hip as well
    moveMotor(  1,    20, 400); // HY Right
    moveMotor(  2,     0, 400); // HR Right
    moveMotor(  3,    40, 400); // HP Right
    moveMotor(  4,   -80, 400); // KP Right
    moveMotor(  5,   -40, 400); // AP Right
    moveMotor(  6,     0, 400); // AR Right
    moveMotor(  7,    20, 400); // HY Left
    moveMotor(  8,     0, 400); // HR Left 
    moveMotor(  9,    40, 400); // HP Left
    moveMotor( 10,   -80, 400); // KP Left
    moveMotor( 11,   -40, 400); // AP Left
    moveMotor( 12,     0, 400); // AR Left
    delay(1000);

    // Step 3: Return to Squat
    moveMotor(  1,    20, 600); // HY Right
    moveMotor(  2,   -80, 600); // HR Right
    moveMotor(  3,    40, 600); // HP Right
    moveMotor(  4,   -80, 600); // KP Right
    moveMotor(  5,   -40, 600); // AP Right
    moveMotor(  6,   -80, 600); // AR Right
    moveMotor(  7,    20, 600); // HY Left
    moveMotor(  8,   -80, 600); // HR Left 
    moveMotor(  9,    40, 600); // HP Left
    moveMotor( 10,   -80, 600); // KP Left
    moveMotor( 11,   -40, 600); // AP Left
    moveMotor( 12,   -80, 600); // AR Left
    delay(1000);

    // Step 4: Rotate Left Roll motors to shift COB to left leg, Raise Right leg and twist right hip as well
    moveMotor(  1,    20, 600); // HY Right
    moveMotor(  2,     0, 600); // HR Right
    moveMotor(  3,    40, 600); // HP Right
    moveMotor(  4,   -80, 600); // KP Right
    moveMotor(  5,   -40, 600); // AP Right
    moveMotor(  6,     0, 600); // AR Right
    moveMotor(  7,    20, 600); // HY Left
    moveMotor(  8,     0, 600); // HR Left 
    moveMotor(  9,    40, 600); // HP Left
    moveMotor( 10,   -80, 600); // KP Left
    moveMotor( 11,   -40, 600); // AP Left
    moveMotor( 12,     0, 600); // AR Left
    delay(1000);


    moveMotor(  1,     0, 400); // HY Right
    moveMotor(  2,     0, 400); // HR Right
    moveMotor(  3,    40, 400); // HP Right
    moveMotor(  4,   -80, 400); // KP Right
    moveMotor(  5,   -40, 400); // AP Right
    moveMotor(  6,     0, 400); // AR Right
    moveMotor(  7,     0, 400); // HY Left
    moveMotor(  8,     0, 400); // HR Left 
    moveMotor(  9,    40, 400); // HP Left
    moveMotor( 10,   -80, 400); // KP Left
    moveMotor( 11,   -40, 400); // AP Left
    moveMotor( 12,     0, 400); // AR Left
    delay(1000);

    moveMotor(  1,     0, 500); // HY Right
    moveMotor(  2,     0, 500); // HR Right
    moveMotor(  3,    -5, 500); // HP Right
    moveMotor(  4,     0, 500); // KP Right
    moveMotor(  5,     0, 500); // AP Right
    moveMotor(  6,     0, 500); // AR Right
    moveMotor(  7,     0, 500); // HY Left
    moveMotor(  8,     0, 500); // HR Left 
    moveMotor(  9,    -5, 500); // HP Left
    moveMotor( 10,     0, 500); // KP Left
    moveMotor( 11,     0, 500); // AP Left
    moveMotor( 12,     0, 500); // AR Left
    delay(100);


    for (int id = 1; id <= 12; id++) {
        enableTorque(id);
        moveMotor(id, 0, 200); // Initialize motors to stand position
    }
    delay(4000);
    
    // The process would be repeated for the other side as needed
    // Cleanup and disable torque
    for (int id = 1; id <= 12; id++) {
        disableTorque(id);
    }

    portHandler->closePort();
}

void enableTorque(int id) {
    packetHandler->write1ByteTxRx(portHandler, id, ADDR_AX_TORQUE_ENABLE, 1, &dxl_error);
}

void disableTorque(int id) {
    packetHandler->write1ByteTxRx(portHandler, id, ADDR_AX_TORQUE_ENABLE, 0, &dxl_error);
}

void setMovingSpeed(int id, int speed) {
    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, ADDR_AX_MOVING_SPEED, speed, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        Serial.println("Failed to set moving speed!");
    }
}

void moveMotor(int id, float angle, int time) {
    // Adjust angle for motors on one side, if necessary
    if (id >= 7 && id <= 12) {
        angle *= -1;
    }
    
    // Get current and goal positions
    int currentPosition = getCurrentPosition(id);
    int goalPosition = angleToPosition(angle, id);

    // Calculate and set speed if time is valid
    if (time > 0) {
        int movementMagnitude = abs(goalPosition - currentPosition);
        int speed = (movementMagnitude * 1000 / time) / 2.286;
        
        // Manually constrain the speed to be non-negative and no greater than 1023
        if(speed < 0){
            speed = -speed;  // Make speed positive if negative
        }
        if(speed > 1023){
            speed = 1023;  // Constrain speed to a maximum of 1023
        }

        setMovingSpeed(id, speed); // Set motor speed
    }

    // Move motor to goal position
    packetHandler->write2ByteTxRx(portHandler, id, ADDR_AX_GOAL_POSITION, goalPosition, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        Serial.println("Failed to move motor!");
    }
}


int angleToPosition(float angle, int id) {
    return (int)(((angle + 150) / 300) * 1023);
}

// Function to read the current position of a motor
int getCurrentPosition(int id) {
    uint16_t dxl_present_position;
    dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, ADDR_AX_PRESENT_POSITION, &dxl_present_position, &dxl_error);
    if (dxl_comm_result == COMM_SUCCESS) {
        return dxl_present_position;
    } else {
        Serial.println("Failed to read present position!");
        return -1; // Handle error appropriately
    }
}

void loop() {
    // Empty - Actions are taken in setup
}