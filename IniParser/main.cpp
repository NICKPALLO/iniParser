#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>

void next_line(std::ifstream& fin)
{
	char nextSymb = ' ';
	while (nextSymb != '\n' && !fin.eof())
	{
		fin.get(nextSymb);
	}
}

template <class T>
void get_Value(std::ifstream& fin, T& answer, bool& find)
{
	char nextSymb = ' ';
	std::string buff = "";
	while (true)
	{
		fin.get(nextSymb);
		if (nextSymb == ';' || nextSymb == '\n' || nextSymb == ' ')
		{
			break;
		}
		buff += nextSymb;
	}
	if (buff != "" && buff != "\n")
	{
		if (typeid(T) == typeid(int))
		{
			answer = std::stoi(buff);
		}
		else if (typeid(T) == typeid(double))
		{
			answer = std::stod(buff);
		}
		else if (typeid(T) == typeid(std::string))
		{
			//answer = buff
		}
		else
		{
			//answer = static_cast<T>(buff);
		}
		find = true;
	}
}

template <>
void get_Value(std::ifstream& fin, std::string& answer, bool& find)
{
	char nextSymb = ' ';
	std::string buff = "";
	while (true)
	{
		fin.get(nextSymb);
		if (nextSymb == ';' || nextSymb == '\n')
		{
			break;
		}
		buff += nextSymb;
	}
	if (buff != "" && buff != "\n")
	{
		answer = buff;
		find = true;
	}
}

template <class T>
void find_Value(std::ifstream& fin, std::string& value, T& answer, bool& find)
{
	std::string buff = "";
	char nextSymb = ' ';
	while (nextSymb != '[' && !fin.eof()) //пока не дошли до следующей секции и файл не закончился
	{
		fin.get(nextSymb);
		buff += nextSymb;
		if (buff == value + '=')
		{
			get_Value(fin, answer, find);
			buff = "";
		}
		else if (nextSymb == '\n')
		{
			buff = "";
		}
		else if (nextSymb == ';')
		{
			next_line(fin);
			buff = "";
		}
	}
	if (!fin.eof()) //захватили символ [ -> нужно вернуться назад на один символ
	{
		fin.seekg(-1, std::ios_base::cur);
	}
}

void find_Section(std::ifstream& fin, std::string& section)
{
	std::string buff = "";
	do
	{
	fin >> buff;
	} while (buff != '[' + section + ']' && !fin.eof());
}





class ini_parser
{
public:
	ini_parser(std::string filename)
	{
		fin.open(filename);
		if (!fin.is_open())
		{
			throw std::exception("file does not exist");
		}
	}
	~ini_parser()
	{
		fin.close();
	}

	ini_parser(const ini_parser& other) = delete;
	ini_parser(ini_parser&& other) = delete;
	ini_parser& operator= (const ini_parser& other) = delete;
	ini_parser& operator= (ini_parser&& other) = delete;

	template<class T>
	T get_value(std::string section, std::string value)
	{
		T answer;
		bool find = false;
		fin.seekg(0);
		while (!fin.eof())
		{
			find_Section(fin, section);
			find_Value(fin,value, answer, find);
		}
		if (find)
		{
			fin.clear();
			return answer;
		}
		else
		{
			throw std::exception("Section or value are not exist!");
		}
	}

private:
	std::ifstream fin;
};



int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ini_parser parser("test.ini");
	try
	{
		auto value1 = parser.get_value<std::string>("Section1", "var2");
		std::cout << value1 << std::endl;
		auto value2 = parser.get_value<double>("Section4", "Mode");
		std::cout << value2 << std::endl;
		auto value3 = parser.get_value<int>("Section2", "var1");
		std::cout << value3 << std::endl;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what();
	}
	return 0;
}