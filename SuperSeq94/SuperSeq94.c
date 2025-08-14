/*
There's something very sad about all of this
Building this
Learning this
There always was
The circumstances just make it seem particularly so
*/

#include <libdragon.h>
#include <string.h>
#include <joypad.h>
#include "lib/ui/ui_elements.h"
#include "lib/ui/ui_menu.h"
#include "lib/audio/synth.h"
#include "lib/util/font_util.h"
#include "lib/input/input.h"

// Mixer channel allocation
#define CHANNEL_SFX1    0
#define CHANNEL_SFX2    1
#define CHANNEL_MUSIC   2
// Input port allocation (easy to read)
#define DRUM_PORT 0
#define MELODIC_PORT 1
#define MOD_PORT 2
#define VISUAL_PORT 3

static volatile double t3 = 0.0; // increment every 1 sec

double lastScrub = 0.0;

//display_context_t disp;
sprite_t *starSprite;

/*
We should allow for saving of presets
and iterating through them via modulation
per project have a limit of the amount of presets that can be saved

Tracker style input with per channel timing capabilities
(maybe conditionals, if you can come up with a simple editor for them)

FX bus should be available

VISUALS:
For visual generation having something that tracks FPS and allows you to set a cleanup based on order
of execution to improve framerate might be a good idea to avoid crashes

FILE BROWSER:
This will need a simple file browser, output should be placed in appropriate folder from assets folder
Maybe with metadata this will be possible?
Script should parse assets folder and extract the file structure on make

Project
{
	Presets
	{	
		Instr
		{
			Sample
			Env 
			Lfo
		}
	}
	Sequences
	{
	
	}
}


CONTROLS:
DRUM_SEQ:
Z + button selects voice to edit
Right and Left trigger switch between mode screens
MODES:
EDIT - 
PERFORM - 
SEQ - Sequencer editor

Once in a screen, pressing b returns you to the previous screen
Flow diagram

Mode
{
	Edit
	{
		Drum
		{
		
		}
		Synth
		{
		
		}
		Mod
		{
		
		}
	}
	Perform
	{
	
	}
	Seq
	{
	
	}
}
*/

// Volume envelope for channels
double ch_envs[16];



/*
Output Channels:
0->Main
1->FX1
2->FX2
*/


lfo_t lfos[3];

/*
Notes:
-Compression must be uncompressed (0) for free scrubbing
-For compressed (1) arguments can be passed in from the command line to designate "scrub points"
-TODO: Add a parameter to the compression process that denotes how many indices to subdivide a sample by
-High quality playback cannot be scrubbed

-Can we implement a simple lowpass filter by manipulating the audio buffer directly? Chorus? Phaser?
*/

int randomR(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void one_sec(int ovfl)
{
    t3 += 1.0;
}

void update_audio()
{
	mixer_try_play();
}

void main_init()
{
	debug_init_isviewer();
    debug_init_usblog();
    joypad_init();
	console_init();
	debugf("Debug Console Sanity Check Test Message\n");
    dfs_init(DFS_DEFAULT_LOCATION);
    //display_init(RESOLUTION_512x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_DISABLED);
    rdpq_init();

	audio_init(48000, 4);
	mixer_init(16);  // Initialize up to 16 channels

	// Initialize wav64 compression level 0 as we're going to use it
	wav64_init_compression(0);

	// Bump maximum frequency of music channel to 128k.
	// The default is the same of the output frequency (44100), but we want to
	// let user increase it.
	mixer_ch_set_limits(CHANNEL_MUSIC, 0, 128000, 0);

}

//void display_init()

int main(void) {
    main_init();
	fonts_init();
	menu_init();

	while (1) {
		input_update();
		if(menu_update())
		{
			menu_render();
			
		}
	}
}
