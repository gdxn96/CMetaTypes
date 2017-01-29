#pragma once
#include <type_traits>
#include <string>
#include <rapidjson\rapidjson.h>
#include <rapidjson\document.h>
#include <rapidjson\prettywriter.h>
#include <unordered_map>

struct Variable;
class MetaType;

#define META_DEBUGGING true

#ifdef _DEBUG 
#define META_DEBUGGING _DEBUG
#endif 
#ifdef NDEBUG 
#define META_DEBUGGING !NDEBUG
#endif 

#define DEBUG(x) if (META_DEBUGGING) { std::cerr << x << std::endl; }

// Takes a pointer and returns a pointer offset in bytes
#define PTR_ADD( PTR, OFFSET ) \
  ((void *)(((char *)(PTR)) + (OFFSET)))

typedef std::string(*SerializeFn)(void*, const MetaType* m);
typedef void(*DeSerializeFn)(void*, rapidjson::Value&, const MetaType*);


#define DEFINE_META( T ) \
  MetaCreator<std::remove_cv<T>::type> UNIQUE_NAME_GENERATOR( )( #T, sizeof( T ) ); \
  std::remove_cv<T>::type *T::NullCast( void ) { return reinterpret_cast<std::remove_cv<T>::type *>(NULL); } \
  void T::AddMember( std::string name, unsigned offset, MetaType *data ) { return MetaCreator<std::remove_cv<T>::type>::AddMember( name, offset, data ); } \
  void MetaCreator<std::remove_cv<T>::type>::RegisterMetaType( void ) { T::RegisterMetaType( ); } \
  void T::RegisterMetaType( void )

// META_DATA
// Purpose : This macro goes on the inside of a class within the public section. It declares
//           a few member functions for use by the MetaType system to retrieve information about
//           the class.
#define META_DATA( T ) \
  static void AddMember( std::string name, unsigned offset, MetaType *data ); \
  static std::remove_cv<T>::type *NullCast( void ); \
  static void RegisterMetaType( void )

// Defines the RegisterMetaType for you
#define DEFINE_META_PRIMITIVE( T ) \
  MetaCreator<std::remove_cv<T>::type> UNIQUE_NAME_GENERATOR( )( #T, sizeof( T ) ); \
  void MetaCreator<std::remove_cv<T>::type>::RegisterMetaType( void ) \
  { \
    MetaCreator<std::remove_cv<T>::type>::SetSerializeFn( MetaType::ToJson<std::remove_cv<T>::type> ); \
	MetaCreator<std::remove_cv<T>::type>::SetDeSerializeFn( MetaType::FromJson<std::remove_cv<T>::type> ); \
  }

#define ADD_MEMBER( MEMBER ) \
  AddMember( #MEMBER, (unsigned)(&(NullCast( )->MEMBER)), META( NullCast( )->MEMBER ))

#define PASTE_TOKENS( _, __ ) _##__
#define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
#define UNIQUE_NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )

//
// META_TYPE
// Purpose: Retrieves the proper MetaType instance of an object by type.
//
#define META_TYPE( T ) (MetaCreator<std::remove_cv<T>::type>::Get( ))

//
// META
// Purpose: Retrieves the proper MetaType instance of an object by an object's type.
//
#define META( OBJECT ) (MetaCreator<std::remove_cv<decltype( OBJECT )>::type>::Get( ))

//
// META_STR
// Purpose : Finds a MetaType instance by string name
//
#define META_STR( STRING ) (MetaManager::Get( STRING ))

class MetaType;

//
// Member
// Purpose: Stores information (name and offset of member) about a data member of a specific class. Multiple
//          Member objects can be stored in MetaType objects within a std::vector.
//
class Member
{
public:
	Member(std::string string, unsigned val, MetaType *meta);
	~Member();

	const std::string &Name(void) const; // Gettor for name
	unsigned Offset(void) const; // Gettor for offset
	const MetaType *Meta(void) const; // Gettor for data

	Member *& Next(void);
	Member *const& Next(void) const;

private:
	std::string name;
	unsigned offset;
	const MetaType *data;
	Member *next;
};

// MetaType
// Purpose: Object for holding various info about any C++ type for the MetaType reflection system.
//
class MetaType
{
public:
	MetaType(std::string string = "", unsigned val = 0);
	~MetaType();

	void Init(std::string string, unsigned val);

	const std::string& Name(void) const;
	
	unsigned Size(void) const;
	void AddMember(const Member *member);
	bool HasMembers(void) const;

	void Copy(void *data, const void *src) const;
	void Delete(void *data) const;
	void *NewCopy(const void *src) const;
	void *New(void) const;

	const Member *Members(void) const;
	void PrintMembers(std::ostream& os) const;

	void SetSerialize(SerializeFn fn = NULL);
	void SetDeSerialize(DeSerializeFn fn = NULL);
	std::string MetaType::Serialize(void* v, const MetaType * m=nullptr) const;
	void MetaType::DeSerialize(void* v, rapidjson::Value& val, const MetaType * m = nullptr) const;

#pragma region ToJson
	const std::string ToJson(Variable& var) const;

	template<typename T>
	static std::string ToJson(void* v, const MetaType* m) 
	{ 
		if (m != nullptr)
		{
			DEBUG("Serialization not supported for type: " + m->m_name);
		}
		else
		{
			DEBUG("Serialization not supported for unregistered type");
		}
		return "null"; 
	};

	static std::string ToJson(void* v, const MetaType* m);

	template<>
	static std::string ToJson<int>(void* val, const MetaType* m) {
		static char buf[65];
		sprintf_s(buf, "%d", *(int*)val);
		return buf;
	}

	template<>
	static std::string ToJson<bool>(void* val, const MetaType* m) {
		bool boolean = *(bool*)val;
		if (boolean)
		{
			return "true";
		}
		else
		{
			return "false";
		}
	}

	template<>
	static std::string ToJson<double>(void* val, const MetaType* m) {
		static char buf[65];
		sprintf_s(buf, "%d", *(double*)val);
		return buf;
	}

	template<>
	static std::string ToJson<long>(void* val, const MetaType* m) {
		static char buf[65];
		sprintf_s(buf, "%d", *(long*)val);
		return buf;
	}

	template<>
	static std::string ToJson<float>(void* val, const MetaType* m) {
		static char buf[65];
		sprintf_s(buf, "%d", *(float*)val);
		return buf;
	}

	template<>
	static std::string ToJson<char *>(void* val, const MetaType* m) {
		std::string result = "\"";
		result += (*(char**)(val));
		return result + "\"";
	}

	template<>
	static std::string ToJson<const char*>(void* val, const MetaType* m) {
		std::string result = "\"";
		result += (*(char**)(val));
		return result + "\"";
	}

	template<>
	static std::string ToJson<std::string>(void* val, const MetaType* m) {
		std::string result = "\"";
		result += (*(std::string*)(val));
		return result + "\"";
	}
#pragma endregion

	
#pragma region FromJson
	static void FromJson(rapidjson::Value& src, void* dest, const MetaType* type)
	{
		if (type->HasMembers())
		{
			const Member* mem = type->Members();
			while (mem)
			{
				assert(src.IsObject());
				assert(src.HasMember(mem->Name().c_str()));//json member not present in object

				rapidjson::Value& memberVal = src[mem->Name().c_str()];
				void* memberDest = PTR_ADD(dest, mem->Offset());
				mem->Meta()->DeSerialize(memberDest, memberVal, mem->Meta());

				mem = mem->Next();
			}
		}
		else
		{
			type->DeSerialize(dest, src, type);
		}
	}
	template<typename T>
	static void FromJson(void* v, rapidjson::Value&, const MetaType* m) 
	{
		DEBUG("DeSerialization not supported for type: " + m->m_name);
	};

	template<>
	static void FromJson<int>(void* v, rapidjson::Value& rVal, const MetaType* m)
	{
		*(int*)v = rVal.GetInt();
	}

	template<>
	static void FromJson<long>(void* v, rapidjson::Value& rVal, const MetaType* m)
	{
		*(long*)v = rVal.GetInt();
	}

	template<>
	static void FromJson<bool>(void* v, rapidjson::Value& rVal, const MetaType* m)
	{
		*(bool*)v = rVal.GetBool();
	}

	//char* and const char* are weird, maybe I can't modify the memory directly?
	//template<>
	//static void FromJson<char*>(void* v, rapidjson::Value& rVal, const MetaType* m)
	//{
	//	const char * string = static_cast<char*> (v);
	//	string = rVal.GetString();
	//}

	//template<>
	//static void FromJson<const char*>(void* v, rapidjson::Value& rVal, const MetaType* m)
	//{
	//	const char * string = static_cast<char*> (v);
	//	string = rVal.GetString();
	//}

	template<>
	static void FromJson<std::string>(void* v, rapidjson::Value& rVal, const MetaType* m)
	{
		*(std::string*)v = rVal.GetString();
	}

	template<>
	static void FromJson<float>(void* v, rapidjson::Value& rVal, const MetaType* m)
	{
		*(float*)v = rVal.GetFloat();
	}
#pragma endregion

	

private:
	SerializeFn serialize;
	DeSerializeFn deserialize;
	Member *m_members;
	Member *m_lastMember;
	std::string m_name;
	unsigned m_size;
};

std::string PrettifyJson(rapidjson::Document & val)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	val.Accept(writer);
	std::string json = buffer.GetString();
	return json;
}

struct Variable
{
	Variable() :
		address(NULL),
		type(NULL)
	{}

	Variable(void *d, const MetaType *m) : address(d), type(m)
	{

	}

	template<typename Var>
	Variable(Var* var) :
		address(var),
		type(META_TYPE(Var))
	{}

	template <typename T>
	void FromJson(std::string json)
	{
		rapidjson::Document document;
		document.Parse(json.c_str());
		assert(document.IsObject());
		type->FromJson(document, address, type);
	}

	std::string ToJson()
	{
		std::string json = type->ToJson(*this);
		rapidjson::Document d;
		d.Parse(json.c_str());
		json = PrettifyJson(d);
		return json;
	}

	template <typename T>
	T& get()
	{
		return *reinterpret_cast<T *>(address);
	}

	void* getAddress()
	{
		return address;
	}

	Variable& operator=(const Variable& rhs)
	{
		if (this == &rhs)
			return *this;

		if (type)
		{
			if (type == rhs.type)
				type->Copy(address, rhs.address);
			else
			{
				assert(rhs.type); // Cannot make an instance of NULL meta!

				type->Delete(address);
				type = rhs.type;

				// We require a new copy if meta does not match!
				if (type)
					address = type->NewCopy(&rhs.address);
			}
		}

		return *this;
	}

	void*  address;
	const MetaType* type;
};

template <typename Type>
class MetaCreator
{
public:
	MetaCreator(std::string name, unsigned size)
	{
		Init(name, size);
	}

	static void Init(std::string name, unsigned size)
	{
		Get()->Init(name, size);
		RegisterMetaType();
	}

	static void AddMember(std::string memberName, unsigned memberOffset, MetaType *meta)
	{
		Get()->AddMember(new Member(memberName, memberOffset, meta));
	}

	static void SetSerializeFn(SerializeFn fn)
	{
		Get()->SetSerialize(fn);
	}

	static void SetDeSerializeFn(DeSerializeFn fn)
	{
		Get()->SetDeSerialize(fn);
	}

	static Type *NullCast(void)
	{
		return reinterpret_cast<Type *>(NULL);
	}

	static void RegisterMetaType(void);

	// Ensure a single instance can exist for this class type
	static MetaType *Get(void)
	{
		static MetaType instance;
		return &instance;
	}
};

const std::string MetaType::ToJson(Variable& var) const
{
	std::string result;
	if (HasMembers())
	{
		result += "{";
		const Member *mem = m_members;
		while (mem)
		{
			void *offsetData = PTR_ADD(var.getAddress(), mem->Offset());
			Variable member = Variable(offsetData, mem->Meta());
			result += "\"" + mem->Name() + "\"" + ":" + member.type->Serialize(member.getAddress(), member.type);
			if (mem->Next())
			{
				result += ",";
			}

			mem = mem->Next();
		}

		result += "}";
	}
	else
	{
		result += var.type->Serialize(var.getAddress());
	}

	return result;
}




//---------------------------------------------------------

Member::Member(std::string string, unsigned val, MetaType *meta) : name(string), offset(val), data(meta), next(NULL)
{
}

Member::~Member()
{
}

const std::string& Member::Name(void) const
{
	return name;
}

unsigned Member::Offset(void) const
{
	return offset;
}

const MetaType *Member::Meta(void) const
{
	return data;
}

Member *& Member::Next(void)
{
	return next;
}

Member *const& Member::Next(void) const
{
	return next;
}

MetaType::MetaType(std::string string, unsigned val) : m_name(string), m_size(val), m_members(NULL), m_lastMember(NULL)
{
}

MetaType::~MetaType()
{
}

void MetaType::Init(std::string string, unsigned val)
{
	m_name = string;
	m_size = val;
}

const std::string& MetaType::Name(void) const
{
	return m_name;
}

unsigned MetaType::Size(void) const
{
	return m_size;
}

void MetaType::AddMember(const Member *member)
{
	if (!m_members)
		m_members = const_cast<Member *>(member);
	else
		m_lastMember->Next() = const_cast<Member *>(member);

	m_lastMember = const_cast<Member *>(member);
}

bool MetaType::HasMembers(void) const
{
	return (m_members) ? true : false;
}

void MetaType::Copy(void *dest, const void *src) const
{
	memcpy(dest, src, m_size);
}

void MetaType::Delete(void *data) const
{
	delete[] reinterpret_cast<char *>(data);
	data = NULL;
}

void *MetaType::NewCopy(const void *src) const
{
	void *data = new char[m_size];
	memcpy(data, src, m_size);
	return data;
}

void *MetaType::New(void) const
{
	return new char[m_size];
}

const Member *MetaType::Members(void) const
{
	return m_members;
}

void MetaType::SetSerialize(SerializeFn fn)
{
	serialize = fn;
}

inline void MetaType::SetDeSerialize(DeSerializeFn fn)
{
	deserialize = fn;
}

std::string MetaType::Serialize(void* v, const MetaType * m) const
{
	if (serialize)
		return serialize(v, m);
	else
	{
		if (m != nullptr)
		{
			return ToJson(v, m);
		}
		else
		{
			return ToJson<void>(v, m);
		}
	}
		
			
}

inline void MetaType::DeSerialize(void * v, rapidjson::Value & val, const MetaType * m) const
{
	if (deserialize)
		deserialize(v, val, m);
	else
		if (m != nullptr)
			FromJson(val, v, m);
		else
			FromJson<void>(v, val, m);
}

std::string MetaType::ToJson(void* v, const MetaType* m) { return m->ToJson(Variable(v, m)); }



DEFINE_META_PRIMITIVE(int);
DEFINE_META_PRIMITIVE(std::string);
DEFINE_META_PRIMITIVE(float);
DEFINE_META_PRIMITIVE(const char*);
DEFINE_META_PRIMITIVE(char*);
DEFINE_META_PRIMITIVE(char);
DEFINE_META_PRIMITIVE(signed char);
DEFINE_META_PRIMITIVE(short int);
DEFINE_META_PRIMITIVE(long int);
DEFINE_META_PRIMITIVE(unsigned char);
DEFINE_META_PRIMITIVE(unsigned short int);
DEFINE_META_PRIMITIVE(unsigned int);
DEFINE_META_PRIMITIVE(unsigned long int);
DEFINE_META_PRIMITIVE(wchar_t);
DEFINE_META_PRIMITIVE(bool);
DEFINE_META_PRIMITIVE(double);
DEFINE_META_PRIMITIVE(long double);