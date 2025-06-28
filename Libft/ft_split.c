/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrhilane <hrhilane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:50:11 by hrhilane          #+#    #+#             */
/*   Updated: 2024/11/18 22:57:52 by hrhilane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c && *s)
			++s;
		if (*s && *s != c)
		{
			count++;
			while (*s && *s != c)
				++s;
		}
	}
	return (count);
}

static char	**malloc_fail(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (NULL);
}

static char	*fillup(char *word, char const *start, char const *end)
{
	size_t	j;

	j = 0;
	while (start < end)
		word[j++] = *start++;
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char const	*start;
	char		**split;
	size_t		i;

	split = (char **)malloc(((count_words(s, c)) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			++s;
		if (*s)
		{
			start = s;
			while (*s && *s != c)
				++s;
			split[i] = (char *)malloc(((s - start) + 1) * sizeof(char));
			if (!split[i])
				return (malloc_fail(split));
			fillup(split[i++], start, s);
		}
	}
	split[i] = NULL;
	return (split);
}
