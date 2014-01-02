#ifndef TL_MODEL_ENUMS_H
#define TL_MODEL_ENUMS_H

/* This is the structure of the GtkListStore returned by get_devices */
enum
{
	TL_DEVICE_MODEL_NAME_COLUMN = 0,
	TL_DEVICE_MODEL_DEV_ID_COLUMN,
	TL_DEVICE_MODEL_N_COLUMNS
};

/* This is the structure of the GtkListStore returned by scan_channels & load_channels */
enum
{
	TL_CHANNEL_MODEL_VCHANNEL_COLUMN = 0,
	TL_CHANNEL_MODEL_STATION_COLUMN,
	TL_CHANNEL_MODEL_FREQ_COLUMN,
	TL_CHANNEL_MODEL_PROGRAM_ID_COLUMN,
	TL_CHANNEL_MODEL_N_COLUMNS
};

#endif
