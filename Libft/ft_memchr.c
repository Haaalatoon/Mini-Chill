/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:14:51 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 12:14:52 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*found;
	size_t			i;

	i = 0;
	found = (unsigned char *)s;
	while (i < n)
	{
		if (found[i] == (unsigned char)c)
			return ((void *)(found + i));
		i++;
	}
	return (NULL);
}
