#include "Factory.h"

namespace Library
{
	template<typename T>
	HashMap<std::string, const Factory<T>&> Factory<T>::mFactories(41);

	template<typename T>
	inline const Factory<T>* Factory<T>::Find(const std::string& name)
	{
		auto it = mFactories.Find(name);
		if (it == mFactories.end())
		{
			return nullptr;
		}

		return &it->second;
	}

	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& name)
	{
		auto it = mFactories.Find(name);
		if (it == mFactories.end())
		{
			return nullptr;
		}

		return it->second.Create();
	}

	template<typename T>
	inline void Factory<T>::Add(const Factory& factory)
	{
		if (mFactories.ContainsKey(factory.ClassName()))
		{
			throw std::runtime_error("Unable to add duplicate factories");
		}

		mFactories.Insert(std::pair<const std::string&, const Factory&>(factory.ClassName(), factory));
	}

	template<typename T>
	inline void Factory<T>::Remove(const Factory& factory)
	{
		mFactories.Remove(factory.ClassName());
	}

	template<typename T>
	inline void Factory<T>::Clear()
	{
		mFactories.Clear();
	}

	template<typename T>
	inline void Library::Factory<T>::Resize(size_t newBucketSize)
	{
		mFactories.Resize(newBucketSize);
	}

	template<typename T>
	inline const HashMap<std::string, const Factory<T>&>& Factory<T>::Factories()
	{
		return mFactories;
	}
}