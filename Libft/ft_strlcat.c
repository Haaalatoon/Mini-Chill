/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:16:36 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 12:16:37 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	space_left;
	size_t	src_len;
	size_t	dst_len;
	size_t	size_tot;

	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	if (dstsize <= dst_len)
		return (src_len + dstsize);
	space_left = dstsize - dst_len - 1;
	size_tot = src_len + dst_len;
	while (*src && space_left--)
		*(dst + dst_len++) = *src++;
	*(dst + dst_len) = '\0';
	return (size_tot);
}
