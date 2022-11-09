#include <string>
#include <cstdint>
#include <chrono>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <type_traits>
#include <queue>
#include <functional>
#include <memory>
class ipser 
{
public:
	virtual ~ipser() {}
	virtual void say() = 0;
};

class my_pser : public ipser
{
public:
	virtual ~my_pser() 
	{
		std::cout << "~my_pser()" << std::endl;
	}

	virtual void say() 
	{
		std::cout << "my_pser say()" << std::endl;
	}
};
class their_pser 
{
public:
	virtual ~their_pser()
	{
		std::cout << "~their_pser()" << std::endl;
	}

	virtual void say()
	{
		std::cout << "their_pser say()" << std::endl;
	}
};

//template <typename this_parser, int>
//struct iss;


template<typename this_parser, typename std::enable_if<std::is_convertible_v<this_parser*, ipser*>>::type* = nullptr>
class iss: public std::enable_shared_from_this<iss<this_parser>>
{
	//static_assert(std::is_convertible_v<this_parser*, ipser*>,"iss comp error");
public:
	virtual ~iss() {}
	virtual void start() = 0;
};

template<class this_parser>
class my_ss : public iss<this_parser> 
{
	using t_parser = this_parser;
public:
	virtual ~my_ss() 
	{
		std::cout << "~my_ss()" << std::endl;
	}
	virtual void start()
	{
		parser_.say();
		std::cout << "my_ss start()" << std::endl;
	}
	t_parser parser_;
};

template<class this_parser>
class their_ss
{
	using t_parser = this_parser;
public:
	virtual ~their_ss()
	{
		std::cout << "~their_ss()" << std::endl;
	}
	virtual void start()
	{
		parser_.say();
		std::cout << "their_ss start()" << std::endl;
	}
	t_parser parser_;
};

//template <typename T, typename Enable = void>
//struct check;
//
//template <typename T>
//struct check<T, typename std::enable_if<T::value>::type> {
//	static constexpr bool value = T::value
//};
#pragma pack(push)
#pragma pack(1)
struct hd
{
	char a;
	int b;
	int64_t c;
	float d;
};

class i_bu : public std::enable_shared_from_this<i_bu>
{
public:
	~i_bu()
	{
		std::cout << "~i_bu()" << std::endl;
	}
	virtual int get_header_size() = 0;
};

template <int size, int headersize >
class auto_bu : public i_bu
{
public:
	auto_bu()
		:size_(size)
		, header_size_(headersize)
	{}
	~auto_bu()
	{
		std::cout << "~auto_bu()" << std::endl;
	}
	virtual int get_header_size() { return header_size_; }
	int size_;
	int header_size_;
};

class dog
{
public:
	void say()
	{
		std::cout << "wangwang" << std::endl;
	}
};

class static_dog 
{
public:
	static dog dog_;
	static void say()
	{
		dog_.say();
	}
};
dog static_dog::dog_ = dog{};

