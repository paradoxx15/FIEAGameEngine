#pragma once

#include <string>
#include <memory>
#include <gsl/gsl>
#include "HashMap.h"

namespace Library
{
	template<typename T>
	class Factory
	{
#pragma region FactoryManager

	public:

		/// <summary>
		/// Given a class name it returns the associated factory. Runs in constant time.
		/// </summary>
		/// <param name="name">The class name of the factory being looked for</param>
		/// <returns>A pointer to the factory associated with that class name. Returns nullptr if no factory was found.</returns>
		static const Factory* Find(const std::string& name);

		/// <summary>
		/// Given a class name it returns a new object of that type created from the associated factory.
		/// </summary>
		/// <param name="name">The class name of the object being created</param>
		/// <returns>A pointer of the abstract product type that points to a newly created concrete product object</returns>
		static gsl::owner<T*> Create(const std::string& name);

		/// <summary>
		/// Resizes the internal HashMap. Should be called before adding new factories so that the buckets of the HashMap don't have too many collisions. 
		/// </summary>
		/// <param name="newBucketSize">The new bucket size of the HashMap</param>
		static void Resize(size_t newBucketSize);

		/// <summary>
		/// Clears out all of the factories contained within the factory HashMap
		/// </summary>
		static void Clear();

		/// <summary>
		/// Returns a const reference of the internal HashMap of factories.
		/// </summary>
		/// <returns>Const reference to the internal HashMap</returns>
		static const HashMap<std::string, const Factory<T>&>& Factories();

	protected:

		/// <summary>
		/// Adds a new factory to the HashMap of available factories.
		/// </summary>
		/// <param name="factory">A const reference to the factory that is being added</param>
		/// <exception cref="std::runtime_error">Throws an exception if you try to add a factory that has already been added.</exception>
		static void Add(const Factory& factory);

		/// <summary>
		/// Removes a factory from the HashMap of available factories.
		/// </summary>
		/// <param name="factory">A const reference to the factory that should be removed</param>
		static void Remove(const Factory& factory);

		/// <summary>
		/// The factories being managed by this class. The HashMap is used for quick look up based on string class names.
		/// </summary>
		static HashMap<std::string, const Factory&> mFactories;

#pragma endregion FactoryManager

#pragma region FactoryBaseClass

	public:
		/// <summary>
		/// Default constructor. Has no purpose as this is an abstract base class
		/// </summary>
		Factory() = default;
		/// <summary>
		/// Virtual destructor so that destruction follows polymorphic behavior
		/// </summary>
		virtual ~Factory() = default;

		Factory(Factory&&) = delete;
		Factory& operator=(Factory&&) = delete;
		Factory(const Factory&) = delete;
		Factory& operator=(const Factory&) = delete;

		/// <summary>
		/// The Create method overridden by any base class to create and return an Abstract Product Type pointer to the Concrete
		/// Product created by that factory.
		/// </summary>
		/// <returns>An Abstract Product Type pointer to the newly created object by the factory</returns>
		/// <exception cref="std::runtime_error">Thros an exception if you try to call Create with a factory that is not registered.</exception>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Returns a string representation of the Concrete Product Class that this factory creates.
		/// </summary>
		/// <returns>A string representing the name of the Class this factory creates</returns>
		virtual const std::string ClassName() const = 0;

#pragma endregion FactoryBaseClass
	};

	using namespace std::string_literals;

	/// <summary>
	/// Used to convert a preprocessor macro type into a string that can be converted into a string literal
	/// </summary>
#define StringLit(stringVal) #stringVal

	/// <summary>
	/// Macro to create Concrete Factory classes that create the ConcreteProductType and are under the Factory template specialization
	/// of the AbstractProductType.
	/// </summary>
#define ConcreteFactory(ConcreteProductType, AbstractProductType)								\
	class ConcreteProductType##Factory final : public Library::Factory<AbstractProductType>		\
	{																							\
	public:																						\
		ConcreteProductType##Factory()															\
		{																						\
			Factory<AbstractProductType>::Add(*this);											\
		}																						\
																								\
		~ConcreteProductType##Factory()															\
		{																						\
			Factory<AbstractProductType>::Remove(*this);										\
		}																						\
																								\
		ConcreteProductType##Factory(ConcreteProductType##Factory&&) = delete;					\
		ConcreteProductType##Factory & operator=(ConcreteProductType##Factory&&) = delete;		\
		ConcreteProductType##Factory(const ConcreteProductType##Factory&) = delete;				\
		ConcreteProductType##Factory& operator=(const ConcreteProductType##Factory&) = delete;	\
																								\
		gsl::owner<AbstractProductType*> Create() const override								\
		{																						\
			return new ConcreteProductType;														\
		} 																						\
																								\
		const std::string ClassName() const override											\
		{																						\
			return StringLit(ConcreteProductType)s;												\
		} 																						\
	};																							
}

#include "Factory.inl"