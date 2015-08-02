
#define BANK_COUNT 4
#define BANK_SIZE 31

#define PACKET_START_BYTE 0xbb
#define PACKET_LEADIN_BYTE 0x88
#define PACKET_TRAIL_BYTE 0xFF

#define DEVICE_RESPONSE_TIMEOUT 5

// Command values from host system
#define PC_COMMAND_UPDATEBANK 98

// Idle waiting for a bank update command
#define PC_IDLE 0
// Got bank update command waiting on bank id to update
#define PC_WAITING_BANK 1
// Loading the bank
#define PC_LOADING_BANK 2

byte pcInputState = PC_IDLE;
byte bankUpdateId = 0;
byte bankUpdateRemaining = 0;

// Stores the data from the simulation
byte simulationData[BANK_COUNT][BANK_SIZE];
// Flags indicating if a bank is ready for transmission
boolean bankReady[BANK_COUNT];

boolean waitingPollingResponse = false;
byte pollingAddress = 0;
byte pollingBank = 0;
long pollingTimeout = 0;

void setup() {
  // Don't send unintialized banks
  for (int i=0; i<BANK_SIZE; i++) {
    bankReady[i] = false;
  }
  
  // Open serial connection to EOS Bus
  Serial1.begin(250000);

  // Open serial connection to PC
  Serial.begin(250000);
}

void loop() {
  processPcInput(Serial.available());

  if (waitingPollingResponse) {
    if ((long)(millis() - pollingTimeout) >= 0) {
      waitingPollingResponse = false;
    }
  } else {
    sendPollingPacket();
  }
}

void sendPollingPacket() {
  Serial1.write(PACKET_START_BYTE);
  Serial1.write(PACKET_LEADIN_BYTE);
  Serial1.write(0x80 | pollingAddress);
  Serial1.write(0x00);
  if (bankReady[pollingBank]) {
    Serial1.write(BANK_SIZE);
    Serial1.write(simulationData[pollingBank], BANK_SIZE);
  } else {
    Serial1.write(0x00);
  }
  Serial1.write(0xff);
  pollingAddress++;
  pollingBank++;
  waitingPollingResponse = true;
  pollingTimeout = millis() + DEVICE_RESPONSE_TIMEOUT;
}

// Parsing loop for pc input status.
void processPcInput(int incommingBytes) {
  if (incommingBytes > 0) {
    switch (pcInputState) {
      case PC_IDLE:
        parsePcCommand(Serial.read());
      case PC_WAITING_BANK:
        parsePcBankId(Serial.read());
        break;
      case PC_LOADING_BANK:
        loadPcBank(incommingBytes);
        break;
    }
  }
}

// Check to see if we got a bank load command. If not drop the byte.
void parsePcCommand(int command) {
  if (command == PC_COMMAND_UPDATEBANK) {
    pcInputState = PC_WAITING_BANK;
  }
}

// Look for a bankId
void parsePcBankId(int bankId) {
  // Serial bus could be empty on this call
  if (bankId > -1) {
    // Check for valid bank id.  If id is valid get ready to 
    // load.  Otherwise start looking for bank load command again.
    if (bankId >= 0 && bankId < BANK_COUNT) {
      bankUpdateId = bankId;
      bankUpdateRemaining = BANK_SIZE;
      bankReady[bankUpdateId] = false;
    } else {
      pcInputState = PC_WAITING_BANK;
    }
  }
}

// Load as much of the bank as we can from the incoming data
void loadPcBank(int incommingBytes) {
  int readAsk = bankUpdateRemaining < incommingBytes ? bankUpdateRemaining : incommingBytes;
  bankUpdateRemaining -= Serial.readBytes(simulationData[bankUpdateId], readAsk);
  if (bankUpdateRemaining == 0) {
    bankReady[bankUpdateId] = true;
    pcInputState = PC_IDLE;
  }
}

