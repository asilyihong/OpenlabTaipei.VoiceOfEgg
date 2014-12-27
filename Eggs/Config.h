/* feature */
#define LED_BLINK    1
#define SERVO_ROTATE 1
#define VOICE        1

/* Pin define */
#define SERVER_PIN 2

/* Constant define */
#define FPS 25
#define FRAME_MS 1000 / FPS

#ifdef LED_BLINK
# define LED_PIN   13
#endif

#ifdef SERVO_ROTATE
# define SENSOR_PIN 3

/* Servo angle define */
# define SERVO_HIGH_ANGLE  170
# define SERVO_LOW_ANGLE    10

# define SERVO_HIGH_MS  1000 + (SERVO_HIGH_ANGLE * 1000 / 180)
# define SERVO_LOW_MS   1000 + (SERVO_LOW_ANGLE * 1000 / 180)
#endif

#ifdef VOICE
# define SPEAKER_PIN 
#endif

#ifdef LED_BLINK
# define BLINK_INTERVAL 500 /* milliseconds */
#endif


typedef enum {
    MANUAL_OPEN = 0,
    MANUAL_CLOSE = MANUAL_OPEN + 1,
    DETECT_OPEN = MANUAL_OPEN + 2,
    DETECT_CLOSE = MANUAL_OPEN + 3
} E_externalEggStatus;
