/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2018  Flirc Inc.
 * Written by Jason Kotzin <jasonkotzin@gmail.com>
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

#ifndef I__USHELL_H__
#define I__USHELL_H__

#ifndef USHELL_MAX_ARGC
#define USHELL_MAX_ARGC			10
#endif /* USHELL_MAX_ARGC */

#ifndef USHELL_LINEBUF_SIZE
#define USHELL_LINEBUF_SIZE		512
#endif /* USHELL_LINEBUF_SIZE */

#ifndef CONSOLE_HISTORY_LEN
#define CONSOLE_HISTORY_LEN		10
#endif /* CONSOLE_HISTORY_LEN */

#ifndef EOF
#define EOF -1
#endif

#ifndef PROMPT
#define PROMPT "shell> "
#endif

void cmd_loop(void);

/* Console command; used by DECLARE_shell_command macro. */
struct shell_command {
        /* Command name.  Case-insensitive. */
        const char *name;
        /* Handler for the command. args start at argv[0] */
        int (*handler)(int argc, char **argv);
        /* Short help for command */
        const char *summary;
        /* Long help for command */
        const char *help;
};

/**
 * Register a console command handler.
 *
 * @param name          Command name; must not be the beginning of another
 *                      existing command name.  Note this is NOT in quotes
 *                      so it can be concatenated to form a struct name.
 * @param handler       Command handling routine, of the form
 *                      int handler(int argc, char **argv)
 * @param shorthelp     String with one-line description of command.
 * @param longhelp      String with long description of command.
 */
#define DECLARE_SHELL_COMMAND(name, handler, summary, help) \
        static const char __con_cmd_label_##name[] = #name; \
        const struct shell_command __con_cmd_##name \
        __attribute__((section(".cmds." #name))) \
             = {__con_cmd_label_##name, handler, summary, help}

#endif /* I__SHELL_H__ */
