#include "global.h"

#include "PercentageDisplay.h"
#include "GameState.h"
#include "ThemeManager.h"
#include "PrefsManager.h"

#include "RageLog.h"

const float PERCENT_X[NUM_PLAYERS]	=	{ +20, -20 };
const float PERCENT_Y				=	0;

const int NUM_DANCE_POINT_DIGITS	=	5;

PercentageDisplay::PercentageDisplay()
{
	this->AddChild( &m_textPercent );
}

void PercentageDisplay::Load( PlayerNumber pn )
{
	m_PlayerNumber = pn;
	m_Last = 0;

	m_textPercent.LoadFromNumbers( THEME->GetPathToN(m_sName + " percent") );
	m_textPercent.EnableShadow( false );
	m_textPercent.SetZoom( 0.7f );
	m_textPercent.SetXY( PERCENT_X[pn], PERCENT_Y );
	m_textPercent.SetDiffuse( PlayerToColor(pn) );	// light blue

	if( PREFSMAN->m_bDancePointsForOni )
		m_textPercent.SetText( "     " );
	else
		m_textPercent.SetText( "00.0%" );
}

void PercentageDisplay::Update( float fDeltaTime )
{
	const int iActualDancePoints = GAMESTATE->m_CurStageStats.iActualDancePoints[m_PlayerNumber];
	if( iActualDancePoints == m_Last )
		return;
	m_Last = iActualDancePoints;

	CString sNumToDisplay;
	if( !PREFSMAN->m_bDancePointsForOni )
	{
		int iPossibleDancePoints = GAMESTATE->m_CurStageStats.iPossibleDancePoints[m_PlayerNumber];
		iPossibleDancePoints = max( 1, iPossibleDancePoints );
		float fPercentDancePoints =  iActualDancePoints / (float)iPossibleDancePoints + 0.00001f;	// correct for rounding errors

	//	LOG->Trace( "Actual %d, Possible %d, Percent %f\n", iActualDancePoints, iPossibleDancePoints, fPercentDancePoints );

		float fNumToDisplay = max( 0, fPercentDancePoints*100 );
		sNumToDisplay = ssprintf("%03.1f%%", fNumToDisplay);
		if( sNumToDisplay.GetLength() == 4 )
			sNumToDisplay = "0" + sNumToDisplay;
	}
	else
		sNumToDisplay = ssprintf( "%*d", NUM_DANCE_POINT_DIGITS , iActualDancePoints );

	m_textPercent.SetText( sNumToDisplay );
}

/*
 * Copyright (c) 2001-2003 by the person(s) listed below.  All rights reserved.
 *  Chris Danford
 */
