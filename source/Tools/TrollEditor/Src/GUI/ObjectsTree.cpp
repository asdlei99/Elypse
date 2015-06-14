/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "TrollEditorPch.h"

#include "GUI/ObjectsTree.h"

#include "GUI/MainFrame.h"

#include "TrollEditorApplication.h"

#include "Project/Scene.h"
#include "Project/Project.h"
#include "Project/3D/3DObject.h"
#include "Project/Temporal/TemporalObject.h"
#include "Project/Temporal/AnimationGroup.h"
#include "Project/Temporal/AnimatedObject.h"
#include "Project/Media/MediaObject.h"
#include "Project/Media/Sound.h"
#include "Project/Media/SoundObject.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"

#include "xpm/icon1.xpm"
#include "xpm/icon2.xpm"
#include "xpm/icon3.xpm"
#include "xpm/icon4.xpm"
#include "xpm/icon5.xpm"
#include "xpm/icon6.xpm"
#include "xpm/icon7.xpm"

BEGIN_TROLL_GUI_NAMESPACE
{
	enum ObjectsTreeIDs
	{
		ObjectsTreeIcon_File = 0,
		ObjectsTreeIcon_FileSelected = 1,
		ObjectsTreeIcon_Folder = 2,
		ObjectsTreeIcon_FolderSelected = 3,
		ObjectsTreeIcon_FolderOpened = 4,
		ObjectsTreeIcon_ClassFunction = 5,

		TreeObjects_Ctrl = 1002,
	};

	BEGIN_EVENT_TABLE( ObjectsTree, wxTreeCtrl )
		EVT_TREE_SEL_CHANGED( TreeObjects_Ctrl, ObjectsTree::OnObjectLClic )
		EVT_TREE_ITEM_MENU( TreeObjects_Ctrl, ObjectsTree::OnObjectRClic )
	END_EVENT_TABLE()

	ObjectsTree::ObjectsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: wxTreeCtrl( parent, TreeObjects_Ctrl, pos, size, style )
	{
		CreateImageList();
	}

	ObjectsTree::~ObjectsTree()
	{
		Cleanup();
	}

	void ObjectsTree::Cleanup()
	{
		General::Utils::map::deleteAll( m_overlays );
		General::Utils::map::deleteAll( m_sceneNodes );
	}

	void ObjectsTree::CreateImageList( int p_size )
	{
		if ( p_size == -1 )
		{
			SetImageList( NULL );
			return;
		}

		if ( p_size == 0 )
		{
			p_size = m_imageSize;
		}
		else
		{
			m_imageSize = p_size;
		}

		wxImageList * l_images = new wxImageList( p_size, p_size, true );
		wxBusyCursor l_wait;
		wxIcon l_icons[7];
		l_icons[0] = wxIcon( icon1_xpm );
		l_icons[1] = wxIcon( icon2_xpm );
		l_icons[2] = wxIcon( icon3_xpm );
		l_icons[3] = wxIcon( icon4_xpm );
		l_icons[4] = wxIcon( icon5_xpm );
		l_icons[5] = wxIcon( icon6_xpm );
		l_icons[6] = wxIcon( icon7_xpm );
		int l_sizeOrig = l_icons[0].GetWidth();

		for ( size_t i = 0; i < WXSIZEOF( l_icons ); i++ )
		{
			if ( p_size == l_sizeOrig )
			{
				l_images->Add( l_icons[i] );
			}
			else
			{
				l_images->Add( wxBitmap( wxBitmap( l_icons[i] ).ConvertToImage().Rescale( p_size, p_size ) ) );
			}
		}

		AssignImageList( l_images );
	}

	void ObjectsTree::_logEvent( const wxChar * name, const wxTreeEvent & p_event )
	{
		wxTreeItemId item = p_event.GetItem();
		wxString text;

		if ( item.IsOk() )
		{
			text << wxT( '"' ) << GetItemText( item ).c_str() << wxT( '"' );
		}
		else
		{
			text = _( "Invalid item" );
		}

		LogDebug( wxString( name ) + wxT( "(" ) + wxString( text.c_str() ) + wxT( ")" ) );
	}

	void ObjectsTree::ShowContextMenuObject( const wxPoint & pos, wxTreeItemId p_item )
	{
		wxMenu menu;

		if ( p_item == m_root )
		{
			menu.Append( saveModifs, _( "Save Modifications" ) );
		}

		PopupMenu( & menu, pos.x, pos.y );
	}

	void ObjectsTree::InitObjectList()
	{
		wxTreeItemId l_id;
		int l_image, l_imageSel;
	//	l_image = (wxGetApp().ShowImages() ? ObjectsTreeIcon_Folder : -1);
		l_image = ObjectsTreeIcon_Folder;
		m_root = AddRoot( _( "Objects" ), l_image, l_image, new TETreeItemData( wxT( " Root item" ), TITObjectsRoot ) );

		if ( l_image != -1 )
		{
			SetItemImage( m_root, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_image = ObjectsTreeIcon_Folder;
		l_imageSel = l_image + 1;
		l_id = AppendItem( m_root, _( "Scene Nodes" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITNodes ) );
		m_idParent = l_id;
		m_folders[_( "Scene Nodes" )] = l_id;
		Expand( m_root );

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Objects" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITObjects ) );
		m_idParent = l_id;
		m_folders[_( "Objects" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Lights" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITLights ) );
		m_idParent = l_id;
		m_folders[_( "Lights" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Cameras" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITCameras ) );
		m_idParent = l_id;
		m_folders[_( "Cameras" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Overlays" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITOverlays ) );
		m_idParent = l_id;
		m_folders[_( "Overlays" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Animation Groups" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITAnimationGroups ) );
		m_idParent = l_id;
		m_folders[_( "Animation Groups" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Sounds" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITSounds ) );
		m_idParent = l_id;
		m_folders[_( "Sounds" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		l_id = AppendItem( m_root, _( "Videos" ), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, TITVideos ) );
		m_idParent = l_id;
		m_folders[_( "Videos" )] = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}
	}

	void ObjectsTree::AddSceneNode( const wxString & p_name, const wxString & p_parentName )
	{
		wxTreeItemId l_id;
		TROLL_PROJECT_3D_NAMESPACE::Troll3DObject * l_node = reinterpret_cast< TROLL_PROJECT_3D_NAMESPACE::Troll3DObject * >( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( p_name ) );
		int l_image, l_imageSel;
		bool l_added = false;
		l_image = ObjectsTreeIcon_File;
		l_imageSel = l_image + 1;
		RecursiveTreeItemObject * l_item = new RecursiveTreeItemObject;
		l_item->ItemObject.Object = l_node;

		if ( p_parentName.empty() )
		{
			l_id = AppendItem( m_folders.find( _( "Scene Nodes" ) )->second, l_node->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITNode ) );
			l_added = true;
			l_item->ItemObject.ItemID = l_id;
			m_sceneNodes[wxTreeItemIdValue( l_id.m_pItem )] = l_item;
	//		std::cout << "TreeCtrl::AddSceneNode - TrollSceneNode " << p_name << " added in list\n";
		}
		else
		{
			RecursiveTreeItemObjectPtrMap::iterator l_it = m_sceneNodes.begin();
			RecursiveTreeItemObject * l_parentItem = NULL;

			while ( l_it != m_sceneNodes.end() && ( l_parentItem = l_it->second->RecursiveIsIn( p_parentName ) ) == NULL )
			{
				++l_it;
			}

			if ( l_parentItem )
			{
				std::clog << "TreeCtrl::AddSceneNode - Parent " << p_parentName << " for " << p_name << " found in list\n";
				l_id = AppendItem( l_parentItem->ItemObject.ItemID, l_node->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITNode ) );
				l_added = true;
				l_item->ItemObject.ItemID = l_id;
				l_parentItem->Childs[wxTreeItemIdValue( l_id.m_pItem )] = l_item;
			}
			else
			{
				std::clog << "TreeCtrl::AddSceneNode - Parent " << p_parentName << " for " << p_name << " not found in list\n";
			}
		}

		if ( l_added )
		{
			m_idParent = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( l_id, l_imageSel, wxTreeItemIcon_Expanded );
			}

			wxRect rcRect = GetRect();
			Refresh( true, &rcRect );
			Update();
		}
		else
		{
			delete l_item;
		}
	}

	TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * ObjectsTree::AddOverlay( TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * p_overlay, const wxString & p_parentName )
	{
		wxTreeItemId l_id;
		TROLL_PROJECT_NAMESPACE::Object * l_overlay = reinterpret_cast< TROLL_PROJECT_NAMESPACE::Object * >( p_overlay );
		int l_image, l_imageSel;
		bool l_added = false;
		l_image = ObjectsTreeIcon_File;
		l_imageSel = l_image + 1;
		RecursiveTreeItemObject * l_item = new RecursiveTreeItemObject;
		l_item->ItemObject.Object = l_overlay;

		if ( p_parentName.empty() )
		{
			l_id = AppendItem( m_folders.find( _( "Overlays" ) )->second, l_overlay->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITOverlay ) );
			l_added = true;
			l_item->ItemObject.ItemID = l_id;
			m_overlays.insert( std::make_pair( wxTreeItemIdValue( l_id.m_pItem ), l_item ) );
		}
		else
		{
			RecursiveTreeItemObjectPtrMap::iterator l_it = m_overlays.begin();
			RecursiveTreeItemObject * l_parentItem = NULL;

			while ( l_it != m_overlays.end() && ( l_parentItem = l_it->second->RecursiveIsIn( p_parentName ) ) == NULL )
			{
				++l_it;
			}

			if ( l_parentItem )
			{
				l_id = AppendItem( l_parentItem->ItemObject.ItemID, l_overlay->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITOverlay ) );
				l_added = true;
				l_item->ItemObject.ItemID = l_id;
				l_parentItem->Childs.insert( std::make_pair( wxTreeItemIdValue( l_id.m_pItem ), l_item ) );
			}
		}

		if ( l_added )
		{
			m_idParent = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( l_id, l_imageSel, wxTreeItemIcon_Expanded );
			}

			wxRect rcRect = GetRect();
			Refresh( true, &rcRect );
			Update();
			return p_overlay;
		}

		return NULL;
	}

	void ObjectsTree::AddSceneObject( TROLL_PROJECT_NAMESPACE::Object * p_object )
	{
		std::clog << "TETreeCtrl::AddSceneObject - Type : " << p_object->GetObjectType() << "\n";
		if ( p_object->GetObjectType() == TROLL_PROJECT_NAMESPACE::tt3DObject )
		{
			TROLL_PROJECT_3D_NAMESPACE::Troll3DObject * l_3DObject = reinterpret_cast< TROLL_PROJECT_3D_NAMESPACE::Troll3DObject * >( p_object );
			_add3DObject( l_3DObject );
		}
		else if ( p_object->GetObjectType() == TROLL_PROJECT_NAMESPACE::ttTemporalObject )
		{
			TROLL_PROJECT_TEMPORAL_NAMESPACE::TemporalObject * l_temporalObject = reinterpret_cast< TROLL_PROJECT_TEMPORAL_NAMESPACE::TemporalObject * >( p_object );
			_addTemporalObject( l_temporalObject );
		}
		else if ( p_object->GetObjectType() == TROLL_PROJECT_NAMESPACE::ttMediaObject )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::MediaObject * l_mediaObject = reinterpret_cast< TROLL_PROJECT_MEDIA_NAMESPACE::MediaObject  *>( p_object );
			_addMediaObject( l_mediaObject );
		}

		wxRect rcRect = GetRect();
		Refresh( true, &rcRect );
		Update();
	}

	wxTreeItemId ObjectsTree::GetFolderId( const wxString & p_name )
	{
		if ( m_folders.find( p_name ) != m_folders.end() )
		{
			return m_folders.find( p_name )->second;
		}

		return 0;
	}

	void ObjectsTree::OnObjectLClic( wxTreeEvent & p_event )
	{
		wxTreeItemId l_item = p_event.GetItem();
		TETreeItemData * l_itemData = reinterpret_cast <TETreeItemData *>( GetItemData( l_item ) );
		wxString l_itemType = l_itemData->GetDesc();

		if ( l_itemType != ITEM_DESC_FOLDER )
		{
			TreeItemObjectMap::iterator l_it;
			wxTreeItemIdValue l_itemIndex = wxTreeItemIdValue( l_item.m_pItem );
			RecursiveTreeItemObject * l_elem;
			l_it = m_objects.find( l_itemIndex );

			if ( l_it != m_objects.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Entities\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			l_it = m_lights.find( l_itemIndex );

			if ( l_it != m_lights.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Lights\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			l_it = m_cameras.find( l_itemIndex );

			if ( l_it != m_cameras.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Cameras\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			l_it = m_animationGroups.find( l_itemIndex );

			if ( l_it != m_animationGroups.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Animation Groups\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			l_it = m_animatedObjects.find( l_itemIndex );

			if ( l_it != m_animatedObjects.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Animated Objects\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			l_it = m_sounds.find( l_itemIndex );

			if ( l_it != m_sounds.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Sounds\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			l_it = m_videos.find( l_itemIndex );

			if ( l_it != m_videos.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Videos\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it->second.Object );
				return;
			}

			RecursiveTreeItemObjectPtrMap::iterator l_it2 = m_overlays.find( l_itemIndex );

			if ( l_it2 != m_overlays.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Overlays\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it2->second->ItemObject.Object );
				return;
			}

			l_it2 = m_overlays.begin();

			while ( l_it2 != m_overlays.end() )
			{
				if ( ( l_elem = l_it2->second->RecursiveIsIn( l_item.m_pItem ) ) != NULL )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Overlay Elements\n";
					GUI::MainFrame::GetInstance()->ShowObjectInfos( l_elem->ItemObject.Object );
					return;
				}

				++l_it2;
			}

			l_it2 = m_sceneNodes.find( l_itemIndex );

			if ( l_it2 != m_sceneNodes.end() )
			{
				std::clog << "TETreeCtrl::OnObjectClic - Found in Scene Nodes\n";
				GUI::MainFrame::GetInstance()->ShowObjectInfos( l_it2->second->ItemObject.Object );
				return;
			}

			l_it2 = m_sceneNodes.begin();

			while ( l_it2 != m_sceneNodes.end() )
			{
				if ( ( l_elem = l_it2->second->RecursiveIsIn( l_item.m_pItem ) ) != NULL )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Scene Nodes Childs\n";
					GUI::MainFrame::GetInstance()->ShowObjectInfos( l_elem->ItemObject.Object );
					return;
				}

				++l_it2;
			}
		}
	}

	void ObjectsTree::OnObjectRClic( wxTreeEvent & p_event )
	{
		wxTreeItemId l_item = p_event.GetItem();
		wxPoint l_pos = p_event.GetPoint();

		if ( l_item == m_root )
		{
			std::clog << "Root right click\n";
			ShowContextMenuObject( l_pos, l_item );
		}
	}

	void ObjectsTree::_add3DObject( TROLL_PROJECT_3D_NAMESPACE::Troll3DObject * p_object )
	{
		wxTreeItemId l_id;
		bool l_added = false;
		TreeItemObject l_object;
		int l_image, l_imageSel;
		l_image = ObjectsTreeIcon_File;
		l_imageSel = l_image + 1;

		if ( p_object->Get3DObjectType() == TROLL_PROJECT_3D_NAMESPACE::ttEntity )
		{
			l_id = AppendItem( m_folders.find( _( "Objects" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITObject ) );
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_objects[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			std::clog << "TETreeCtrl::AddSceneObject - Added Object - ID : " << l_id << " - NbElems : " << m_objects.size() << "\n";
			l_added = true;
		}
		else if ( p_object->Get3DObjectType() == TROLL_PROJECT_3D_NAMESPACE::ttLight )
		{
			l_id = AppendItem( m_folders.find( _( "Lights" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITLight ) );
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_lights[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
		else if ( p_object->Get3DObjectType() == TROLL_PROJECT_3D_NAMESPACE::ttCamera )
		{
			l_id = AppendItem( m_folders.find( _( "Cameras" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITCamera ) );
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_cameras[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}

		if ( l_added )
		{
			m_idParent = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( l_id, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}
		}
	}

	void ObjectsTree::_addTemporalObject( TROLL_PROJECT_TEMPORAL_NAMESPACE::TemporalObject * p_object )
	{
		wxTreeItemId l_id;
		bool l_added = false;
		TreeItemObject l_object;
		int l_image, l_imageSel;
		l_image = ObjectsTreeIcon_File;
		l_imageSel = l_image + 1;

		if ( p_object->GetTemporalObjectType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ttAnimationGroup )
		{
			l_id = AppendItem( m_folders.find( _( "Animation Groups" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITAnimationGroup ) );
			TreeItemObject l_object;
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_animationGroups[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
		else if ( p_object->GetTemporalObjectType() == TROLL_PROJECT_TEMPORAL_NAMESPACE::ttAnimatedObject )
		{
			TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollAnimatedObject * l_animatedObject = reinterpret_cast< TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollAnimatedObject * >( p_object );
			wxString l_groupName = l_animatedObject->GetGroup()->GetObjectName();
			wxTreeItemId l_parentId;
			TreeItemObjectMap::iterator l_it = m_animationGroups.begin();
			bool l_found = false;

			std::clog << "TETreeCtrl::AddSceneObject - Looking for group " << l_groupName << "\n";
			while ( l_it != m_animationGroups.end() && ! l_found )
			{
				std::clog << "TETreeCtrl::AddSceneObject - Current group : " << l_it->second.Object->GetObjectName() << "\n";
				if ( l_it->second.Object->GetObjectName() == l_groupName )
				{
					l_parentId = l_it->second.ItemID;
					l_found = true;
				}

				++l_it;
			}

			if ( l_found )
			{
				std::clog << "TETreeCtrl::AddSceneObject - Group found\n";
				l_id = AppendItem( l_parentId, l_animatedObject->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITAnimatedObject ) );
				TreeItemObject l_object;
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_animatedObjects[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
		}

		if ( l_added )
		{
			m_idParent = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( l_id, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}
		}
	}

	void ObjectsTree::_addMediaObject( TROLL_PROJECT_MEDIA_NAMESPACE::MediaObject * p_object )
	{
		if ( ! p_object )
		{
			return;
		}

		wxTreeItemId l_id;
		bool l_added = false;
		TreeItemObject l_object;
		int l_image, l_imageSel;
		l_image = ObjectsTreeIcon_File;
		l_imageSel = l_image + 1;

		if ( p_object->GetMediaObjectType() == TROLL_PROJECT_MEDIA_NAMESPACE::ttSound )
		{
			l_id = AppendItem( m_folders.find( _( "Sounds" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITSound ) );
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
		else if ( p_object->GetMediaObjectType() == TROLL_PROJECT_MEDIA_NAMESPACE::ttSoundObject )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::TrollSoundObject * l_soundObject = reinterpret_cast< TROLL_PROJECT_MEDIA_NAMESPACE::TrollSoundObject * >( p_object );

			if ( ! l_soundObject->GetSound() )
			{
				return;
			}

			wxString l_soundName = l_soundObject->GetSound()->GetObjectName();
			wxTreeItemId l_parentId;
			TreeItemObjectMap::iterator l_it = m_sounds.begin();
			bool l_found = false;

			std::clog << "TETreeCtrl::AddSceneObject - Looking for parent node " << l_soundName << "\n";
			while ( l_it != m_sounds.end() && ! l_found )
			{
				std::clog << "TETreeCtrl::AddSceneObject - Current node : " << l_it->second.Object->GetObjectName() << "\n";
				if ( l_it->second.Object->GetObjectName() == l_soundName )
				{
					l_parentId = l_it->second.ItemID;
					l_found = true;
				}

				++l_it;
			}

			if ( l_found )
			{
				std::clog << "TETreeCtrl::AddSceneObject - Parent node found\n";
				l_id = AppendItem( l_parentId, l_soundObject->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITSoundObject ) );
				TreeItemObject l_object;
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
		}
		else if ( p_object->GetMediaObjectType() == TROLL_PROJECT_MEDIA_NAMESPACE::ttSoundPlaylist )
		{
			l_id = AppendItem( m_folders.find( _( "Sounds" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITSoundPlaylist ) );
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
		else if ( p_object->GetMediaObjectType() == TROLL_PROJECT_MEDIA_NAMESPACE::ttVideo )
		{
			l_id = AppendItem( m_folders.find( _( "Videos" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITVideo ) );
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_videos[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
		else if ( p_object->GetMediaObjectType() == TROLL_PROJECT_MEDIA_NAMESPACE::ttVideoObject )
		{
			TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * l_videoObject = reinterpret_cast< TROLL_PROJECT_MEDIA_NAMESPACE::TrollVideoObject * >( p_object );

			if ( ! l_videoObject->GetVideo() )
			{
				return;
			}

			wxString l_videoName = l_videoObject->GetVideo()->GetObjectName();
			wxTreeItemId l_parentId;
			TreeItemObjectMap::iterator l_it = m_videos.begin();
			bool l_found = false;
			std::clog << "TETreeCtrl::AddVideoObject - Looking for parent node " << l_videoName << "\n";

			while ( l_it != m_videos.end() && ! l_found )
			{
				std::clog << "TETreeCtrl::AddVideoObject - Current node : " << l_it->second.Object->GetObjectName() << "\n";

				if ( l_it->second.Object->GetObjectName() == l_videoName )
				{
					l_parentId = l_it->second.ItemID;
					l_found = true;
				}

				++l_it;
			}

			if ( l_found )
			{
				std::clog << "TETreeCtrl::AddSceneObject - Parent node found\n";
				l_id = AppendItem( l_parentId, l_videoObject->GetObjectName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, TITVideoObject ) );
				TreeItemObject l_object;
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_videos[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
		}

		if ( l_added )
		{
			m_idParent = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( l_id, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}
		}
	}
}
END_TROLL_GUI_NAMESPACE
