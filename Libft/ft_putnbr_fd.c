/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 20:39:44 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 12:29:04 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;
	char	c[10];
	short	i;

	nb = n;
	i = 0;
	if (nb == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	if (nb < 0)
	{
		nb *= -1;
		write(fd, "-", 1);
	}
	while (nb)
	{
		c[i++] = (nb % 10) + 48;
		nb /= 10;
	}
	while (i > 0)
		write(fd, &c[--i], 1);
}
