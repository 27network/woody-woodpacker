/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:19:39 by kiroussa          #+#    #+#             */
/*   Updated: 2025/02/22 15:53:32 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINARY_H
# define BINARY_H
# undef BINARY_H
# ifndef __WW_BINARY_H__
#  define __WW_BINARY_H__

#  include <ww/cli.h>
#  include <stdbool.h>

#  define WW_BIN_HANDLERS_MAX 64

enum e_ww_error
{
	WW_OK = 0,
	WW_ERROR = 1
};

struct					s_ww_binary_handler;

typedef struct s_ww_binary
{
	const char					*input;
	const char					*output;
	int							input_fd;
	char						*blob;
	size_t						blob_size;
	struct s_ww_binary_handler	*handler;
}	t_ww_binary;

enum e_ww_error
ww_bin_init(t_ww_binary *bin, t_ww_args *args);

void
ww_bin_free(t_ww_binary *bin);

enum e_ww_error
ww_bin_identify(t_ww_binary *bin);

enum e_ww_error
ww_bin_map(t_ww_binary *bin);

enum e_ww_error
ww_bin_read(t_ww_binary *bin);

enum e_ww_error
ww_bin_process(t_ww_binary *bin);

enum e_ww_error
ww_bin_write(t_ww_binary *bin);

//

typedef enum e_ww_error	t_ww_binary_handler_fn(struct s_ww_binary_handler *self,
							t_ww_binary * bin);
typedef enum e_ww_error	t_ww_binary_ident_fn(struct s_ww_binary_handler *self);

typedef struct s_ww_binary_handler
{
	const char				*type;
	size_t					struct_size;
	t_ww_binary_ident_fn	*identify;
	t_ww_binary_handler_fn	*read;
	t_ww_binary_handler_fn	*process;
	t_ww_binary_handler_fn	*write;
}	t_ww_binary_handler;

t_ww_binary_handler
*ww_bin_handler_registry(void);

void
ww_bin_handler_registry_dump(void);

void
ww_bin_handler_register(t_ww_binary_handler handler);

# endif // __WW_BINARY_H__
#endif // BINARY_H
