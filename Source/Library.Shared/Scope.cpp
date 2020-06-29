#include "pch.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope);

#pragma region MemberMethods

	Scope::Scope(size_t bucketSize) : 
		mOrderVector(bucketSize)
	{
		assert(bucketSize != 0);
		mMap.Resize(bucketSize);
	}

	Scope::Scope(Scope* parent) : mParent(parent) {}

	Scope::Scope(const Scope& rhs) :
		mOrderVector(rhs.NumAttributes()), mMap(rhs.BucketSize())
	{
		CopyHelper(rhs);
	}

	Scope::Scope(Scope&& rhs) noexcept : 
		mOrderVector(std::move(rhs.mOrderVector)), mMap(std::move(rhs.mMap)), mParent(rhs.mParent)
	{
		MoveHelper(&rhs);
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mOrderVector.Reserve(rhs.NumAttributes());
			mMap.Resize(rhs.BucketSize());
			CopyHelper(rhs);
		}

		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Orphan();
			Clear();
			mOrderVector = std::move(rhs.mOrderVector);
			mMap = std::move(rhs.mMap);
			mParent = rhs.mParent;
			MoveHelper(&rhs);
		}

		return *this;
	}

	void Scope::CopyHelper(const Scope& rhs)
	{
		for (PairType* pair : rhs.mOrderVector)
		{
			if (pair->second.Type() == Datum::DatumTypes::Table)
			{
				Datum& newDatum = Append(pair->first);
				newDatum.SetType(Datum::DatumTypes::Table);
				newDatum.Reserve(pair->second.Size());

				for (size_t i = 0; i < pair->second.Size(); ++i)
				{
					Scope* newScope = pair->second.GetScope(i)->Clone();
					newScope->mParent = this;
					newDatum.PushBack(*newScope);
				}
			}
			else
			{
				mOrderVector.PushBack(&(*mMap.Insert(*pair).first));
			}
		}
	}

	void Scope::MoveHelper(const Scope* rhs) noexcept
	{
		for (PairType* pair : mOrderVector)
		{
			if (pair->second.Type() == Datum::DatumTypes::Table)
			{
				for (size_t i = 0; i < pair->second.Size(); ++i)
				{
					pair->second.GetScope(i)->mParent = this;
				}
			}
		}

		if (mParent != nullptr)
		{
			auto [retDatum, index] = mParent->FindNestedScope(rhs);
			if (retDatum != nullptr)
			{
				retDatum->Set(*this, index);
			}
		}
	}

	Scope::~Scope()
	{
		Clear();
	}

#pragma endregion

