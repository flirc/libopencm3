/*
 * This file is part of the libopencm3 project and was heavily borrowed from
 * the google chrome embedded controller project
 *
 * Integration by Flirc Inc. Jason Kotzin 2018 <jasonkotzin@gmail.com
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/ushell/printf.h>
#include <libopencm3/ushell/ushell.h>

#include <string.h>

#include "ushell_priv.h"

extern const struct shell_command _cmds[];
extern const struct shell_command _cmds_end[];

static const struct shell_command *running_cmd;
static unsigned int line_i;
static uint8_t first_loop = 1;
static char linebuf[USHELL_LINEBUF_SIZE];

#ifdef CONSOLE_HISTORY_ENABLE
static char history[CONSOLE_HISTORY_LEN][USHELL_LINEBUF_SIZE];
static int history_next, history_pos;
#endif

static void ushell_prompt(void)
{
	printf("%s", PROMPT);
}

static void ushell_run_command(const struct shell_command *cmd,
		int argc, char **argv)
{
	int exit_status;
	if ( (exit_status = cmd->handler(argc-1, &argv[1])) < 0) {
		printf("ERROR: %d\n", exit_status);
		ushell_prompt();
	} else {
		running_cmd = NULL;
		ushell_prompt();
	}
}

static int isalpha(int c)
{
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/**
 * Escape code handler
 *
 * @param c             Next received character.
 * @return		Key code, or -1 if character was eaten
 */
int handle_esc(int c)
{
	switch (esc_state) {
	case ESC_START:
		if (c == '[') {
			esc_state = ESC_BRACKET;
			return -1;
		} else if (c == 'O') {
			esc_state = ESC_O;
			return -1;
		}
		break;

	case ESC_BRACKET:
		if (c == '1') {
			esc_state = ESC_BRACKET_1;
			return -1;
		} else if (c == '3') {
			esc_state = ESC_BRACKET_3;
			return -1;
		}

		if (c == 'A')
			return KEY_UP_ARROW;
		else if (c == 'B')
			return KEY_DOWN_ARROW;
		else if (c == 'C')
			return KEY_RIGHT_ARROW;
		else if (c == 'D')
			return KEY_LEFT_ARROW;
		break;

	case ESC_O:
		if (c == 'F')
			return KEY_END;
		break;

	case ESC_BRACKET_1:
		if (c == '~')
			return KEY_HOME;
		break;

	case ESC_BRACKET_3:
		if (c == '~')
			return KEY_DEL;
		break;

	default:
		break;
	}

	/* Check if the escape code is done */
	if (isalpha(c) || c == '~')
		esc_state = ESC_OUTSIDE;
	else
		esc_state = ESC_BAD;

	return -1;
}


static int handle_special(int c)
{
	if (esc_state) {
		c = handle_esc(c);
		if (c != -1)
			esc_state = ESC_OUTSIDE;
		else
			return 0;
	}

	switch (c) {
	case '\b':
	case 0x7f:
	case (KEY_DEL):
	        if(line_i) {
	                printf("\b \b");
	                line_i--;
	        }
		return 0;
	case CTRL('C'):
		printf("^C\n");
		line_i = 0;
		ushell_prompt();
		return 0;
	case KEY_RIGHT_ARROW:
	case KEY_LEFT_ARROW:
		return 0;
#ifdef CONSOLE_HISTORY_ENABLE
	case KEY_DOWN_ARROW:
		/* History next */
		if (history_pos == history_next)
			save_history();

		if (++history_pos >= CONSOLE_HISTORY_LEN)
			history_pos = 0;

		load_history(history_pos);
		return 0;
	case KEY_UP_ARROW:
		/* History previous */
		if (history_pos == history_next)
			save_history();

		if (--history_pos < 0)
			history_pos = CONSOLE_HISTORY_LEN - 1;

		load_history(history_pos);
		return 0;
#endif
	case ESC:
		/* escape key */
		esc_state = ESC_START;
		return 0;
	default:
		/* no special character */
		return -1;
	}
}

static inline int ushell_iswhite(char c)
{
	const char *white = " \t\r\n";
	while (*white) {
		if (*white == c)
			return 1;
		white++;
	}
	return 0;
}

static void ushell_tokenize_linebuf(char **argv, int *argc)
{
	*argc = 0;
	unsigned int j;
	int was_white, is_white;
	char quote = '\0';

	for(j = 0, was_white = 1, is_white = 0;
			linebuf[j] && (*argc < USHELL_MAX_ARGC);
			j++, was_white = is_white) {
		if(!quote) {
			if(linebuf[j] == '\"' || linebuf[j] == '\'') {
				quote = linebuf[j];
			}
		} else {
			if(linebuf[j] == quote) {
				quote = '\0';
			}
			continue;
		}

		is_white = ushell_iswhite(linebuf[j]);
		if(is_white) {
			linebuf[j] = '\0';
			continue;
		}

		if(!is_white && was_white) {
			argv[(*argc)++] = &(linebuf[j]);
		}
	}
}

