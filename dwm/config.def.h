/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;       /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 0;     /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const int user_bh            = 20;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = {
    "Cantarell:size=9",
    "M+ 1c:size=9",
    "Apple Color Emoji:size=9",
};
static const char *colors[][3]      = {
	/*                       fg         bg         border   */
	[SchemeNorm]        = { "#f8f8f8", "#181818", "#383838" },
	[SchemeSel]         = { "#f8f8f8", "#282828", "#f8f8f8" },
	[SchemeStatus]      = { "#f8f8f8", "#181818", "#000000" }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]     = { "#f8f8f8", "#282828", "#000000" }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]    = { "#f8f8f8", "#181818", "#000000" }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]     = { "#f8f8f8", "#282828", "#000000" }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]    = { "#f8f8f8", "#181818", "#000000" }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const char *const autostart[] = {
	"picom", NULL,
    "dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",           NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",        NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,             NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int decorhints  = 1;    /* 1 means respect decoration hints */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "F",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	{ 0, XF86XK_AudioRaiseVolume,   spawn,      SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +1% ; kill -36 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,   spawn,      SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -1% ; kill -36 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioMute,          spawn,      SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle ; kill -36 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioMicMute,       spawn,      SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle ; kill -35 $(pidof dwmblocks)") },
    // No use. Because my current laptop does not have such media keys. Consider removing statusbar item.
	// { 0, XF86XK_AudioPlay,          spawn,      SHCMD("playerctl play-pause ; kill -40 $(pidof dwmblocks)") },
	// { 0, XF86XK_AudioNext,          spawn,      SHCMD("playerctl next ; kill -40 $(pidof dwmblocks)") },
	// { 0, XF86XK_AudioPrev,          spawn,      SHCMD("playerctl previous ; kill -40 $(pidof dwmblocks)") },
	{ 0, XF86XK_MonBrightnessDown,  spawn,      SHCMD("xbacklight -dec 5") },
	{ 0, XF86XK_MonBrightnessUp,    spawn,      SHCMD("xbacklight -inc 5") },
    
    // Screen rotation. To be honest it works now but script should be review and repaired.
    // { MODKEY, XK_Up,                spawn,      SHCMD("screenr normal") },
    // { MODKEY, XK_Down,              spawn,      SHCMD("screenr inverted") },
    // { MODKEY, XK_Left,              spawn,      SHCMD("screenr left") },
    // { MODKEY, XK_Right,             spawn,      SHCMD("screenr right") },

	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },

	{ MODKEY,                       XK_m,      zoom,           {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_b,      togglebar,      {0} },

	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_Tab,    togglefloating, {0} },

    // I use one monitor for now. Should configure this when I get one more.
	// { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	// { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

    { ControlMask|ShiftMask,        XK_r,      quit,           {1} },
	{ ControlMask|ShiftMask,        XK_q,      quit,           {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },

	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
	/* signum       function        argument  */
	{ 1,            setlayout,      {.v = 0} },
};
