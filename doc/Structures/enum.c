
// CONTEXT
typedef enum {
  EASY,
  MEDIUM,
  HARD
} Difficulty;

// MOVE
typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN
} Direction;

char* Direction_value[] = {
  "LEFT",
  "RIGHT",
  "UP",
  "DOWN"
};

// INVENTORY
typedef enum {
  LOOT,
  POWERUP
} ItemType;

// DEBUG
typedef enum {
  INFO,
  WARN,
  ERR
} errorLevel;

const char* errorLevel_value[] = {
  "INFO",
  "WARN",
  "ERR "
};
