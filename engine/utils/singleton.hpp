#pragma once
#include <thread>
#include <mutex>
#include <utils/platform.hpp>
template <class T>
class EngineAPI singleton
{

	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;

protected:
	singleton() {}
	~singleton()
	{
		if (ins_)
		{
			delete ins_;
			ins_ = nullptr;
		}
	}
public:


	static T* pointer()
	{
		if (ins_)
		{
			return ins_;
		}
		std::call_once(once_, [&]() {
			ins_ = new T();
			});
		return ins_;
	}

	//inline T* operator->() const {
	//	return pointer();
	//}

	template <class... P>
	static T& instance(P... args)
	{
		if (ins_)
		{
			return *ins_;
		}

		std::call_once(once_, [&]() {
			ins_ = new T(args...);
			});
		return *ins_;
	}

	static T& instance()
	{
		if (ins_)
		{
			return *ins_;
		}
		std::call_once(once_, [&]() {
			ins_ = new T();
			});
		return *ins_;
	}

private:
	static std::once_flag once_;
	static T* ins_;
};

template <class T>
std::once_flag singleton<T>::once_;
template <class T>
T* singleton<T>::ins_ = nullptr;
