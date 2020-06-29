#include "pch.h"
#include "Attributed.h"

using namespace std::string_literals;

namespace Library
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(RTTI::IdType typeId)
	{
		(*this)["this"s] = this;
		Populate(typeId);
	}

	Attributed::Attributed(const Attributed& rhs) :
		Scope(rhs)
	{
		(*this)["this"s] = this;

		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& rhs) noexcept :
		Scope(std::move(rhs))
	{
		(*this)["this"s] = this;

		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);

		(*this)["this"s] = this;

		UpdateExternalStorage(rhs.TypeIdInstance());

		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));

		(*this)["this"s] = this;

		UpdateExternalStorage(rhs.TypeIdInstance());

		return *this;
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return mMap.ContainsKey(name);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		const Vector<Signature>& prescribedAttributes = TypeManager::GetPrescribedSignatures(this->TypeIdInstance());
		for (const Signature& signature : prescribedAttributes)
		{
			if (signature.Name == name)
			{
				return true;
			}
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxilaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("Can't append an Attribute that is already Prescribed");
		}

		return Append(name);
	}

	const Vector<Scope::PairType*>& Attributed::GetAttributes() const
	{
		return mOrderVector;
	}

	void Attributed::Populate(RTTI::IdType typeId)
	{
		const Vector<Signature>& prescribedAttributes = TypeManager::GetPrescribedSignatures(typeId);

		for (const Signature& signature : prescribedAttributes)
		{
			Datum& attributeDatum = Append(signature.Name);
			attributeDatum.SetType(signature.Type);

			if (signature.IsExternal)
			{
				void* dataLocation = reinterpret_cast<uint8_t*>(this) + signature.Offset;
				attributeDatum.SetStorage(dataLocation, signature.Size);
			}
			else
			{
				attributeDatum.Reserve(signature.Size);
			}
		}
	}

	void Attributed::UpdateExternalStorage(RTTI::IdType typeId)
	{
		Vector<Signature> signatures = TypeManager::GetPrescribedSignatures(typeId);

		for (const Signature& signature : signatures)
		{
			if (signature.IsExternal)
			{
				(*this)[signature.Name].SetStorage<void>(reinterpret_cast<uint8_t*>(this) + signature.Offset, signature.Size);
			}
		}
	}
}