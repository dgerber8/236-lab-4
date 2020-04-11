#ifndef HEADER_H
#define HEADER_H
#include <iostream> 
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector> 

using namespace std;

string GiveFullToken(string str, string &id)
{
	int strLength = str.length();
	bool quoteEnded = false;

	switch (str[0])
	{
	case ('Q'):
		if (str[1] == 'u' && str[2] == 'e' && str[3] == 'r' && str[4] == 'i' && str[5] == 'e' && str[6] == 's' && strLength == 7)
		{
			id = str;
			return "QUERIES";
		}
		else
		{
			id = str;
			return "ID";
		}
	case ('S'):
		if (str[1] == 'c' && str[2] == 'h' && str[3] == 'e' && str[4] == 'm' && str[5] == 'e' && str[6] == 's' && strLength == 7)
		{
			id = str;
			return "SCHEMES";
		}
		else
		{
			id = str;
			return "ID";
		}

	case ('F'):
		if (str[1] == 'a' && str[2] == 'c' && str[3] == 't' && str[4] == 's' && strLength == 5)
		{
			id = str;
			return "FACTS";
		}
		else
		{
			id = str;
			return "ID";
		}
	case ('R'):
		if (str[1] == 'u' && str[2] == 'l' && str[3] == 'e' && str[4] == 's' && strLength == 5)
		{
			id = str;
			return "RULES";
		}
		else
		{
			id = str;
			return "ID";
		}
		// STRING
	case ('\''):

		for (int i = 1; i < strLength; i++)
		{

			if (str[i] == '\'')
			{
				if (quoteEnded == false)
				{
					quoteEnded = true;
				}
				else
				{
					quoteEnded = false;
				}
			}
		}

		if (quoteEnded == true)
		{
			id = str;
			return "STRING";
		}
		else
		{
			return "UNDEFINED";
		}

		//COMMENT
	case('#'):
		if (str[1] == '|')
		{
			for (int i = 2; i < strLength; i++)
			{
				if (str[i] == '|' && i == strLength - 2)
				{
					if (str[i + 1] == '#')
					{
						return "COMMENT";
					}
				}
			}
			return "UNDEFINED";
		}
		else
		{
			return "COMMENT";
		}

	default:
		if (isalpha(str[0]))
		{
			id = str;
			return "ID";
		}
		else
		{
			return "UNDEFINED";
		}

	}
}

// Takes the current character and gives it the final token string if it is a token
string GiveCharToken(char cChar, ifstream &in, string &id)
{
	char next;
	switch (cChar)
	{
		// All Single Characters
	case (','): id = cChar; return "COMMA"; break;
	case ('.'): id = cChar; return "PERIOD"; break;
	case ('?'): id = cChar; return "Q_MARK"; break;
	case ('('): id = cChar; return "LEFT_PAREN"; break;
	case (')'): id = cChar; return "RIGHT_PAREN"; break;
	case ('*'): id = cChar; return "MULTIPLY"; break;
	case ('+'): id = cChar; return "ADD"; break;
	case (EOF): id = cChar; return "EOF"; break;
	case (':'):
		next = in.peek();
		if (next == '-')
		{
			return "COLON_DASH";
		}
		else
		{
			return "COLON";
		}
	default: return "UNDEFINED";
	}
}

// Takes in the in stream and outputs all the Tokens
string PrintFinal(ifstream &in, string &id, int &lineNum)
{
	string currWord = "";

	//Add 1 to line num and move on if we get a new line
	while (in.peek() == 10 || in.peek() == 32 || in.peek() == '\t')
	{
		if (in.peek() == 10)
		{
			lineNum++;
		}
		in.get();
	}
	// move on to the next character if a space is recieved
	if (in.peek() == EOF)
	{
		return "EOF";
	}

	//10 = new line     32 = space
	// This while loop seperates the input into components that can be evealuated as tokens later
	while (GiveCharToken(in.peek(), in, id) == "UNDEFINED" && in.peek() != 10 && in.peek() != EOF)
	{
		if (currWord[0] == '\'')
		{
			while (in.peek() != '\'')
			{
				currWord += in.get();
			}
			currWord += in.get();

			break;
		}

		else if (currWord[0] == '#')
		{
			while (in.peek() != 10 && in.peek() != EOF)
			{
				currWord += in.get();
			}
			if (in.peek() == 10)
			{
				lineNum++;
			}
			in.get();
			return PrintFinal(in, id, lineNum);
			//break;
		}
		else
		{
			if (in.peek() != 32)
			{
				currWord += in.get();
			}
			else
			{
				in.get();
			}
		}

	}

	// Determines token for a word if it isn't empty
	if (currWord != "")
	{
		return GiveFullToken(currWord, id);
	}

	else
	{
		string token = GiveCharToken(in.get(), in, id);
		// Special case for Colon Dash b/c we need to print the next character as well ('-')
		if (token == "COLON_DASH")
		{
			in.get();
			return token;
		}
		else
		{
			return token;

		}
	}
}

#endif