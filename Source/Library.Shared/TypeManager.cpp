#include "pch.h"
#include "TypeManager.h"
#include "SList.h"

namespace Library
{
	void TypeManager::RegisterType(RTTI::IdType typeId, RTTI::IdType parentId, const Vector<Signature>& prescibedAttributes)
	{
		mTypeAttributes.Insert(std::pair(typeId, TypeInfo(parentId, prescibedAttributes)));
	}

	void TypeManager::UnregisterType(RTTI::IdType typeId)
	{
		mTypeAttributes.Remove(typeId);
	}

	const Vector<Signature> TypeManager::GetPrescribedSignatures(RTTI::IdType typeId)
	{
		 size_t id = typeId;
		 size_t size = 0;
		 SList<size_t> typeIdQueue;

		 do 
		 {
			 auto it = mTypeAttributes.Find(id);
			 assert(it != mTypeAttributes.end());

			 size += it->second.PrescribedAttributes.Size();
			 typeIdQueue.PushFront(id);
			 id = it->second.ParentId;
		 } while (id != Attributed::TypeIdClass());

		 if (typeIdQueue.Size() == 1)
		 {
			 return mTypeAttributes.Find(typeId)->second.PrescribedAttributes;
		 }
		 else
		 {
			 Vector<Signature> retSignatures(size);
			 for (size_t type : typeIdQueue)
			 {
				 Vector<Signature>& signatures = mTypeAttributes.Find(type)->second.PrescribedAttributes;
				 
				 assert(CheckForDuplicates(retSignatures, signatures) == false);
				 
				 for (Signature& signature : signatures)
				 {
					 retSignatures.PushBack(signature);
				 }
			 }

			 return retSignatures;
		 }
	}

	void TypeManager::Clear()
	{
		mTypeAttributes.Clear();
	}

	bool TypeManager::CheckForDuplicates(const Vector<Signature>& checkFor, const Vector<Signature>& checkWith)
	{
		bool ret = false;

		for (Signature signature : checkWith)
		{
			for (Signature otherSig : checkFor)
			{
				ret |= signature.Name == otherSig.Name;
			}
		}

		return ret;
	}

	TypeManager::TypeInfo::TypeInfo(RTTI::IdType parentId, const Vector<Signature>& prescribedAttributes) :
		ParentId(parentId), PrescribedAttributes(prescribedAttributes) {}

	Signature::Signature(const std::string& name, Datum::DatumTypes type, bool isExternal, size_t size, size_t offset) : 
		Name(name), Type(type), IsExternal(isExternal), Size(size), Offset(offset) {}
}