#pragma region Equality

	bool Scope::operator==(const Scope& rhs) const
	{
		if (this != &rhs)
		{
			if (mOrderVector.Size() != rhs.mOrderVector.Size())
			{
				return false;
			}

			for (size_t i = 0; i < mOrderVector.Size(); ++i)
			{
				if (mOrderVector[i]->first == "this")
				{
					continue;
				}

				const Datum* foundDatum = rhs.Find(mOrderVector[i]->first);
				if (foundDatum == nullptr)
				{
					return false;
				}

				if (mOrderVector[i]->second != *foundDatum)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

#pragma endregion

#pragma region ScopeInformation

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	size_t Scope::BucketSize() const
	{
		return mMap.BucketSize();
	}

	size_t Scope::NumAttributes() const
	{
		return mOrderVector.Size();
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		Scope* parent = scope.mParent;
		
		while (parent != nullptr)
		{
			if (scope.mParent == this)
			{
				return true;
			}

			parent = parent->mParent;
		}

		return false;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

#pragma endregion

#pragma region AtributeAccess

	const Vector<Scope::PairType*>& Scope::GetOrderVector() const
	{
		return mOrderVector;
	}

	Datum* Scope::Find(const std::string& name)
	{
		auto it = mMap.Find(name);
		if (it == mMap.end())
		{
			return nullptr;
		}

		return &it->second;
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		return const_cast<Scope*>(this)->Find(name);
	}

	Datum* Scope::Search(const std::string& name, Scope** foundScope)
	{
		Datum* ret = Find(name);

		if (ret != nullptr && foundScope != nullptr)
		{
			*foundScope = this;
		}

		if (ret == nullptr && mParent != nullptr)
		{
			ret = mParent->Search(name, foundScope);
		}

		return ret;
	}

	const Datum* Scope::Search(const std::string& name, Scope** foundScope) const
	{
		return const_cast<Scope*>(this)->Search(name, foundScope);
	}

	Datum* Scope::SearchForValue(const std::string& name, const Datum& value, Scope** foundScope)
	{
		Datum* ret = Find(name);

		if (ret == nullptr && mParent != nullptr)
		{
			ret = mParent->SearchForValue(name, value, foundScope);
		}

		if (ret != nullptr && foundScope != nullptr && value == *ret)
		{
			*foundScope = this;
		}

		return ret;
	}

	std::pair<Datum*, size_t> Scope::FindNestedScope(const Scope* scope)
	{
		for (PairType* pair : mOrderVector)
		{
			if (pair->second.Type() == Datum::DatumTypes::Table)
			{
				for (size_t i = 0; i < pair->second.Size(); ++i)
				{
					if (pair->second.GetScope(i) == scope)
					{
						return std::pair(&pair->second, i);
					}
				}
			}
		}

		return std::pair(nullptr, 0);
	}

#pragma endregion

#pragma region ModifyAtributes

	Datum& Scope::Append(const std::string& name)
	{
		if (name.size() == 0)
		{
			throw std::runtime_error("Can't append with an empty string");
		}

		auto [ret, inserted] = mMap.Insert(PairType(name, Datum()));
		if (inserted)
		{
			mOrderVector.PushBack(&(*ret));
		}
		
		return ret->second;
	}

	Scope& Scope::AppendScope(const std::string& name, size_t bucketSize)
	{
		Datum& scopeDatum = Append(name);
		scopeDatum.SetType(Datum::DatumTypes::Table);
		Scope* scope = new Scope(bucketSize);
		scope->mParent = this;
		scopeDatum.PushBack(scope);
		return *scope;
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		if (&child == this)
		{
			throw std::runtime_error("A Scope can't adopt itself");
		}

		if (child.IsAncestorOf(*this))
		{
			throw std::runtime_error("A Scope can't adopt one of its ancestor Scopes");
		}

		if (name.size() == 0)
		{
			throw std::runtime_error("Can't append with an empty string");
		}

		auto [retIt, inserted] = mMap.Insert(PairType(name, Datum()));
		retIt->second.SetType(Datum::DatumTypes::Table);
		if (inserted)
		{
			mOrderVector.PushBack(&(*retIt));
		}

		child.Orphan();
		child.mParent = this;
		retIt->second.PushBack(&child);
	}

	void Scope::Orphan()
	{
		if (mParent != nullptr)
		{
			auto [owningDatum, index] = mParent->FindNestedScope(this);
			if (owningDatum != nullptr)
			{
				owningDatum->RemoveAt(index);
			}
			mParent = nullptr;
		}
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](size_t index)
	{
		return mOrderVector[index]->second;
	}

	const Datum& Scope::operator[](size_t index) const
	{
		return mOrderVector[index]->second;
	}

	void Scope::Clear()
	{
		for (PairType* pair : mOrderVector)
		{
			if (pair->second.Type() == Datum::DatumTypes::Table)
			{
				for (size_t i = 0; i < pair->second.Size(); i++)
				{
					Scope* childScope = pair->second.GetScope(i);
					if (childScope->mParent == this)
					{
						delete childScope;
					}
				}
			}
		}

		mMap.Clear();
		mOrderVector.Clear();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}
	
#pragma endregion
	
#pragma region RTTIOverloads

	std::string Scope::ToString() const
	{
		return "Scope";
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (this == rhs)
		{
			return true;
		}

		if (rhs == nullptr)
		{
			return false;
		}

		const Scope* rhsScope = rhs->As<Scope>();
		return (rhsScope != nullptr ? *this == *rhsScope : false);
	}

#pragma endregion

}


