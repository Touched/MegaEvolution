#ifndef CONFIG_H_
#define CONFIG_H_

/* Key stone variables */

#define KEYSTONE_PLAYER_VAR 0x4001 /* Variable that holds item index for player keystone */
#define KEYSTONE_OPPONENT_VAR 0x4002 /* Same as above but for partner */
#define KEYSTONE_DEFAULT 0x161	/* Default Key Stone item index */

/* Options */
/* Prevent rival name being substituted in for trainer class 0x51 and 0x5A */
// #define NO_RIVAL_NAME_SWAP

/* Prevent keystone checking */
// #define DISABLE_KEYSTONE_CHECK

/* Ignored colours for Mega Trigger */

#define TRIGGER_NUM_IGNORED_COLORS 0x4 /* Number of ignored colors in list below */
#define TRIGGER_IGNORED_COLORS 0x2147, 0x424F, 0x31AA, 0x00E4 /* List */

	/* 0x7FFF,0x3757,0x7B97,0x31AA,0x72F1,0x424F,0x2147,0x429E,
	   0x4953,0x00E4,0x6ADB,0x37BD,0x0000,0x0000,0x0000,0x0000, */
#endif /* CONFIG_H_ */
