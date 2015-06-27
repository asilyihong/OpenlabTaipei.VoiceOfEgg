#ifndef __DEFS_H__
#define __DEFS_H__

/* feature */
#define LED_BLINK    1
#define SERVO_ROTATE 1
#define VOICE        1

/* Pin define */
#define ULTRASONIC_TRIGGER_PIN  5
#define ULTRASONIC_ECHO_PIN     6
#define CUM_RX                  8
#define CUM_TX                  9
#define LIGHT_SENSOR_PIN        A0

/* Constant define (ID indep.) */
#define FPS 25
#define FRAME_MS (1000 / FPS)

#define OPEN_START  0
#define OPEN_END    1
#define VOICE_START 2
#define VOICE_END   3
#define CLOSE_START 4
#define CLOSE_END   5

#ifdef LED_BLINK
# define LED1_PIN    10
# define LED2_PIN    11
#endif

#ifdef SERVO_ROTATE
# define SERVO_PIN  3

/* Servo angle define */
# define SERVO_HIGH_ANGLE  160
# define SERVO_LOW_ANGLE    20

# define SERVO_HIGH_MS  (750 + ((long)SERVO_HIGH_ANGLE * 1500 / 180))
# define SERVO_LOW_MS   (750 + ((long)SERVO_LOW_ANGLE * 1500 / 180))
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

#endif // __DEFS_H__
