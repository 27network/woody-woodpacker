/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:22:52 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/15 02:31:19 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLI_H
# define CLI_H
# undef CLI_H
# ifndef __WW_CLI_H__
#  define __WW_CLI_H__

#  include <ft/opt.h>
#  include <ft/print.h>
#  include <stdbool.h>
#  include <ww/compress.h>
#  include <ww/encrypt.h>
#  include <ww/log.h>

#  define WW_OUTPUT_DEFAULT "woody"
#  define WW_ENCRYPTION_DEFAULT "xor"

// The getopt-like options string
#  define OPTSTRING "hVc:e:k:o:ap:v"

enum e_ww_cli_status
{
	CLI_SUCCESS = 0,
	CLI_EXIT_SUCCESS = 1,
	CLI_EXIT_FAILURE = 2,
};

/**
 * @brief	The structure that holds the arguments passed to the program.
 *
 * @note	Some of those are optional, some have default values, and some
 * 			can be set via environment variables. See `ww_cli_defaults`.
 *
 * @param	target			The file to encrypt/decrypt.
 *							(required)
 * @param	output			The output file.
 *							(optional, default: "woody", env: WW_OUTPUT)
 * @param	encryption_algo The encryption algorithm to use.
 * 							(optional, default: AES, env: WW_ENCRYPTION_ALGO)
 * @param	encryption_key	The encryption key.
 * 							(optional, env: WW_ENCRYPTION_KEY)
 * @param	log_level		The log verbosity level (see `log.h`).
 *							(optional, default: INFO, env: WW_LOG_LEVEL)
 * @param	compression		The compression algorithm to use.
 * 							(optional, default: none, env: WW_COMPRESSION)
 * @param	payload_file	The payload file to use.
 * 							(optional, default: <built-in>, env: WW_PAYLOAD_FILE)
 * @param	payload_async	Whether the payload should run asynchronously.
 * 							(optional, default: false, env: WW_PAYLOAD_ASYNC)
 */
typedef struct s_ww_args
{
	const char					*target;
	const char					*output;
	enum e_ww_encryption_algo	encryption_algo;
	const char					*encryption_key;
	enum e_ww_log_level			log_level;
	enum e_ww_compression_algo	compression;
	const char					*payload_file;
	bool						payload_async;
}	t_ww_args;

/**
 * @brief	Parses the command line arguments.
 *
 * @param	args		The program argument struct to fill.
 * @param	argc		The number of arguments.
 * @param	argv		The arguments.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_parse(t_ww_args *args, int argc, char **argv);

/**
 * @brief	Sets the default values for the arguments, either from
 * 			environment variables or from hardcoded defaults.
 *
 * @param	args	The arguments to set the defaults for.
 * @param	envp	The environment table.
 */
void
ww_cli_defaults(t_ww_args *args, char **envp);

#  ifdef __WW_CLI_INTERNAL__

/**
 * @brief	Handles the `-c` option, setting the compression algorithm
 * 			to use.  If an inappropriate algorithm is passed, it will
 * 			fail and print the available algorithms.
 *
 * @param	args		The arguments.
 * @param	compression	The desired compression algorithm.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_compression(t_ww_args *args, const char *compression);

/**
 * @brief	Handles the `-e` option, setting the encryption algorithm
 * 			to use.  If an inappropriate algorithm is passed, it will
 * 			fail and print the available algorithms.
 *
 * @param	args			The arguments.
 * @param	encryption_key	The desired encryption algorithm.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_encryption_algo(t_ww_args *args, const char *encryption_algo);

/**
 * @brief	Handles the `-k` option, setting the encryption key.
 *
 * @param	args			The arguments.
 * @param	encryption_key	The desired encryption key.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_encryption_key(t_ww_args *args, const char *encryption_key);

/**
 * @brief	Handles the `--help` option, printing the help message.
 *
 * @param	argv	The arguments.
 * @param	fd		The file descriptor to print to, useful when printing
 * 					help in response to an error.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_help(char **argv, int fd);

/**
 * @brief	Handles the `-o` option, setting the output file.  It
 * 			will fail if the file already exists.
 *
 * @param	args	The arguments.
 * @param	output	The desired output file.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_output(t_ww_args *args, const char *output);

/**
 * @brief	Handles the `-p` option, setting the payload file to use.
 *
 * @param	args		The arguments.
 * @param	payload	The desired payload file.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_payload(t_ww_args *args, const char *payload);

/**
 * @brief	Handles the `-v` flag(s), increasing the log level by one
 * 			for each occurence.
 *
 * @param	args	The arguments.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_verbose(t_ww_args *args);

/**
 * @brief	Handles the `--version` option, printing the version message.
 *
 * @param	fd		The file descriptor to print to, useful when printing
 * 					help in response to an error.
 *
 * @return	An `enum e_ww_cli_status` value.
 */
int
ww_cli_opt_version(int fd);

#  endif // __WW_CLI_INTERNAL__

# endif // __WW_CLI_H__
#endif // CLI_H
