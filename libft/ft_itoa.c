/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <hrhilane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 21:56:07 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/19 00:06:19 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	abs_value(int n)
{
	if (n >= 0)
		return (n);
	return (-n);
}

static char	*fill_number(char *number, unsigned int num, int size)
{
	int	i;

	i = size;
	while (num)
	{
		number[--i] = num % 10 + '0';
		num = num / 10;
	}
	return (number);
}

static char	*case_zero(void)
{
	char	*number;

	number = (char *)malloc(2 * sizeof(char));
	if (!number)
		return (NULL);
	number[0] = '0';
	number[1] = '\0';
	return (number);
}

char	*ft_itoa(int n)
{
	char			*number;
	int				count;
	int				size;
	unsigned int	num;

	count = 0;
	if (n == 0)
		return (case_zero());
	num = abs_value(n);
	while (num)
	{
		count++;
		num = num / 10;
	}
	size = count + (n < 0);
	number = (char *)malloc((size + 1) * sizeof(char));
	if (!number)
		return (NULL);
	number[size] = '\0';
	if (n < 0)
		number[0] = '-';
	num = abs_value(n);
	fill_number(number, num, size);
	return (number);
}
