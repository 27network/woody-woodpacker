/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consts.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 22:35:35 by kiroussa          #+#    #+#             */
/*   Updated: 2025/06/20 21:06:25 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTS_H
# define CONSTS_H
# undef CONSTS_H
# ifndef __WW_CONSTS_H__
#  define __WW_CONSTS_H__

/**
 * @file	consts.h
 * @brief	Some replacable macro constants, see the project `Makefile`.
 */

#  ifndef WW_PROJECT_NAME
#   define WW_PROJECT_NAME "woody-woodpacker"
#  endif // WW_PROJECT_NAME

#  ifndef WW_PROJECT_VERSION
#   define WW_PROJECT_VERSION "0.0.0+unknown"
#  endif // WW_PROJECT_VERSION

#  ifndef WW_PROJECT_URL
#   define WW_PROJECT_URL "https://github.com/27network/woody-woodpacker"
#  endif // WW_PROJECT_URL

#  define USAGE_LINE "Usage: %s [OPTION]... <target>\n"
#  define HELP_MESSAGE "\
A dead-simple packer.\n\
\n\
  -h, --help   Display this help message and exit.\n\
  -V           Displays the version number and exit.\n\
  -o <output>  Sets the output file (default: '%s').\n\
  -e <mode>    Sets the encryption mode to use (default: '%s').\n\
  -k <key>     Sets the encryption key (default: random).\n\
  -c <comp>    Sets the compression algorithm to use (default: '%s').\n\
  -p <payload> Sets the payload file to use (default: builtin).\n\
  -a           Runs the payload asynchronously (default: false).\n\
  -v           Increses the verbosity level for each occurence.\n\
\n\
If no target is specified, the program will exit with an error.\n\
"

# endif // __WW_CONSTS_H__
#endif // CONSTS_H
