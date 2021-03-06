/*
See LICENSE file in root folder
*/
#ifndef ___Troll_PonctualEvent___
#define ___Troll_PonctualEvent___

#include "TemporalObject.h"

#include "GUI/TimeSequence/Module_Time.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TrollPonctualEvent
				: public TemporalObject
			{
			public:
				TrollPonctualEvent( TrollSequence * p_sequence, BasePonctualEvent * p_event, wxString const & p_targetTypeName, wxString const & p_targetName, float p_fireTime, wxString const & p_eventAction, wxString const & p_name, wxString const & p_fileName );
				~TrollPonctualEvent();

				void Write( wxTextOutputStream & p_stream );

				void SetFireTime( float p_time );
				void SetActionName( wxString const & p_name );
				void SetTargetType( wxString const & p_type );
				void SetTarget( wxString const & p_target );
				void SetActionArgs( wxString const & p_args );

				inline float GetFireTime()const
				{
					return m_fireTime;
				}
				inline wxString GetTargetType()const
				{
					return m_targetTypeName;
				}
				inline wxString GetTarget()const
				{
					return m_targetName;
				}
				inline BasePonctualEvent * GetMuseEvent()const
				{
					return m_museEvent;
				}
				inline wxString GetActionArgs()const
				{
					return m_actionArguments;
				}
				inline TrollSequence * GetSequence()const
				{
					return m_sequence;
				}
				inline GUI::Time::PonctualEventPanel * GetEventPanel()const
				{
					return m_panel;
				}

				inline void SetPanel( GUI::Time::PonctualEventPanel * p_panel )
				{
					m_panel = p_panel;
				}
				inline void	SetMuseEvent( BasePonctualEvent * p_event )
				{
					m_museEvent = p_event;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollSequence * m_sequence;
				BasePonctualEvent * m_museEvent;
				float m_fireTime;
				wxString m_targetTypeName;
				wxString m_targetName;
				wxString m_actionArguments;
				GUI::Time::PonctualEventPanel * m_panel;
			};
		}
	}
}

#endif
