#include "get_next_line.h"

void	list_clean(t_list **list, t_list *new)
{
	t_list	*cur_node;
	t_list	*temp;

	cur_node = *list;
	while (cur_node)
	{
		temp = cur_node;
		cur_node = cur_node -> next;
		free(temp -> buffer);
		free(temp);
	}
	*list = NULL;
	if (new -> buffer[0] != '\0')
		*list = new;
	else
	{
		free(new -> buffer);
		free(new);
	}
}

void	pull_new_line(t_list **list)
{
	t_list	*last_node;
	t_list	*new_node;
	char	*new_str;
	int		i;
	int		j;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return ;
	new_str = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!new_str)
		return ;
	last_node = *list;
	while (last_node -> next != NULL)
		last_node = last_node -> next;
	j = 0;
	i = 0;
	while (last_node -> buffer[i] && (last_node -> buffer[i] != '\n'))
		i++;
	while (last_node -> buffer[i] && last_node -> buffer[++i])
		new_str[j++] = last_node -> buffer[i];
	new_str[j] = '\0';
	new_node -> buffer = new_str;
	new_node -> next = NULL;
	list_clean(list, new_node);
}

char	*get_cur_line(t_list *list)
{
	char	*str;
	int		len;

	if (!list)
		return (NULL);
	len = list_len(list);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	list_strcpy(list, str);
	return (str);
}

void	create_list(t_list **list, int fd, int *error_read)
{
	int		read_buffer;
	char	*str;

	while (!check_new_line(*list))
	{
		str = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!str)
			return ;
		read_buffer = read(fd, str, BUFFER_SIZE);
		*error_read = read_buffer;
		if (read_buffer < 0)
			clear_list_leak(list);
		if (read_buffer < 1)
		{
			free(str);
			return ;
		}
		str[read_buffer] = '\0';
		add_back(list, str);
	}
}

char	*get_next_line(int fd)
{
	static t_list	*list;
	char			*line;
	int				error_read;

	error_read = 0;
	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (NULL);
	create_list(&list, fd, &error_read);
	if ((long)error_read < 0)
		return (NULL);
	if (!list)
		return (NULL);
	line = get_cur_line(list);
	pull_new_line(&list);
	return (line);
}