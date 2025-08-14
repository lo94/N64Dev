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
joypad_inputs_t curJoypadInputs[4];
joypad_inputs_t prevJoypadInputs[4];

//display_context_t disp;
sprite_t *starSprite;

typedef struct display_state_s
{
	bool updateDisplay;
	bool redrawMenu;
} display_state_t;



display_state_t displayState;
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

void update_input()
{
	joypad_poll();
	for(int i = 0; i < 4; i++)
	{
		prevJoypadInputs[i] = curJoypadInputs[i];
		curJoypadInputs[i] = joypad_get_inputs(i);
	}
}

void update_display_begin()
{
	//disp = display_get();
}

void update_display_end()
{
	/*
	if(displayState.updateDisplay)
	{
		display_show(disp);
		displayState.updateDisplay = false;
	}
		*/
}

void update_audio()
{
	mixer_try_play();
}

void update_header_selection()
{
	/*
	graphics_draw_box(disp, mainHeaderSection.textElements[mainHeaderSection.curSelection].x - 5, 
			mainHeaderSection.textElements[mainHeaderSection.curSelection].y, 4, 4, 
			graphics_make_color(0, 0, 255, 255));
	displayState.updateDisplay = true;
	*/
}



void update_render()
{

	
	
}

void init_main()
{
	debug_init_isviewer();
    debug_init_usblog();
    joypad_init();
	console_init();
	//console_set_debug(true);
	debugf("FARTS\n");
    dfs_init(DFS_DEFAULT_LOCATION);
    //display_init(RESOLUTION_512x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_DISABLED);
    rdpq_init();

	//timer_init();
	//new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, one_sec);
	audio_init(48000, 4);
	mixer_init(16);  // Initialize up to 16 channels

	// Initialize wav64 compression level 0 as we're going to use it
	wav64_init_compression(0);

	// Bump maximum frequency of music channel to 128k.
	// The default is the same of the output frequency (44100), but we want to
	// let user increase it.
	mixer_ch_set_limits(CHANNEL_MUSIC, 0, 128000, 0);

	displayState.redrawMenu = true;
}



void init_wav64()
{
	
}

void draw_clear()
{

}

void init_display()
{
	displayState.updateDisplay = true;
	//disp = display_get();
}

//void display_init()

int main(void) {
    init_main();
	init_wav64();
	init_fonts();

	wav64_t sfx_cannon, sfx_laser, sfx_monosample;

	wav64_open(&sfx_cannon, "rom:/cannon.wav64");
	
	wav64_open(&sfx_laser, "rom:/laser.wav64");
	wav64_set_loop(&sfx_laser, true);

	wav64_open(&sfx_monosample, "rom:/monosample8.wav64");
	wav64_set_loop(&sfx_monosample, true);
	bool music = false;
	bool fired = false;
	int music_frequency = sfx_monosample.wave.frequency*0.75;
	char lengthReadout[100];
	double samplePoint = 1.1;//sfx_monosample.wave.len/sfx_monosample.wave.bits;//sfx_monosample.wave.bits;
	//sprintf(lengthReadout, "Length: %d, SamplePoint: %f, Unknown Length: %d", sfx_monosample.wave.len, samplePoint, WAVEFORM_UNKNOWN_LEN);
	//init_display();
	//init_menu();
	
	
	/*
	graphics_fill_screen(disp, 0);
	graphics_draw_text(disp, 200-75, 10, "Audio mixer test");
	graphics_draw_text(disp, 200-70, 20, "v1.0 - by Rasky");
	graphics_draw_text(disp, 50, 60, "A - Play cannon");
	graphics_draw_text(disp, 50, 70, "B - Play laser (keep pressed)");
	graphics_draw_text(disp, 50, 80, "Z - Start / stop background music");
	graphics_draw_text(disp, 70, 90, "L/R - Change music frequency");
	graphics_draw_text(disp, 50, 140, lengthReadout);
	*/

	//wav64_play(&sfx_monosample, CHANNEL_MUSIC);
	//mixer_ch_set_freq(CHANNEL_MUSIC, music_frequency);
	

	menu_init();

	bool firstLoop = true;
	while (1) {

		//update_display_begin();
		update_input();
		update_audio();
		input_update();
		if(menu_update())
		{
			menu_render();
			//mixer_ch_play(CHANNEL_SFX1, &sfx_cannon.wave);
		}
		/*
		joypad_buttons_t ckeys = joypad_get_buttons_pressed(JOYPAD_PORT_1);

		if (music_frequency >= 8000 && ckeys.l) {
			music_frequency /= 1.1;
			mixer_ch_set_freq(CHANNEL_MUSIC, music_frequency);
			
		}
		if (music_frequency*1.1 <= 128000 && ckeys.r) {
			music_frequency *= 1.1;
			mixer_ch_set_freq(CHANNEL_MUSIC, music_frequency);
		}

		ckeys = joypad_get_buttons_released(JOYPAD_PORT_1);
		
		// Check whether one audio buffer is ready, otherwise wait for next
		// frame to perform mixing.
		//mixer_ch_play(CHANNEL_MUSIC, &sfx_monosample.wave);
		if(((int)t3 > 0 && (int)t3 % 2 == 0) && lastScrub != t3) {
			double curPos = mixer_ch_get_pos(CHANNEL_MUSIC);
			double minVal = 1000 > curPos ? curPos : 1000;
			double maxVal = sfx_monosample.wave.len - curPos;
			double position = randomR(minVal, maxVal);
			//debugf("MIXER POSITION: %f \n", position);
			mixer_ch_set_pos(CHANNEL_MUSIC,  position);
			lastScrub = t3;
		}
		mixer_try_play();

		*/

		
		
		
	}
}
