

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edoardo <edoardo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 15:48:09 by edoardo           #+#    #+#             */
/*   Updated: 2024/02/20 12:05:41 by edoardo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

std::string StringToUpper(char *str)
{
	for (size_t i = 0; str[i] ; i++)
		str[i] = toupper(str[i]);
	return (str);
}

int main(int argc, char **argv) 
{	
	if (argc <= 1)
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
	else
	{
		for (int i = 1; i < argc; i++)
			std::cout << StringToUpper(argv[i]);
		std::cout << std::endl;
	}
	return 0;
}
