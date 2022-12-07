/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx	= 1;				/* border pixel of windows */
static const Gap default_gap		= {.isgap = 1, .realgap = 20, .gappx = 20};
static const unsigned int snap		= 32;			 /* snap pixel */
static const int showbar			= 1;				/* 0 means no bar */
static const int topbar				= 0;				/* 0 means bottom bar */
static const char *fonts[]			= {
		"Fantasque Sans Mono:pixelsize=16:antialias=true:autohint=true",
		//"Noto Sans Mono:size=10",
		"Noto Color Emoji:style=Regular",
		"Font Awesome 6 Brands Regular:style=Regular",
		"Font Awesome 6 Free Solid:style=Solid",
		"Font Awesome v4 Compatibility Regular:style=Regular",
		"Font Awesome 6 Free Regular:style=Regular",
};
static const char dmenufont[]			= "Fantasque Sans Mono:pixelsize=16:antialias=true:autohint=true";

static const char col_gray1[]			= "#222222";
static const char col_gray2[]			= "#444444";
static const char col_gray3[]			= "#bbbbbb";
static const char col_gray4[]			= "#eeeeee";
static const char col_cyan[]			= "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]			= {
	/*							 fg				 bg				 border	 */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]	= { col_gray4, col_cyan,	col_cyan	},
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class			instance	title	tags mask	isfloating	monitor */
	{ "Gimp",			NULL,		NULL,	1 << 3,		0,			-1 },
	{ "qutebrowser",	NULL,		NULL,	1 << 1,		0,			-1 },
	{ "mpv",			NULL,		NULL,	1 << 2,		1,			-1 },
	{ "tabbed",			NULL,		NULL,	0,			1,			-1 },
};

