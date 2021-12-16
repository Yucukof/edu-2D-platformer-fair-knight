// CONTEXT
typedef struct {
  Score  board
    int  lootCollected;
    int  enemyKilled;
} Scoring;

typedef struct {
  Scoring  score;
    Level  currentLevel;
   Player  player1;
  Monster  gang[MAXMONSTERS];
   Bullet  volley[MAXBULLETS];
     Item  loot[MAXPOWERUPS];
      int  difficulty;
      int  timer;
} Context;

// MOVE
typedef struct {
        int  x, y;
        int vx,vy;
       char symbol;
  Direction look;
} Coordinates;

// PLAYER
typedef struct {
Coordinates  position;
    PowerUp  powerbank[MAXPOWERUPS];
        int  baseHealth;
        int  currentHealth;
        int  baseSpeed;
        int  currentSpeed;
       bool  isFalling;
} Player;

// MONSTER
typedef struct{
Coordinates  position;
        int  name;
        int  baseHealth;
        int  currentHealth;
        int  baseSpeed;
        int  currentSpeed;
        int  baseAttack;
        int  currentAttack;
       bool  isStunned;
        int  stunTimer;
        int  scoreValue;
} Monster;

// INVENTORY
typedef struct {
Coordinates   position;
        int   name;
        int   type;
        int   value;
} Item;

typedef struct {
        int   name;
        int   type;
        int   value;
       bool   active;
        int   timer;
} PowerUp;

typedef struct {
Coordinates   position;
        int   baseDamage;
        int   timer;
} Bullet;

// DEBUG
typedef struct {
  int   errorLevel;
  char  *errorLocation;
  char  *errorMsg;
} log;
