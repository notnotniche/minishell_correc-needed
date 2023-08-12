/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itahani <itahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 18:14:13 by nklingsh          #+#    #+#             */
/*   Updated: 2023/08/12 15:30:13 by itahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *is_pathabs(t_init *init, char *str)
{
	if ((str[0] == '/' || str[0] == '.') && access(str, F_OK) == 0)
	{
		// printf("lkjfslkgweklfgwlelgjweljgwejlgjlkwe");
		if (check_if_directory(str))
		{
			init->err_msg = ft_strjoin(str, ": Is a directory");
			ft_print_fd(init->err_msg, 2);
			exit(126);
		}
		if (file_exec(str) == 1)
			return (str);
		if (file_exec(str) != 1)
		{
			init->err_msg = ft_strjoin(str, ": Permission denied");
			ft_print_fd(init->err_msg, 2);
			exit(126);
		}
		exit(1);
	}
	else if ((str[0] == '/' || str[0] == '.') && access(str, F_OK) == -1)
	{
		init->err_msg = ft_strjoin(str, ": No such file or directory");
		ft_print_fd(init->err_msg, 2);
		exit(127);
	}
	return (NULL);
	init->lst_token->token = 1;
}

char *path_res(t_init *init, t_str_list *cmd)
{
	char *res;
	
	if (ft_strlen(cmd->str_list) == 0)
		exit(1);
	// printf("\n cmd->str_list : %s \n",cmd->str_list);
	res = is_pathabs(init, cmd->str_list);
	// printf("path res  : --> %s\n", res);
	if (res)
		return (res);
	return (NULL);
}

void free_tab_tab(char **splittos)
{
	int i;
	
	i = 0;
	while (splittos[i])
	{
		free(splittos[i]);
		i++;
	}
	free(splittos);
	return ;
}

char *path_maker(t_init *init, t_str_list *cmd, char *path,t_exec_init *exec_init)
{
	char *res;
	char *tmp;
	char **splittos;
	int i;
	
	i = 0;
	res = path_res(init, cmd);
		printf("res      %s", res);
	if (res)
		return (printf("--> path_res %s res \n",res), res);
	if (!path)
		exit(1);
	splittos = ft_split(path, ':');
	while (splittos[i])		
	{
		tmp = ft_strjoin(splittos[i], "/");
		res = ft_strjoin(tmp, cmd->str_list);
		printf("arguments :::::::: ->    %s \n", res);
		free(tmp);
		// printf("\n%s", res);
		if (file_exec(res) == 1)
			return (ft_strdup(res));
		free(res);
		i++;
	}
	free_tab_tab(splittos);
	init->err_msg = ft_strjoin(cmd->str_list, ": command not found");
	ft_print_fd(init->err_msg, 2);
	close(exec_init->mypipe[1]);
	close(exec_init->mypipe[0]);
	close(exec_init->pipetmp);
	free(init->err_msg);
	free_s_init(init);
	free_env_list(init->lst_env);
	exit(127);
	return (NULL);
}