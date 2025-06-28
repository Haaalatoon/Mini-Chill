/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:09:34 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/27 12:09:38 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s;
	size_t	len2;
	size_t	len1;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	s = (char *)malloc(len1 + len2 + 1);
	if (s == NULL)
		return (NULL);
	ft_strlcpy(s, s1, len1 + 1);
	ft_strlcat(s, s2, len1 + len2 + 1);
	return (s);
}
