/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elfstream_macros.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 13:31:34 by kiroussa          #+#    #+#             */
/*   Updated: 2025/05/14 16:21:03 by kiroussa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ELFSTREAM_MACROS_H
# define ELFSTREAM_MACROS_H

# ifndef ELF_BITNESS
#  error "ELF_BITNESS not defined"
# endif

# define _APPEND(x, y) x##y
# define APPEND(x, y) _APPEND(x, y)

# define Elf(n) APPEND(APPEND(APPEND(Elf, ELF_BITNESS), _), n)
# define Func(n) APPEND(APPEND(n, _x), ELF_BITNESS)

#endif // ELFSTREAM_MACROS_H
