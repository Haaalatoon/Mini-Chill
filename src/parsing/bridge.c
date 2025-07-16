/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchetoui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 12:55:30 by mchetoui          #+#    #+#             */
/*   Updated: 2025/07/12 12:55:31 by mchetoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <main.h>
#include <_printfd.h>

void	count_words(t_list *list, int *args, int *redir)
{
	t_list	*curr;
	t_token	*tok;

	*args = 0;
	*redir = 0;
	curr = list;
	while (curr)
	{
		tok = (t_token *)curr->data;
		if (tok->type == Pipe || tok->type == End_of_file)
			break ;
		if (tok->type == Word)
			(*args)++;
		else if (tok->type <= 5 && tok->type >= 2)
		{
			(*args)--;
			(*redir)++;
		}
		curr = curr->next;
	}
}

t_cmd	*init_cmd(t_list *list)
{
	t_cmd	*cmd;
	int		argcount;
	int		redircount;

	count_words(list, &argcount, &redircount);
	cmd = malloc(sizeof(t_cmd));
	cmd->args = NULL;
	cmd->redir = NULL;
	if (argcount)
	{
		cmd->args = malloc(sizeof(char *) * (argcount + 1));
		cmd->args[argcount] = NULL;
	}
	if (redircount)
		cmd->redir = malloc(sizeof(t_redir) * redircount);
	cmd->argcount = argcount;
	cmd->redircount = redircount;
	cmd->next = NULL;
	return (cmd);
}

char	*check_heredoc(t_token_type type, t_token *token)
{
	char	*eof;
	char	*path;
	int		expandable;

	expandable = 1;
	if (type != Here_doc)
		return (_strdup(token->value));
	if (*token->value == '"' || *token->value == '\'')
	{
		expandable = 0;
		eof = quotes(token->value);
	}
	else
		eof = token->value;
	path = do_heredoc(eof, expandable);
	if (!expandable)
		free(eof);
	return (path);
}

t_cmd	*create_cmd(t_list **list, t_list *cpy)
{
	t_token	*tok;
	t_cmd	*cmd;

	cmd = init_cmd(cpy);
	while (*list)
	{
		tok = (t_token *)(*list)->data;
		if (tok->type == Pipe || tok->type == End_of_file)
			break ;
		if (tok->type == Word)
			*(cmd->args++) = quotes_expand(tok->value, NULL);
		else if (tok->type >= 2)
		{
			cmd->redir->type = tok->type;
			*list = (*list)->next;
			cmd->redir->file = check_heredoc(cmd->redir->type, (*list)->data);
			cmd->redir++;
		}
		*list = (*list)->next;
	}
	cmd->args -= cmd->argcount;
	cmd->redir -= cmd->redircount;
	return (cmd);
}

t_cmd	*create_pipeline(t_list *list)
{
	t_list	*cpy;
	t_cmd	*head;
	t_cmd	*cmd;

	cpy = list;
	head = create_cmd(&list, cpy);
	if (data()->sig)
	{
		_printfd(2, "hdsigint\n");
		return (NULL);
	}
	if (head->argcount == 0)
    {
        free_pipeline(head);
        return NULL;
    }
	cmd = head;
	while (list && ((t_token *)list->data)->type != End_of_file)
	{
		if (((t_token *)list->data)->type == Pipe)
		{
			list = list->next;
			continue ;
		}
		cpy = list;
		cmd->next = create_cmd(&list, cpy);
		if (data()->sig)
		{
			_printfd(2, "hdsigint\n");
			return (NULL);
		}
		cmd = cmd->next;
	}
	return (head);
}
