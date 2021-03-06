/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdurst <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/05 03:24:43 by rdurst            #+#    #+#             */
/*   Updated: 2018/09/09 05:27:08 by rdurst           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	main -	loop that handles user input
*/

int		main(int ac, char **av, char **env)
{
	init_env(env);
	while (1)
		minishell();
	(void)ac;
	(void)av;
	return (0);
}

/*
**	minishell - uses the input to make the magic begins
*/

void	minishell(void)
{
	char	**command;
	char	*line;

	print_prompt();
	line = NULL;
	signal(SIGINT, signal_handler);
	if (get_next_line(STDIN_FILENO, &line) != -1 && !ft_strequ(line, ""))
	{
		command = parse_command(line);
		if (ft_strlen_tab(command) == 0 ||
			is_builtin(ft_strlen_tab(command), command) != -1)
		{
			ft_strdel(&line);
			free_command(command);
			return ;
		}
		else
			exec_command(command, g_envp);
		free_command(command);
	}
	ft_strdel(&line);
}

/*
**	print_prompt -	simply prints the prompt
*/

void	print_prompt(void)
{
	static int color = 0;

	if (color++ % 2)
		ft_putcolor(BGREEN, "$> ");
	else
		ft_putcolor(BBLUE, "$> ");
}

/*
**	is_builtin -	calls the built-in functions
**			if the command is a built-in
*/

int		is_builtin(int ac, char **av)
{
	if (ft_strequ(av[0], "echo"))
		return (echo_builtin(ac, av));
	if (ft_strequ(av[0], "cd"))
		return (cd_builtin(ac, av));
	if (ft_strequ(av[0], "setenv"))
		return (setenv_builtin(ac, av));
	if (ft_strequ(av[0], "unsetenv"))
		return (unsetenv_builtin(ac, av));
	if (ft_strequ(av[0], "env"))
		return (env_builtin(ac, av));
	if (ft_strequ(av[0], "exit"))
		return (exit_builtin(ac, av));
	return (-1);
}

/*
**	free_command -	free ft_strsplit() output
*/

void	free_command(char **command)
{
	int i;

	i = 0;
	while (command[i])
		ft_strdel(&command[i++]);
	free(command);
	command = NULL;
}
