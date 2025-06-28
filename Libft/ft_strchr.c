/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <hrhilane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:15:56 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 23:39:18 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	*found;

	found = (unsigned char *)s;
	while (*found)
	{
		if (*found == (unsigned char)c)
			return ((char *)found);
		found++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)found);
	return (NULL);
}
