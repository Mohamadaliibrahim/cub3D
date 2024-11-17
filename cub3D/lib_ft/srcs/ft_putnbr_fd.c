/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohamibr <mohamibr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:03:42 by mohamibr          #+#    #+#             */
/*   Updated: 2024/11/17 17:49:49 by mohamibr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	ft_putnbr_fd1(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd1('-', fd);
		n = -n;
	}
	if (n > 9)
	{
		ft_putnbr_fd1(n / 10, fd);
		ft_putnbr_fd1(n % 10, fd);
	}
	else
		ft_putchar_fd1(n + '0', fd);
}
