#include "RegistryKey.h"

#if GENLIB_WINDOWS

#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include <windows.h>
#pragma warning( pop )

using namespace General::Computer;

namespace General
{
	namespace Computer
	{
		template< typename T >
		class RegistryKeyBaseImplBase
		{
		public:
			RegistryKeyBaseImplBase( RegistryFolder p_folder, const T & p_name )
				:	m_folder( p_folder == RF_HKLM ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER ),
					m_name( p_name ),
					m_created( false )
			{
			}

			~RegistryKeyBaseImplBase();

		public:
			void Close()
			{
				if ( m_created )
				{
					RegCloseKey( m_key );
					m_created = false;
				}
			}

		protected:
			T m_name;
			HKEY m_folder;
			HKEY m_key;
			bool m_created;
		};

		template<>
		class RegistryKeyBaseImpl< std::string >
			: public RegistryKeyBaseImplBase< std::string >
		{
		public:
			RegistryKeyBaseImpl( RegistryFolder p_folder, const std::string & p_name )
				:	RegistryKeyBaseImplBase( p_folder, p_name )
			{
			}

			bool IsValid()
			{
				return m_created || Open();
			}

			bool Open()
			{
				m_created = RegOpenKeyExA( m_folder, m_name.c_str(), 0L,  KEY_ALL_ACCESS, &m_key ) == ERROR_SUCCESS;
				return m_created;
			}

			bool Delete()
			{
				bool l_return = !m_created;

				if ( m_created )
				{
					long l_result = RegDeleteKeyA( m_folder, m_name.c_str() );

					if ( l_result == ERROR_SUCCESS )
					{
						m_created = false;
						l_return = true;
					}
					else
					{
						l_return = false;
					}
				}

				return l_return;
			}

			bool Create()
			{
				bool l_return = !m_created;

				if ( !m_created )
				{
					m_created = ( RegCreateKeyExA( m_folder, m_name.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, & m_key, NULL ) == ERROR_SUCCESS );
					l_return = m_created;
				}

				return l_return;
			}

			bool SetStringValue( const std::string & p_name, const std::string & p_value )
			{
				return RegSetValueExA( m_key, p_name.c_str(), 0, REG_SZ, reinterpret_cast <const unsigned char *>( p_value.c_str() ), static_cast<unsigned long>( p_value.size() + 1 ) ) == ERROR_SUCCESS;
			}

			bool SetStringExpandValue( const std::string & p_name, const std::string & p_value )
			{
				return RegSetValueExA( m_key, p_name.c_str(), 0, REG_EXPAND_SZ, reinterpret_cast <const unsigned char *>( p_value.c_str() ), static_cast<unsigned long>( p_value.size() + 1 ) ) == ERROR_SUCCESS;
			}

			std::string GetStringValue( const std::string & p_name )
			{
				std::string l_return;
				char l_buffer[512];
				unsigned long l_size = 511;

				if ( RegQueryValueExA( m_key, p_name.c_str(), NULL, NULL, reinterpret_cast <unsigned char *>( &l_buffer[0] ), & l_size ) == ERROR_SUCCESS )
				{
					l_return = l_buffer;
				}

				return l_return;
			}
		};

