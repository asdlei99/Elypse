#include "PrecompiledHeader.h"

#include "EMuseLoadingBar.h"
#include "EMuseController.h"
#include "EMuseInstance.h"
#include "EMusePlugin.h"

#include <OgreException.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>

EMuseLoadingBar :: EMuseLoadingBar()
	:	m_window( NULL ),
		m_loadOverlay( NULL ),
		m_loadingBarElement( NULL ),
		m_loadingDescriptionElement( NULL ),
		m_loadingPercentElement( NULL ),

		m_loadingOverlayName( "Core/LoadOverlay" ),
		m_loadingBarProgressName( "Core/LoadPanel/Bar/Progress" ),
		m_loadingBarPercentageName( "Core/LoadPanel/Bar/Percentage" ),
		m_loadingBarName( "Core/LoadPanel/Bar" ),
		m_loadingBarOffset( 22 ),
		m_started( false )
{
}

EMuseLoadingBar :: ~EMuseLoadingBar()
{
	if ( m_started )
	{
		finish();
	}
}

void EMuseLoadingBar :: finish()
{
	if ( m_started )
	{
		m_loadOverlay->hide();
		m_started = false;
		ResourceGroupManager::getSingletonPtr()->removeResourceGroupListener( this );
	}
}

void EMuseLoadingBar :: start( unsigned short p_numGroupsInit, unsigned short p_numGroupsLoad, Real p_initProportion )
{
	if ( p_numGroupsInit == 0 )
	{
		return;
	}

	if ( m_window == NULL )
	{
		return;
	}

	if ( m_started )
	{
		return;
	}

	m_numGroupsInit = p_numGroupsInit;
	m_numGroupsLoad = p_numGroupsLoad;
	m_initProportion = p_initProportion;
	OverlayManager * l_overMan = OverlayManager::getSingletonPtr();
	m_loadOverlay = static_cast <Overlay *>( l_overMan->getByName( m_loadingOverlayName ) );

	if ( m_loadOverlay == NULL )
	{
		return;
	}

	m_loadOverlay->show();
	m_loadingBarElement = l_overMan->getOverlayElement( m_loadingBarProgressName );
	m_loadingPercentElement = l_overMan->getOverlayElement( m_loadingBarPercentageName );
	OverlayElement * l_bar = l_overMan->getOverlayElement( m_loadingBarName );
	m_progressBarMaxSize = l_bar->getWidth() - m_loadingBarOffset;
	// add the listener
	ResourceGroupManager::getSingletonPtr()->addResourceGroupListener( this );
	m_loadingBarElement->setWidth( 0.0 );
	m_previousWidth = 0.0;
	m_started = true;
	_updateWindow();
}

void EMuseLoadingBar :: _updateWindow()
{
	EMuseController::GetSingletonPtr()->GetOwner()->GetPlugin()->LockGui();
	m_window->update();
	EMuseController::GetSingletonPtr()->GetOwner()->GetPlugin()->UnlockGui();
}

void EMuseLoadingBar :: UpdateBar()
{
	m_loadingBarElement->setWidth( m_loadingBarElement->getWidth() + m_progressBarInc );
	int percent = Math::IFloor( 100.0f * float( m_loadingBarElement->getWidth() ) / m_progressBarMaxSize );
	m_loadingPercentElement->setCaption( StringConverter::toString( percent ) + " %" );
	_updateWindow();
}

void EMuseLoadingBar :: resourceGroupScriptingStarted( const String & p_name , size_t scriptCount )
{
	m_progressBarInc = m_progressBarMaxSize * m_initProportion / static_cast <Real>( scriptCount );
	m_progressBarInc /= m_numGroupsInit;
	_updateWindow();
}

void EMuseLoadingBar :: scriptParseEnded( const String & p_name )
{
	UpdateBar();
}

void EMuseLoadingBar :: resourceGroupLoadStarted( const String & p_name, size_t resourceCount )
{
	m_progressBarInc = m_progressBarMaxSize * ( 1 - m_initProportion ) / static_cast <Real>( resourceCount );
	m_progressBarInc /= m_numGroupsLoad;
	_updateWindow();
}


void EMuseLoadingBar :: resourceLoadEnded()
{
	UpdateBar();
}

void EMuseLoadingBar :: resourceDownloadStarted( const String & p_name )
{
	m_dlres = p_name;
}

void EMuseLoadingBar :: setPercent( float p_percent, float p_downSpeed )
{
	float l_width = float( Math::IFloor( m_progressBarMaxSize * p_percent ) );
	String l_tmp = m_dlres + " - ";

	if ( p_percent < 1.0 )
	{
		l_tmp += StringConverter::toString( Math::IFloor( p_percent * 100.0f ) );
	}
	else
	{
		l_tmp += "100";
	}

	l_tmp += "%, " + StringConverter::toString( Math::IFloor( p_downSpeed ) ) + "Ko/s.";
	m_loadingPercentElement->setCaption( l_tmp );
	m_loadingBarElement->setWidth( l_width );
	_updateWindow();
}

void EMuseLoadingBar :: StartDownloadOfFile()
{
	m_startByDownload = ! m_started;

	if ( m_startByDownload )
	{
		start( 1, 0, Real( 0.1 ) );
	}
	else
	{
		m_previousWidth = m_loadingBarElement->getWidth();
		m_loadingPercentElement->setCaption( m_dlres + " - 0%" );
		m_loadingBarElement->setWidth( 0 );
		_updateWindow();
	}
}

void EMuseLoadingBar :: EndDownloadOfFile()
{
	if ( m_startByDownload )
	{
		finish();
	}
	else
	{
		m_loadingBarElement->setWidth( m_previousWidth );
		_updateWindow();
	}
}
