/************************************************************************************

Filename    :   MoviePlayerView.h
Content     :
Created     :	6/17/2014
Authors     :   Jim Dosé

Copyright   :   Copyright (c) Facebook Technologies, LLC and its affiliates. All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the Cinema/ directory. An additional grant 
of patent rights can be found in the PATENTS file in the same directory.

*************************************************************************************/

#if !defined( MoviePlayerView_h )
#define MoviePlayerView_h

#include "GazeCursor.h"
#include "GuiSys.h"
#include "Lerp.h"
#include "UI/UITexture.h"
#include "UI/UIMenu.h"
#include "UI/UIContainer.h"
#include "UI/UILabel.h"
#include "UI/UIImage.h"
#include "UI/UIButton.h"

using namespace OVR;

namespace OculusCinema {

enum ePlaybackControlsEvent
{
	UI_NO_EVENT = 0,
	UI_RW_PRESSED = 1,
	UI_PLAY_PRESSED = 2,
	UI_FF_PRESSED = 3,
	UI_CAROUSEL_PRESSED = 4,
	UI_CLOSE_UI_PRESSED = 5,
	UI_USER_TIMEOUT = 6,
	UI_SEEK_PRESSED = 7
};

class CinemaApp;

class ControlsGazeTimer : public VRMenuComponent
{
public:
	static const int 		TYPE_ID = 152413;

							ControlsGazeTimer();

	virtual int				GetTypeId() const { return TYPE_ID; }

	void					SetGazeTime();
	double					GetLastGazeTime() const { return LastGazeTime; }

	bool					IsFocused() const { return HasFocus; }

private:
	double					LastGazeTime;
	bool					HasFocus;

private:
	virtual eMsgStatus		OnEvent_Impl( OvrGuiSys & guiSys, ovrFrameInput const & vrFrame,
									VRMenuObject * self, VRMenuEvent const & event );
};

class ScrubBarComponent : public VRMenuComponent
{
public:
	static const int 		TYPE_ID = 152414;

							ScrubBarComponent();

	virtual int				GetTypeId() const { return TYPE_ID; }

	void					SetDuration( const int duration );
	void					SetOnClick( void ( *callback )( ScrubBarComponent *, void *, float ), void *object );
	void					SetWidgets( UIObject *background, UIObject *scrubBar, UILabel *currentTime, UILabel *seekTime, const int scrubBarWidth );
	void 					SetProgress( const float progress );

	bool					IsScrubbing() const { return TouchDown; }

private:
	bool					HasFocus;
	bool					TouchDown;

	float					Progress;
	int						Duration;

	UIObject *				Background;
	UIObject *				ScrubBar;
	UILabel *				CurrentTime;
	UILabel *				SeekTime;
	int 					ScrubBarWidth;

	void 					( *OnClickFunction )( ScrubBarComponent *button, void *object, float progress );
	void *					OnClickObject;

private:
	virtual eMsgStatus      OnEvent_Impl( OvrGuiSys & guiSys, ovrFrameInput const & vrFrame,
                                    VRMenuObject * self, VRMenuEvent const & event );

	eMsgStatus 				OnFrame( OvrGuiSys & guiSys, ovrFrameInput const & vrFrame,
									VRMenuObject * self, VRMenuEvent const & event );

	void 					OnClick( OvrGuiSys & guiSys, ovrFrameInput const & vrFrame, VRMenuEvent const & event );

	void 					SetTimeText( UILabel *label, const int time );
};

class MoviePlayerView : public View
{
public:
							MoviePlayerView( CinemaApp &app_ );
	virtual 				~MoviePlayerView();

	virtual void 			OneTimeInit( const char * launchIntent );
	virtual void			OneTimeShutdown();

	virtual void 			OnOpen();
	virtual void 			OnClose();

	virtual void			EnteredVrMode();
	virtual void 			LeavingVrMode();

