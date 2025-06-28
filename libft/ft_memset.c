/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:15:41 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 12:15:42 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr1;

	i = 0;
	ptr1 = (unsigned char *)s;
	while (i < n)
	{
		ptr1[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
