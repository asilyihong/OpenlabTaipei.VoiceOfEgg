#ifndef __CONFIG_H__
#define __CONFIG_H__

/* feature */
#define LED_BLINK    1
#define SERVO_ROTATE 0
#define VOICE        1

/* Pin define */
#define SERVER_PIN		2
#define SD_SDCS_PIN             4
#define SD_CS_PIN              10
#define ULTRASONIC_TRIGGER_PIN  5
#define ULTRASONIC_ECHO_PIN     6

/* Constant define */
#define FPS 25
#define FRAME_MS (1000 / FPS)
#define IDLE_INTERVAL (30 * 1000)

#define OPEN_START  0
#define OPEN_END    1
#define VOICE_START 2
#define VOICE_END   3
#define CLOSE_START 4
#define CLOSE_END   5

#ifdef LED_BLINK
# define LED_PIN    7
#endif

#ifdef SERVO_ROTATE
# define SERVO_PIN  3

/* Servo angle define */
# define SERVO_HIGH_ANGLE  170
# define SERVO_LOW_ANGLE    10

# define SERVO_HIGH_MS  (1000 + ((long)SERVO_HIGH_ANGLE * 1000 / 180))
# define SERVO_LOW_MS   (1000 + ((long)SERVO_LOW_ANGLE * 1000 / 180))
#endif

#ifdef VOICE
# define SPEAKER_PIN 
#endif

#ifdef LED_BLINK
# define BLINK_INTERVAL 500 /* milliseconds */
#endif

/**
 * ExtEggStatus
 * +-------------+-------------+
 * |Manual/Detect|  Open/Close |
 * +-------------+-------------+
 * Detect: 0
 * Manual: 1
 * Close : 0
 * Open  : 1
 */
#define STATUS 0x00000001 /* Open/Close */
#define MODE   0x00000002 /* Manual/Detect */

#endif // __CONFIG_H__