	virtual bool 			OnKeyEvent( const int keyCode, const int repeatCount, const KeyEventType eventType );
	virtual void 			Frame( const ovrFrameInput & vrFrame );

	void 					MovieLoaded( const int width, const int height, const int duration );

private:
	CinemaApp &				Cinema;

	bool					uiActive;

	bool					RepositionScreen;

	static const int		MaxSeekSpeed;
	static const int 		ScrubBarWidth;

	static const double 	GazeTimeTimeout;

	int						SeekSpeed;
	int						PlaybackPos;
	double					NextSeekTime;

	UITexture				BackgroundTintTexture;

	UITexture				RWTexture;
	UITexture				RWHoverTexture;
	UITexture				RWPressedTexture;

	UITexture				FFTexture;
	UITexture				FFHoverTexture;
	UITexture				FFPressedTexture;

	UITexture				PlayTexture;
	UITexture				PlayHoverTexture;
	UITexture				PlayPressedTexture;

	UITexture				PauseTexture;
	UITexture				PauseHoverTexture;
	UITexture				PausePressedTexture;

	UITexture				CarouselTexture;
	UITexture				CarouselHoverTexture;
	UITexture				CarouselPressedTexture;

	UITexture				SeekbarBackgroundTexture;
	UITexture				SeekbarProgressTexture;

	UITexture				SeekPosition;

	UITexture				SeekFF2x;
	UITexture				SeekFF4x;
	UITexture				SeekFF8x;
	UITexture				SeekFF16x;
	UITexture				SeekFF32x;

	UITexture				SeekRW2x;
	UITexture				SeekRW4x;
	UITexture				SeekRW8x;
	UITexture				SeekRW16x;
	UITexture				SeekRW32x;

	UIMenu *				MoveScreenMenu;
	UILabel 				MoveScreenLabel;
	Lerp					MoveScreenAlpha;

	UIMenu *				PlaybackControlsMenu;
	UIContainer 			PlaybackControlsPosition;
	UIContainer 			PlaybackControlsScale;
	UILabel 				MovieTitleLabel;

	UIImage					SeekIcon;

	UIImage					ControlsBackground;
	ControlsGazeTimer		GazeTimer;

	UIButton				RewindButton;
	UIButton				PlayButton;
	UIButton				FastForwardButton;
	UIButton				CarouselButton;

	UIImage					SeekbarBackground;
	UIImage					SeekbarProgress;
	ScrubBarComponent 		ScrubBar;

	UILabel 				CurrentTime;
	UILabel 				SeekTime;

	bool					BackgroundClicked;
	bool					UIOpened;							// Used to ignore button A or touchpad until release so we don't close the UI immediately after opening it


private:
	void 					CreateMenu( OvrGuiSys & guiSys );
	void					SetSeekIcon( const int seekSpeed );

	void					BackPressed();

	void					TogglePlayback();
	void 					PauseMovie();
	void 					PlayMovie();

	friend void 			PlayPressedCallback( UIButton *button, void *object );
	void					RewindPressed();
	friend void 			RewindPressedCallback( UIButton *button, void *object );
	void					FastForwardPressed();
	friend void 			FastForwardPressedCallback( UIButton *button, void *object );
	void					CarouselPressed();
	friend void 			CarouselPressedCallback( UIButton *button, void *object );
	void					ScrubBarClicked( const float progress );
	friend void				ScrubBarCallback( ScrubBarComponent *, void *, float );

	Vector2f 				GazeCoordinatesOnScreen( const Matrix4f & viewMatrix, const Matrix4f panelMatrix ) const;

	void 					UpdateUI( const ovrFrameInput & vrFrame );
	void 					CheckInput( const ovrFrameInput & vrFrame );
	void 					CheckDebugControls( const ovrFrameInput & vrFrame );

	void 					ShowUI();
	void 					HideUI();
};

} // namespace OculusCinema

#endif // MoviePlayerView_h
