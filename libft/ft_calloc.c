/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:12:51 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 12:12:54 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void	*arr;

	if (size && number > SIZE_MAX / size)
		return (NULL);
	arr = malloc(number * size);
	if (arr == NULL)
		return (NULL);
	ft_memset(arr, 0, number * size);
	return (arr);
}
