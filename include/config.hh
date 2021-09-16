typedef enum
{
	Pl2B = 65536,
	Pl9S = 66048,
	PlA2 = 65792
} player_choice_t;

typedef struct
{
	struct
	{
		bool Enabled;
		int MinObjectId;
		int MaxObjectId;
	} EntityOverlay;

	bool EnableBuffedEnemies;

	bool EnableLevelScaling;
	bool EnablePlayerForcing;
	player_choice_t PlayerForcingValue;
} config_t;

extern config_t gConfig;