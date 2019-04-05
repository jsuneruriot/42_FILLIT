/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alstupin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 00:36:39 by alstupin          #+#    #+#             */
/*   Updated: 2019/03/12 00:36:41 by alstupin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fillit.h"

t_piece	*new_list(char **piece, char letter)
{
	t_piece	*new;

	if (!(new = (t_piece*)malloc(sizeof(t_piece))))
		return (NULL);
	new->piece = resize_piece(piece, 0);
	new->letter = letter;
	new->next = NULL;
	return (new);
}

t_piece	*general_listing(char **piece)
{
	static t_piece	*head;
	static char		letter;
	t_piece			*current;

	if (!head)
	{
		letter = 'A';
		head = new_list(piece, letter);
		return (head);
	}
	letter++;
	current = head;
	while (current)
	{
		if (current->next == NULL)
		{
			current->next = new_list(piece, letter);
			break ;
		}
		current = current->next;
	}
	return (head);
}

char	**make_piece(int fd, int *chars)
{
	int		y;
	char	**piece;
	char	*line;
	int		rd;

	y = 0;
	if (!(piece = (char**)malloc(sizeof(char*) * 5)))
		return (NULL);
	while (y != 4)
	{
		if (!(line = (char*)malloc(sizeof(char) * 6)))
			return (NULL);
		rd = read(fd, line, 5);
		*chars = *chars + rd;
		piece[y] = ft_strdup(line);
		free(line);
		piece[y][4] = '\0';
		y++;
	}
	piece[y] = "\0";
	return (piece);
}

t_piece	*get_pieces(int fd)
{
	char	*line;
	char	**piece;
	int		rd;
	int		chars;
	t_piece	*head;

	rd = 1;
	chars = 0;
	if (!(line = (char*)malloc(sizeof(char) * 2)))
		return (NULL);
	while (rd > 0)
	{
		chars = chars + rd;
		piece = make_piece(fd, &chars);
		if (!basic_tests(piece, -1, 0) || !(head = general_listing(piece)))
			return (NULL);
		if (!(rd = read(fd, line, 1)))
			break ;
		if (line[0] != '\n' && line[0] != '\0')
			return (NULL);
	}
	free(line);
	return (chars % 21 == 0 ? head : NULL);
}
