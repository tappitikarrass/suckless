/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

static int topbar = 0;                      /* -b  option; if 0, dmenu appears at bottom     */
static int fuzzy = 1;                      /* -F  option; if 0, dmenu doesn't use fuzzy matching     */
/* -fn option overrides fonts[0]; default X11 font or font set */
static const char *fonts[] = {
	"Fira Mono:size=9",
    "JetBrainsMono Nerd Font:size=11",
    "M+ 1c:size=9",
    "Apple Color Emoji:size=9"
};
static const char *prompt      = NULL;      /* -p  option; prompt to the left of input field */
static const char *colors[SchemeLast][2] = {
	/*     fg         bg       */
	[SchemeNorm] = { "#f8f8f8", "#181818" },
	[SchemeSel] = { "#181818", "#f8f8f8" },
	[SchemeOut] = { "#f8f8f8", "#585858" },
};
/* -l option; if nonzero, dmenu uses vertical list with given number of lines */
static unsigned int lines      = 0;
/* -h option; minimum height of a menu line */
static unsigned int lineheight = 20;
static unsigned int min_lineheight = 20;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " ";

/*
 * Use prefix matching by default; can be inverted with the -x flag.
 */
static int use_prefix = 1;
