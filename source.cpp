#include <iostream>
#include <functional>
#include <cmath>
#include <vector>
#include <string>

typedef std::vector<double> vecD;

void print(vecD x, std::string str)
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
	static vecD forward(const vecD& in)
	{
		return in;
	}

	static vecD backward(const vecD& in)
	{
		return in;
	}
};

template<typename Func = NOFUNC>
class Square 
{
public:
	typedef vecD Type;
	static vecD tempdata;
	static vecD forward(const vecD& in)
	{
		auto temp = Func::forward(in);
		Square<Func>::tempdata = temp;
		vecD o;
		for (auto p = temp.begin(); p != temp.end(); p++) {
			o.push_back(pow(*p, 2));
		}
		return o;
	}

	static vecD backward(const vecD& grad)
	{
		auto temp = Func::backward(grad);
		vecD o;
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
	typedef vecD Type;
	static vecD tempdata;
	static vecD forward(const vecD& in)
	{
		auto x = Func::forward(in);
		Exp<Func>::tempdata = x;
		vecD o;
		for (auto p = x.begin(); p != x.end(); p++) {
			o.push_back(exp(*p));
		}
		return o;
	}

	static vecD backward(const vecD& grad)
	{
		auto temp = Func::backward(grad);
		vecD o;
		for (int i = 0; i < temp.size(); i++) {
			o.push_back(temp[i] * exp(Exp<Func>::tempdata[i]));
		}
		return o;
	}
};

template<typename Func>
vecD Square<Func>::tempdata;
template<typename Func>
vecD Exp<Func>::tempdata;

struct Forward;
struct Backward;

template <typename Func,typename Mode>
class Diff;

template <typename Func>
class Diff<Func,Forward>
{
public:
	typedef typename Func::Type DataType;
	static DataType get(DataType& in)
	{
		return 	Func::forward(in);
	}
};

template <typename Func>
class Diff<Func,Backward>
{
public:
	typedef typename Func::Type DataType;
	static DataType get(DataType& in)
	{
		return Func::backward(in);
	}
};


int main()
{
	vecD var(3, 0.5f), grad(3, 1.0f);
	typedef Square<Exp<Square<>>> Func;

	/*
	//1
	Func func;
	print(func.forward(var),"forward");
	print(func.backward(grad),"backward");
	*/

	//or 2
	print(Diff<Func,Forward>::get(var), "forward");
	print(Diff<Func,Backward>::get(grad), "backward");
}