		template<>
		class RegistryKeyBaseImpl< std::wstring >
			: public RegistryKeyBaseImplBase< std::wstring >
		{
		public:
			RegistryKeyBaseImpl( RegistryFolder p_folder, const std::wstring & p_name )
				: RegistryKeyBaseImplBase( p_folder, p_name )
			{
			}

			bool IsValid()
			{
				return m_created || Open();
			}

			bool Open()
			{
				m_created = RegOpenKeyExW( m_folder, m_name.c_str(), 0L,  KEY_ALL_ACCESS, &m_key ) == ERROR_SUCCESS;
				return m_created;
			}

			bool Delete()
			{
				bool l_return = !m_created;

				if ( m_created )
				{
					long l_result = RegDeleteKeyW( m_folder, m_name.c_str() );

					if ( l_result == ERROR_SUCCESS )
					{
						m_created = false;
						l_return = true;
					}
					else
					{
						l_return = false;
					}
				}

				return l_return;
			}

			bool Create()
			{
				bool l_return = !m_created;

				if ( !m_created )
				{
					m_created = ( RegCreateKeyExW( m_folder, m_name.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, & m_key, NULL ) == ERROR_SUCCESS );
					l_return = m_created;
				}

				return l_return;
			}

			bool SetStringValue( const std::wstring & p_name, const std::wstring & p_value )
			{
				return RegSetValueExW( m_key, p_name.c_str(), 0, REG_SZ, reinterpret_cast <const BYTE *>( p_value.c_str() ), static_cast<unsigned long>( p_value.size() + 1 ) ) == ERROR_SUCCESS;
			}

			bool SetStringExpandValue( const std::wstring & p_name, const std::wstring & p_value )
			{
				return RegSetValueExW( m_key, p_name.c_str(), 0, REG_EXPAND_SZ, reinterpret_cast <const BYTE *>( p_value.c_str() ), static_cast<unsigned long>( p_value.size() + 1 ) ) == ERROR_SUCCESS;
			}

			std::wstring GetStringValue( const std::wstring & p_name )
			{
				std::wstring l_return;
				wchar_t l_buffer[512];
				DWORD l_size = 511;

				if ( RegQueryValueExW( m_key, p_name.c_str(), NULL, NULL, reinterpret_cast< BYTE *>( &l_buffer[0] ), &l_size ) == ERROR_SUCCESS )
				{
					l_return = l_buffer;
				}

				return l_return;
			}
		};
	}
}

using General::Computer::RegistryKeyBaseImpl;

RegistryKeyBase< std::string >::RegistryKeyBase( RegistryFolder p_folder, const std::string & p_keyName )
	: m_impl( std::make_unique< RegistryKeyBaseImpl< std::string > >( p_folder, p_keyName ) )
	, m_exists( false )
{
}

RegistryKeyBase< std::string >::~RegistryKeyBase()
{
	Close();
}

bool RegistryKeyBase< std::string >::SetStringValue( const std::string & p_name, const std::string & p_value )
{
	return m_impl->SetStringValue( p_name, p_value );
}

bool RegistryKeyBase< std::string >::SetStringExpandValue( const std::string & p_name, const std::string & p_value )
{
	return m_impl->SetStringExpandValue( p_name, p_value );
}

std::string RegistryKeyBase< std::string >::GetStringValue( const std::string & p_valueName )
{
	return m_impl->GetStringValue( p_valueName );
}

void RegistryKeyBase< std::string >::Close()
{
	m_impl->Close();
}

bool RegistryKeyBase< std::string >::Open()
{
	return m_impl->Open();
}

bool RegistryKeyBase< std::string >::Delete()
{
	return m_impl->Delete();
}

bool RegistryKeyBase< std::string >::Create()
{
	return m_impl->Create();
}

bool RegistryKeyBase< std::string >::IsValid()
{
	return m_impl->IsValid();
}

/********************************************************************************************************************
												Wide String
********************************************************************************************************************/

RegistryKeyBase< std::wstring >::RegistryKeyBase( RegistryFolder p_folder, const std::wstring & p_keyName )
	: m_impl( std::make_unique< RegistryKeyBaseImpl< std::wstring > >( p_folder, p_keyName ) )
	, m_exists( false )
{
}

RegistryKeyBase< std::wstring >::~RegistryKeyBase()
{
	Close();
}

bool RegistryKeyBase< std::wstring >::SetStringValue( const std::wstring & p_name, const std::wstring & p_value )
{
	return m_impl->SetStringValue( p_name, p_value );
}

bool RegistryKeyBase< std::wstring >::SetStringExpandValue( const std::wstring & p_name, const std::wstring & p_value )
{
	return m_impl->SetStringExpandValue( p_name, p_value );
}

std::wstring RegistryKeyBase< std::wstring >::GetStringValue( const std::wstring & p_valueName )
{
	return m_impl->GetStringValue( p_valueName );
}

void RegistryKeyBase< std::wstring >::Close()
{
	m_impl->Close();
}

bool RegistryKeyBase< std::wstring >::Open()
{
	return m_impl->Open();
}

bool RegistryKeyBase< std::wstring >::Delete()
{
	return m_impl->Delete();
}

bool RegistryKeyBase< std::wstring >::Create()
{
	return m_impl->Create();
}

bool RegistryKeyBase< std::wstring >::IsValid()
{
	return m_impl->IsValid();
}

#endif // GENLIB_WINDOWS
