// ========================================================================
//  FILE:
//	GenKbEvent.cpp
//
//  SYNOPSIS:
//	GenKbEvent sequence [-key=ch] ..
//	    key:	sleep | space | escape | htab | enter
//	    ch:		alternate character symbol
//
//  DESCRIPTION:
//	Convert character sequence specified as command line argument to
//	keyboard event sequence.
//	Special symbol:
//	    .	take 1 second interval.
//	    \s	stands for 'space' character
//	    \e	stands for 'escape' character
//	    \t	stands for 'horizontal tab' character
//	    \e	stands for 'enter' character
//	These special symbols can be changed by option argument.
//	Special symbol (except 'sleep' symbol) should be preceded by '\'
//	(escape) character when using in a sequence.
//
//  VERSION:
//	Ver 1.0  2016/11/02 F.Kanehori	First release version.
//	Ver 1.01 2017/09/28 F.Kanehori	Correct comment.
// ========================================================================
#include <windows.h>
#include <stdio.h>
#include <string.h>

bool check_key(int, int[]);
void usage();

// special key ID's
static const int sleep	= 0;
static const int space	= 1;
static const int escape	= 2;
static const int htab	= 3;
static const int enter	= 4;
static const int num_specials	= 5;

// key event flags
const unsigned KEY_DOWN = 0;
const unsigned KEY_UP   = KEYEVENTF_KEYUP;

// for debug
static bool TEST = false;

// ------------------------------------------------------------------------
//  Main function
// ------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	char* seq = argv[1];
	int spec_ch[num_specials];

	if (argc < 2) { usage(); }

	// default special symbols
	spec_ch[sleep]	= '.';
	spec_ch[space]	= 's';
	spec_ch[escape]	= 'e';
	spec_ch[htab]	= 't';
	spec_ch[enter]	= 'n';

	// alternate special key symbols
	for (int n = 2; n < argc; n++) {
		if (strcmp(argv[n], "TEST") == 0) {
			// for debug
			TEST = true;
			continue;
		}
		char* p = argv[n];
		char* q = strchr(argv[n], '=');
		if (q == NULL) {
			fprintf(stderr, "GenKbEvent: invalid speical key '%s'\n", p);
			usage();
		}
		*q++ = '\0';
		if (strcmp(p, "sleep") == 0)	{ spec_ch[sleep]  = *q; }
		if (strcmp(p, "space") == 0)	{ spec_ch[space]  = *q; }
		if (strcmp(p, "escape") == 0)	{ spec_ch[escape] = *q; }
		if (strcmp(p, "htab") == 0)	{ spec_ch[htab]   = *q; }
		if (strcmp(p, "enter") == 0)	{ spec_ch[enter]  = *q; }
	}
	if (TEST) {
		printf("sleep(%c), space(\\%c), escape(\\%c), htab(\\%c), enter(\\%c)\n",
			spec_ch[sleep], spec_ch[space], spec_ch[escape],
			spec_ch[htab], spec_ch[enter]);
	}

	// check validity of argument(s)
	bool ok = true;
	for (int n = 0; n < (int) strlen(seq); n++) {
		int c = seq[n];
		if (!check_key(c, spec_ch)) {
			fprintf(stderr, "GenKbEvent: invalid key '%c'\n", c);
			ok = false;
		}
	}
	if (!ok) {
		exit(-1);
	}

	// generate keyboard event
	for (int n = 0; n < (int) strlen(seq); n++) {
		int c = seq[n];
		if (c == spec_ch[sleep]) {
			if (TEST) { printf("sleep "); }
			Sleep(1000);
			continue;
		}
		char* ch_disp = NULL;
		if (c == '\\' && seq[n+1]) {
			// special keys are processed here
			int c2 = seq[n+1];
			if	(c2 == spec_ch[space])  { c = 0x20; ch_disp = "SP"; }
			else if (c2 == spec_ch[escape]) { c = 0x1b; ch_disp = "ESC"; }
			else if (c2 == spec_ch[htab])   { c = 0x09; ch_disp = "HTAB"; }
			else if (c2 == spec_ch[enter])  { c = 0x0a; ch_disp = "ENTER"; }
			else				{ c = c2; }
			n++;
		}

		const unsigned char vk_lower = VK_MENU;
		const unsigned char vk_upper = VK_SHIFT;
		const char* lower_chars = "abcdefghijklmnopqrstuvwxyz";
		char check[2] = {c, 0};
		bool lower = strstr(lower_chars, check) != NULL;
		if (TEST) {
			if (ch_disp == NULL) {
				printf("0x%02X('%c') ", c, c);
			} else {
				printf("0x%02X(%s) ", c, ch_disp);
			}
		} else {
			if (lower) {
				keybd_event(vk_lower, 0, KEY_DOWN, 0);
				keybd_event(c-0x20,   0, KEY_DOWN, 0);
				keybd_event(c-0x20,   0, KEY_UP,   0);
				keybd_event(vk_lower, 0, KEY_UP,   0);
			} else {
				keybd_event(vk_upper, 0, KEY_DOWN, 0);
				keybd_event(c,        0, KEY_DOWN, 0);
				keybd_event(c,        0, KEY_UP,   0);
				keybd_event(vk_upper, 0, KEY_UP,   0);
			}
		}
	}

	// done
	return 0;
}

// ------------------------------------------------------------------------
//  Check validity of sequence to generate.
// ------------------------------------------------------------------------
bool check_key(int key, int spec_ch[])
{
	// valid characters: 0-9, A-Z, ' ', '\t'
	// special characters are treated as valid characters
	//
	const char* ok_chars = "0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				" \t\\";
	const char check[2] = { key, '\0' };

	if (strstr(ok_chars, check) != NULL) {
		return true;
	}
	for (int n = 0; n < num_specials; n++) {
		if (key == spec_ch[n])
		return true;
	}
	return false;
}

// ------------------------------------------------------------------------
//  Show usage and die.
// ------------------------------------------------------------------------
void usage()
{
	fprintf(stderr, "Usage: GenKbEvent seq <special>=x ...\n");
	fprintf(stderr, "    seq:\tcharacter sequence to generate\n");
	fprintf(stderr, "    special:\tone of sleep, space, escape, htab\n");
	fprintf(stderr, "    x:\talternate character correspondig to <special>\n");
	exit(-1);
}

// end: GenKbEvent.cpp
