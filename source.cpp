#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <string>

typedef std::vector<double> vecS;

void print(vecS x, std::string str)
{
	std::cout << str << "\n";
	for (auto p = x.begin(); p != x.end(); p++) {
		std::cout << *p << " ";
	}
	std::cout << "\n";
}

struct NOFUNC
{
public:
	static vecS forward(const vecS& in)
	{
		return in;
	}

	static vecS backward(const vecS& in)
	{
		return in;
	}
};

template<typename Func = NOFUNC>
class Square 
{
public:
	static vecS tempdata;
	static vecS forward(const vecS& in)
	{
		auto temp = Func::forward(in);
		Square<Func>::tempdata = temp;
		vecS o;
		for (auto p = temp.begin(); p != temp.end(); p++) {
			o.push_back(pow(*p, 2));
		}
		return o;
	}

	static vecS backward(const vecS& grad)
	{
		auto temp = Func::backward(grad);
		vecS o;
		for (int i = 0; i < temp.size(); i++) {
			o.push_back(2 * temp[i] * Square<Func>::tempdata[i]);
		}
		return o;
	}

};

template<typename Func = NOFUNC>
class Exp 
{
public:
	static vecS tempdata;
	static vecS forward(const vecS& in)
	{
		auto x = Func::forward(in);
		Exp<Func>::tempdata = x;
		vecS o;
		for (auto p = x.begin(); p != x.end(); p++) {
			o.push_back(exp(*p));
		}
		return o;
	}

	static vecS backward(const vecS& grad)
	{
		auto temp = Func::backward(grad);
		vecS o;
		for (int i = 0; i < temp.size(); i++) {
			o.push_back(temp[i] * exp(Exp<Func>::tempdata[i]));
		}
		return o;
	}
};

template<typename Func>
vecS Square<Func>::tempdata;
template<typename Func>
vecS Exp<Func>::tempdata;

int main()
{
	vecS var(3, 0.5f), grad(3, 1.0f);

	Square<Exp<Square<>>> func;
	print(func.forward(var),"forward");
	print(func.backward(grad),"backward");
}