class t_template
{
public:
	virtual ~t_template() {}
	virtual void excute()
	{
		bool b_to_a = std::is_convertible<my_pser, ipser>::value;
		bool a_to_b = std::is_convertible<ipser ,my_pser>::value;
		
		bool b_to_a_dec = std::is_convertible<std::decay_t<my_pser>, std::decay_t<ipser>>::value;
		bool b_to_a_same = std::is_same<my_pser*, ipser*>::value;
		bool b_to_a_same2 = std::is_same<std::decay_t<my_pser*>, std::decay_t<ipser*>>::value;

		bool b_to_a_v = std::is_convertible_v<my_pser*, ipser*>;
		bool b_to_a_v2 = std::is_convertible_v< std::decay_t<my_pser*>, std::decay_t<ipser*>>;
		bool a_to_b_v = std::is_convertible_v<ipser*, my_pser*>;
		bool a_to_b_v2 = std::is_convertible_v< std::decay_t<ipser*>, std::decay_t<my_pser*>>;
		bool c_to_a_v = std::is_convertible_v<ipser*, their_pser*>;
		bool c_to_a_v2 = std::is_convertible_v<their_pser*, ipser*>;

		bool i32_to_i32 = std::is_convertible< std::decay_t<int64_t*>, std::decay_t<int32_t*>>::value;
		std::cout << b_to_a_dec << b_to_a_same << b_to_a_same2 << b_to_a_v  << b_to_a_v2  << std::endl;
		std::cout << a_to_b_v << a_to_b_v2 << c_to_a_v << c_to_a_v2 << i32_to_i32 << std::endl;
		std::enable_if<std::is_convertible_v<std::decay_t<my_pser*>, std::decay_t<ipser*>>>::type* num = nullptr;
		if (num) {}
		std::cout << "b_to_a = " << b_to_a << "   a_to_b = " << a_to_b  << std::endl;

		/*bool b_to_a = std::is_convertible_v< std::decay_t<my_pser*>, std::decay_t<ipser*>>;
		bool c_to_a = std::is_convertible_v< std::decay_t<their_pser*>, std::decay_t<ipser*>>;*/
		//以下无法编译
		//typename std::enable_if_t<std::is_convertible_v<std::decay_t<their_pser*>, std::decay_t<ipser*>>>* mypoint2 = nullptr;
		//std::enable_if_t<std::is_convertible_v<std::decay_t<ipser*>, std::decay_t<my_pser*>>>;
		//std::enable_if_t<std::is_convertible_v<std::decay_t<ipser*>, std::decay_t<their_pser*>>>;
		//std::enable_if_t<std::is_convertible_v<std::decay_t<their_pser*>, std::decay_t<ipser*>>>;
		std::cout << "b_to_a = " << b_to_a <<  "   a_to_b = " << a_to_b << std::endl;
		my_ss<my_pser> my_as_pser;
		my_as_pser.start();
	/*	my_ss<their_pser> their_as_pser;
		their_as_pser.start();*/
		auto their_ss_parse = new their_ss<their_pser>();
		their_ss_parse->start();
		delete their_ss_parse;

		ptr_release();
		auto b = que_.front();
		std::cout << " b header_size " << b->get_header_size() << std::endl;
		//i_bu* ab = que_.pop();

		static_dog::say();
	}
	
	using ibu_ptr = std::shared_ptr<i_bu>;
	using auto_bu_ptr = std::shared_ptr<auto_bu<64, sizeof(hd)>>;
	std::queue<i_bu*> que_;

	void add_que(auto_bu<64, 1>* ab)
	{
		/*ab->shared_from_this();*/
		que_.push(ab);
	}


	void const_auto(const std::shared_ptr < auto_bu<64, 1> >& ab)
	{
		if(ab == nullptr)
		{
			std::cout << " ab is null " << std::endl;
		}else
		{
			std::cout << " ab header_size " << ab->get_header_size() << std::endl;
		}
	}
	void ptr_release()
	{
		/*using ibu_ptr = std::shared_ptr<i_bu>;
		using auto_bu_ptr = std::shared_ptr<auto_bu<64, sizeof(hd)>>;
		ibu_ptr bu_ptr = std::make_shared<auto_bu<64, sizeof(hd)>>();
		std::cout << " bu header_size " << bu_ptr->get_header_size() << std::endl;
		
		std::cout << "que size " << que_.size() << std::endl;*/

		//std::shared_ptr<Sample> p3(new Sample[3], [](Sample* sm) {});
		auto ab = new auto_bu<64, 1>();
		std::shared_ptr < auto_bu<64, 1> > abp(ab, std::bind(&t_template::add_que,this, std::placeholders::_1));
		const_auto(abp);
		std::shared_ptr < auto_bu<64, 1>> tmp = nullptr;
		const_auto(tmp);
	/*	if (!tmp) 
		{
			std::cout << " tmp is null " << std::endl;
		}
		std::cout << " bu header_size " << abp->get_header_size() << std::endl;*/

		//std::swap(ab,ab2);
	}
};

int main(int argc, char* argv[])
{
	printf("\n----- start -----\n");
	t_template* sln = new t_template();
	sln->excute();
	delete sln;
	printf("------ end ------\n");
	return 0;
}
