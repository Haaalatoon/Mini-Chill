/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 19:54:14 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/26 21:46:40 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *node)
{
	t_list	*p_lst;

	if (!lst)
		return ;
	if (*lst)
	{
		p_lst = ft_lstlast(*lst);
		p_lst->next = node;
		node->prev = p_lst;
	}
	else
	{
		*lst = node;
		node->prev = NULL;
	}
}
