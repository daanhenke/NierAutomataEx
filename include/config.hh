typedef enum
{
	Pl2B = 65536,
	Pl9S = 66048,
	PlA2 = 65792
} player_choice_t;

typedef struct
{
	bool EnableEntityOverlay;
	bool EnableLevelScaling;
	bool EnablePlayerForcing;
	player_choice_t PlayerForcingValue;
} config_t;

extern config_t gConfig;