/* layout(s) */
static const float mfact		= 0.5;		/* factor of master area size [0.05..0.95] */
static const int nmaster		= 1;		/* number of clients in master area */
static const int resizehints	= 1;		/* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;		/* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol		 arrange function */
	{ "[]=",			tile },		/* first entry is default */
	{ "><>",			NULL },		/* no layout function means floating behavior */
	{ "[M]",			monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,			view,		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,			toggleview,	{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,			tag,		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,			toggletag,	{.ui = 1 << TAG} },


/* commands */
static char dmenumon[2] =				"0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] =			{ "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *passmenucmd[] =		{ "passmenu", NULL };
static const char *dmenuemojicmd[] =	{ "dmenu_emoji", NULL };

static const char *termcmd[]	=		{ "st", NULL };
static const char *browsercmd[]	=		{ "qutebrowser", NULL };
static const char *pavucontrolcmd[]	=	{ "pavucontrol", NULL };
static const char *gimpcmd[] =			{ "gimp", NULL };
static const char *nmtuicmd[]	=		{ "st", "-e", "nmtui", NULL };
static const char *nvimcmd[]	=		{ "st", "-e", "nvim", NULL };
static const char *snipcmd[] =			{ "snip", NULL }; // not really using this

static const char *audiomute[] =		{ "/bin/sh", "-c", "pamixer -t && kill -58 $(pidof dwmblocks)", NULL };
static const char *audiodec[] =			{ "/bin/sh", "-c", "pamixer --allow-boost -d 1 && kill -58 $(pidof dwmblocks)", NULL };
static const char *audioinc[] =			{ "/bin/sh", "-c", "pamixer --allow-boost -i 1 && kill -58 $(pidof dwmblocks)", NULL };
static const char *brightnessdec[] =	{ "/bin/sh", "-c", "brightnessctl -d 'amdgpu_bl0' set 1%-", NULL };
static const char *brightnessinc[] =	{ "/bin/sh", "-c", "brightnessctl -d 'amdgpu_bl0' set +1%", NULL };

static const char *screenclip[] =		{ "/bin/sh", "-c", "set -o pipefail; maim -s -u | xclip -selection clipboard -t image/png -i && notify-send \"Screenshoted and saved selection to clipboard\"", NULL };
static const char *screenallclip[] =	{ "/bin/sh", "-c", "set -o pipefail; maim -u | xclip -selection clipboard -t image/png -i && notify-send \"Screenshoted and saved whole screen to clipboard\"", NULL };
static const char *screenfile[] =		{ "/bin/sh", "-c", "maim -s -u $SCR_DIR/$(date +%s).png && notify_send \"Screenshoted selection and saved it to $SCR_DIR\"", NULL };
static const char *screenallfile[] =	{ "/bin/sh", "-c", "maim -u $SCR_DIR/$(date +%s).png && notify_send \"Screenshoted whole screen and saved it to $SCR_DIR\"", NULL };

static const char *dmenubookmark[] =	{ "dmenu_bookmark", NULL };
static const char *addbookmark[] =		{ "add_bookmark",	NULL };
static const char *delbookmark[] =		{ "del_bookmark",	NULL };
/* for brightness/audio fn buttons */
#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier						key			function		argument */
	{ MODKEY,						XK_d,		spawn,			{.v = dmenucmd		} },
	{ MODKEY,						XK_p,		spawn,			{.v = passmenucmd	} },
	{ MODKEY,						XK_e,		spawn,			{.v = dmenuemojicmd } },
	{ MODKEY,						XK_Return,	spawn,			{.v = termcmd		} },
	{ MODKEY,						XK_w,		spawn,			{.v = browsercmd	} },
	{ MODKEY,						XK_a,		spawn,			{.v = pavucontrolcmd} },
	{ MODKEY,						XK_g,		spawn,			{.v = gimpcmd		} },
	{ MODKEY,						XK_n,		spawn,			{.v = nmtuicmd		} },
	{ MODKEY,						XK_v,		spawn,			{.v = nvimcmd		} },
	{ MODKEY,						XK_s,		spawn,			{.v = snipcmd		} },
	{ MODKEY,						XK_b,		spawn,			{.v = dmenubookmark } },
	{ MODKEY|ShiftMask,				XK_b,		spawn,			{.v = addbookmark	} },
	{ MODKEY|ControlMask,			XK_b,		spawn,			{.v = delbookmark	} },
	/* dwm functions */
	{ MODKEY,						XK_x,		togglebar,		{0} },
	{ MODKEY,						XK_j,		focusstack,		{.i = +1 } },
	{ MODKEY,						XK_k,		focusstack,		{.i = -1 } },
	{ MODKEY,						XK_u,		incnmaster,		{.i = +1 } },
	{ MODKEY,						XK_i,		incnmaster,		{.i = -1 } },
	{ MODKEY,						XK_h,		setmfact,		{.f = -0.05} },
	{ MODKEY,						XK_l,		setmfact,		{.f = +0.05} },
	{ MODKEY,						XK_r,		zoom,			{0} },
	{ MODKEY,						XK_Tab,		view,			{0} },
	{ MODKEY,						XK_t,		setlayout,		{.v = &layouts[0]} },
	{ MODKEY,						XK_f,		setlayout,		{.v = &layouts[1]} },
	{ MODKEY,						XK_m,		setlayout,		{.v = &layouts[2]} },
	{ MODKEY,						XK_space,	setlayout,		{0} },
	{ MODKEY|ShiftMask,				XK_space,	togglefloating, {0} },
	{ MODKEY,						XK_0,		view,			{.ui = ~0 } },
	{ MODKEY|ShiftMask,				XK_0,		tag,			{.ui = ~0 } },
	{ MODKEY,						XK_period,  focusmon,		{.i = +1 } },
	{ MODKEY,						XK_comma,	focusmon,		{.i = -1 } },
	{ MODKEY,						XK_minus,	setgaps,		{.i = -5 } },
	{ MODKEY,						XK_equal,	setgaps,		{.i = +5 } },
	{ MODKEY|ShiftMask,				XK_minus,	setgaps,		{.i = GAP_RESET } },
	{ MODKEY|ShiftMask,				XK_equal,	setgaps,		{.i = GAP_TOGGLE} },
	{ MODKEY|ShiftMask,				XK_comma,	tagmon,			{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_period,	tagmon,			{.i = +1 } },
	{ MODKEY,						XK_q,		killclient,		{0} },
	{ MODKEY|ShiftMask,				XK_q,		quit,			{0} },
	/* screenshots */
	{ MODKEY,						XK_apostrophe,	spawn,		{.v = screenclip	} },
	{ MODKEY|ShiftMask,				XK_apostrophe,	spawn,		{.v = screenfile	} },
	{ MODKEY|ControlMask,			XK_apostrophe,	spawn,		{.v = screenallclip } },
	{ MODKEY|ControlMask|ShiftMask,	XK_apostrophe,	spawn,		{.v = screenallfile } },
	/* fn buttons */
	{ 0,			XF86XK_AudioMute,			spawn,			{.v = audiomute		} },
	{ 0,			XF86XK_AudioLowerVolume,	spawn,			{.v = audiodec		} },
	{ 0,			XF86XK_AudioRaiseVolume,	spawn,			{.v = audioinc		} },
	{ 0,			XF86XK_MonBrightnessDown,	spawn,			{.v = brightnessdec	} },
	{ 0,			XF86XK_MonBrightnessUp,		spawn,			{.v = brightnessinc	} },
	{ MODKEY,						XK_F1,		spawn,			{.v = audiomute		} },
	{ MODKEY,						XK_F2,		spawn,			{.v = audiodec		} },
	{ MODKEY,						XK_F3,		spawn,			{.v = audioinc		} },
	{ MODKEY,						XK_F5,		spawn,			{.v = brightnessdec	} },
	{ MODKEY,						XK_F6,		spawn,			{.v = brightnessinc	} },
	TAGKEYS(						XK_1,						0)
	TAGKEYS(						XK_2,						1)
	TAGKEYS(						XK_3,						2)
	TAGKEYS(						XK_4,						3)
	TAGKEYS(						XK_5,						4)
	TAGKEYS(						XK_6,						5)
	TAGKEYS(						XK_7,						6)
	TAGKEYS(						XK_8,						7)
	TAGKEYS(						XK_9,						8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click						event		mask		button				function	argument */
	{ ClkLtSymbol,					0,			Button1,	setlayout,			{0} },
	{ ClkLtSymbol,					0,			Button3,	setlayout,			{.v = &layouts[2]} },
	{ ClkWinTitle,					0,			Button2,	zoom,				{0} },
	{ ClkStatusText,				0,			Button2,	spawn,				{.v = termcmd } },
	{ ClkClientWin,					MODKEY,		Button1,	movemouse,			{0} },
	{ ClkClientWin,				 	MODKEY,		Button2,	togglefloating,		{0} },
	{ ClkClientWin,				 	MODKEY,		Button3,	resizemouse,		{0} },
	{ ClkTagBar,					0,			Button1,	view,				{0} },
	{ ClkTagBar,					0,			Button3,	toggleview,			{0} },
	{ ClkTagBar,					MODKEY,		Button1,	tag,				{0} },
	{ ClkTagBar,					MODKEY,		Button3,	toggletag,			{0} },
};
