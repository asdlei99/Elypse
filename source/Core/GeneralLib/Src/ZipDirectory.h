/*
See LICENSE file in root folder
*/
#ifndef ___ZIP_DIRECTORY_H___
#define ___ZIP_DIRECTORY_H___

#include "ZipBase.h"
#include "ZipFile.h"
#include "STLMapMacros.h"
#include "File.h"

namespace General
{
	namespace Files
	{
		template< typename CharType >
		class ZipDirectoryBase
		{
		private:
			typedef std::basic_string< CharType > StringType;
			typedef std::map< StringType, ZipDirectoryBase< CharType > * > ZipDirectoryMapType;
			typedef std::map< StringType, ZipFileBase< CharType > * > ZipFileMapType;

		public:
			inline ZipDirectoryBase( StringType const & p_name, ZipDirectoryBase< CharType > * p_parent )
				: m_name{ p_name }
				, m_parent{ p_parent }
				, m_uncompressedSize{ 0 }
				, m_compressedSize{ 0 }
			{
				if ( m_parent )
				{
					m_fullpath = m_parent->GetPath() + m_name + CharType( '/' );
				}
			}

			inline ~ZipDirectoryBase()
			{
				General::Utils::map::deleteAll( m_subDirectories );
				General::Utils::map::deleteAll( m_files );
			}

			inline bool Extract( StringType const & p_newPath, ZipArchiveBase< CharType > * p_archive )
			{
				if ( !DirectoryExists( p_newPath + m_fullpath ) )
				{
					DirectoryCreate( p_newPath + m_fullpath );
				}

				for ( auto & l_dir : m_subDirectories )
				{
					if ( !l_dir.second->Extract( p_newPath, p_archive ) )
					{
						return false;
					}
				}

				for ( auto & l_file : m_files )
				{
					if ( !l_file.second->Extract( p_newPath, p_archive ) )
					{
						return false;
					}
				}

				return true;
			}

			inline ZipFileBase< CharType > * AddFile( uint64_t p_index, StringType const & p_filename, StringType const & p_fullpath )
			{
				auto const & ifind = m_files.find( p_filename );

				if ( ifind != m_files.end() )
				{
					return ifind->second;
				}

				auto * l_file = new ZipFileBase< CharType > ( p_index, p_filename, p_fullpath, this );
				m_files.insert( { p_filename, l_file } );
				return l_file;
			}

			inline ZipDirectoryBase< CharType > * AddDirectory( StringType const & p_name )
			{
				auto const & ifind = m_subDirectories.find( p_name );

				if ( ifind != m_subDirectories.end() )
				{
					return ifind->second;
				}

				auto * l_directory = new ZipDirectoryBase< CharType > ( p_name, this );
				m_subDirectories.insert( std::make_pair( p_name, l_directory ) );
				return l_directory;
			}

			inline ZipFileArray RecurseFindFile( StringType const & p_name, StringType const & p_currentDir = StringType() )
			{
				StringType l_currentDir = p_currentDir + CharType( '/' ) + m_name;
				ZipFileArray l_array;

				for ( auto & l_dir : m_subDirectories )
				{
					auto const & l_tempArray = l_dir->RecurseFindFile( p_name, l_currentDir );

					if ( !l_tempArray.empty() )
					{
						l_array.insert( l_array.end(), l_tempArray.begin(), l_tempArray.end() );
					}
				}

				auto const & ifind = m_files.find( p_name );

				if ( ifind != m_files.end() )
				{
					l_array.push_back( ifind->second );
				}

				return l_array;
			}

			inline void CheckDirectorySize()
			{
				m_uncompressedSize = 0;
				m_compressedSize = 0;

				for ( auto & l_dir : m_subDirectories )
				{
					l_dir.second->CheckDirectorySize();
					m_uncompressedSize += l_dir.second->GetUncompressedSize();
					m_compressedSize += l_dir.second->GetCompressedSize();
				}

				for ( auto & l_file : m_files )
				{
					m_uncompressedSize += l_file.second->GetUncompressedSize();
					m_compressedSize += l_file.second->GetCompressedSize();
				}
			}

			inline StringType const & GetName()const
			{
				return m_name;
			}

			inline uint64_t GetCompressedSize()const
			{
				return m_compressedSize;
			}

			inline uint64_t GetUncompressedSize()const
			{
				return m_uncompressedSize;
			}

			inline StringType const & GetPath()const
			{
				return m_fullpath;
			}

		private:
			StringType m_name;
			StringType m_fullpath;
			ZipDirectoryMapType m_subDirectories;
			ZipFileMapType m_files;
			ZipDirectoryBase< CharType > * m_parent;
			uint64_t m_uncompressedSize;
			uint64_t m_compressedSize;
		};
	}
}

#endif
