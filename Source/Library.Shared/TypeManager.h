#pragma once

#include "HashMap.h"
#include "Vector.h"
#include "Attributed.h"

namespace Library
{
	/// <summary>
	/// Contains the information necessary to build an attribute for a scope. Used to designate prescribed attributes for a type.
	/// </summary>
	struct Signature
	{
		Signature(const std::string& name, Datum::DatumTypes type, bool isExternal, size_t size, size_t offset);
		Signature(const Signature&) = default;
		Signature(Signature&&) = default;
		Signature& operator=(const Signature&) = default;
		Signature& operator=(Signature&&) = default;
		~Signature() = default;

		std::string Name;
		Datum::DatumTypes Type;
		bool IsExternal;
		size_t Size;
		size_t Offset;
	};

	class TypeManager
	{
	public:

		/// <summary>
		/// Contains the parent ID and vector of prescribed attributes associated with a type ID
		/// </summary>
		struct TypeInfo
		{
			TypeInfo(RTTI::IdType parentId, const Vector<Signature>& prescribedAttributes);

			size_t ParentId;
			Vector<Signature> PrescribedAttributes;
		};

		TypeManager() = delete;
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;
		~TypeManager() = delete;

		/// <summary>
		/// Registers a new class Type with the manager by adding a Vector of prescribed attribute signatures associated with the class to the manager
		/// </summary>
		/// <param name="typeId">The RTTI TypeIdClass used to associate a class type with its prescribed attributes</param>
		/// <param name="prescibedAttributes">The Vector of Attribute signatures to be associated with the passed in typeId</param>
		static void RegisterType(RTTI::IdType typeId, RTTI::IdType parentId, const Vector<Signature>& prescibedAttributes);

		/// <summary>
		/// Unregisters a class Type with the manager by removing the Vector of prescribed attribute signatures associated with the class from the manager
		/// </summary>
		/// <param name="typeId">The RTTI TypeIdClass used to associate a class type with its prescribed attributes</param>
		static void UnregisterType(RTTI::IdType typeId);

		/// <summary>
		/// Returns the Vector of Attribute signatures associated with the given typeId
		/// </summary>
		/// <param name="typeId">The typeId whose corresponding list of </param>
		/// <returns>A const reference to the Vector of Attribute signatures associated with the typeId</returns>
		/// <exception cref="std::runtime_error">Throws an exception if there is no attributes registered with that type or the type has a duplicate signature as one of its parent types</exception>
		static const Vector<Signature> GetPrescribedSignatures(RTTI::IdType typeId);

		/// <summary>
		/// Clears all stored type data contained within the manager
		/// </summary>
		static void Clear();

	private:
		/// <summary>
		/// Given two vectors of signatures check if they contain any signatures with duplicate names
		/// Will only be called in debug mode, can be removed for release.
		/// </summary>
		/// <param name="checkFor">The Vector of signatures being checked for duplicates</param>
		/// <param name="checkWith">The Vector of signatures being checked for duplicates against the checkFor Vector</param>
		/// <returns>True if there were duplicate signatures between the Vectors, false otherwise</returns>
		static bool CheckForDuplicates(const Vector<Signature>& checkFor, const Vector<Signature>& checkWith);

		/// <summary>
		/// Static HashMap that maps class typeId's to Vectors that contain signatures of that types prescribed attributes
		/// </summary>
		inline static HashMap<RTTI::IdType, TypeInfo> mTypeAttributes;
	};
}

