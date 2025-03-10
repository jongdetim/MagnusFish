#include "MagnusFish.hpp"
#include "Board.hpp"

void	run()
{
	const std::string name = "MagnusFish";
	const std::string author = "vvan-der";

	bool		botShouldRun = true;
	std::string	line;
	Board		board;

	std::cout << board << std::endl;
	while (botShouldRun == true && std::getline(std::cin, line))
	{
		std::stringstream	ss(line);
		std::string			token;

		while (ss >> token)
		{
			if (token == "quit")
			{
				botShouldRun = false;
				break ;
			}
			else if (token == "uci")
			{
				std::cout << "id name " << name << std::endl;
				std::cout << "id author " << author << std::endl;
				std::cout << "uciok" << std::endl;
			}
			else if (token == "isready")
			{
				std::cout << "readyok" << std::endl;
			}
			else if (token == "position")
			{
				;// 
			}
			else if (token == "go")
			{
				;//
			}
			else if (token == "setoption" || token == "register")
			{
				break ;
			}
			else
			{
				continue ;
			}
		}
	}
}