/**
 * Find a command by name.
 *
 * Allows partial matches, as long as the partial match is unique to one
 * command.  So "foo" will match "foobar" as long as there isn't also a
 * command "food".
 *
 * @param name          Command name to find.
 *
 * @return A pointer to the command structure, or NULL if no match found.
 */
static const struct shell_command *find_command(char *name)
{
        const struct shell_command *cmd, *match = NULL;
        int match_length = strlen(name);

        for (cmd = _cmds; cmd < _cmds_end; cmd++) {
                if (!strncasecmp(name, cmd->name, match_length)) {
                        if (match)
                                return NULL;
                        /*
                         * Check if 'cmd->name' is of the same length as
                         * 'name'. If yes, then we have a full match.
                         */
                        if (cmd->name[match_length] == '\0')
                                return cmd;
                        match = cmd;
                }
        }

        return match;
}

static void ushell_handle_linebuf(void)
{
	int argc;
	char *argv[USHELL_MAX_ARGC];
	const struct shell_command *cmd;

#ifdef CONSOLE_HISTORY_ENABLE

	if (strlen(linebuf) && !strstr(linebuf, "history")) {
		save_history();
		history_next = (history_next + 1) % CONSOLE_HISTORY_LEN;
		history_pos = history_next;
	}
#endif
	ushell_tokenize_linebuf(argv, &argc);

	/* output prompt and bail for empty string */
	if (!argc) {
		ushell_prompt();
		return;
	}

	/* find the command */
	if ((cmd = find_command(argv[0])) == NULL) {
		printf("Command not found: %s\n", argv[0]);
		ushell_prompt();
		return;
	}

	/* run the command */
	ushell_run_command(cmd, argc, &argv[0]);
}

void cmd_loop(void)
{
	int c = EOF;

	if (first_loop) {
		printf("\033[2J\n");
		ushell_prompt();
               	first_loop = 0;
	}

	while((c = _getc()) != EOF) {
	        static int last_was_cr = 0;

	        /* only handle 1 of a \r\n sequence */
	        if((char)c == '\n' && last_was_cr) {
	                last_was_cr = 0;
	                continue;
	        }

	        if((char)c == '\n' || (char)c == '\r') {
	                _putc('\n');
	                linebuf[line_i] = '\0';
	                line_i = 0;
	                ushell_handle_linebuf();
	        } else if(handle_special(c) >= 0) {
			continue;
	        } else if(line_i < (USHELL_LINEBUF_SIZE - 1)) {
	                _putc(c);
	                linebuf[line_i++] = (char)c;
	        }

		last_was_cr = ((char)c == '\r') ? 1 : 0;
	}
}

static int cmd_help(int argc, char **argv)
{
	const int ncmds = _cmds_end - _cmds;
	int i;

	if (argc == 1) {
		const struct shell_command *cmd;

		cmd = find_command(argv[0]);
		if (!cmd) {
			printf("Command '%s' not found or ambiguous.\n",
				 argv[0]);
			return -1;
		}
		if (cmd->help)
			printf("%s\n", cmd->help);
		return 0;
	}

	for (i = 0; i < ncmds; i++) {
		printf("  %-18s ", _cmds[i].name);
		printf("%.56s\n", _cmds[i].summary);
	}

	return 0;
}

DECLARE_SHELL_COMMAND(help, cmd_help,
	"Show this help. Also try `help <command'",
	"  Display helpful information about builtin commands.\n"
	"  If PATTERN is specified, gives detailed help on all\n"
	"  commands matching PATTERN, otherwise a list of the \n"
	"  builtins is printed. The -s option restricts the output\n"
	"  for each builtin command matching PATTERN to a short usage\n"
	"  synopsis");

#ifdef CONSOLE_HISTORY_ENABLE
static void move_cursor_begin(void)
{
	while (line_i) {
		printf("\b \b");
		line_i--;
	}
}

/**
 * Load input history
 *
 * @param idx		History index to load
 */
void load_history(int idx)
{
	/* Copy history */
	strzcpy(linebuf, history[idx], USHELL_LINEBUF_SIZE);

	/* Print history */
	move_cursor_begin();
	printf("%s", linebuf);

	/* Clear everything past end of history */
	line_i = strlen(linebuf);
}

/**
 * Save line to the next history slot
 */
void save_history(struct console_list *shellp)
{
	if (strlen(linebuf) > 0)
		strzcpy(history[history_next], linebuf, USHELL_LINEBUF_SIZE);
}

#endif /* CONSOLE_HISTORY_ENABLE */
