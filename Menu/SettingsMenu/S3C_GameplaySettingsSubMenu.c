modded class SCR_GameplaySettingsSubMenu : SCR_SettingsSubMenuBase
{
	void S3C_OnCrosshairOpacityChanged(SCR_WLibComponentBase comp, float newOpacity)
	{
		S3C_CrosshairInfoDisplay crosshairInfoDisplay = S3C_CrosshairInfoDisplay.Cast(GetGame().GetHUDManager().FindInfoDisplay(S3C_CrosshairInfoDisplay));
		if (!crosshairInfoDisplay) return;

		crosshairInfoDisplay.SetCrosshairOpacity(newOpacity);
	}
	
	void S3C_On3PPAimingChanged(SCR_SpinBoxComponent spinBox, int newState)
	{
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());
		if (!character) return;
		
		SCR_CharacterCameraHandlerComponent cameraHandlerComponent = SCR_CharacterCameraHandlerComponent.Cast(character.FindComponent(SCR_CharacterCameraHandlerComponent));
		if (!cameraHandlerComponent) return;
		
		cameraHandlerComponent.S3C_SetUse3PPAds(newState);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnTabCreate(Widget menuRoot, ResourceName buttonsLayout, int index)
	{
		super.OnTabCreate(menuRoot, buttonsLayout, index);
		
		//Crosshair Opacity
		m_aSettingsBindings.Insert(new SCR_SettingBindingGameplay("S3C_GameplaySettings", "m_fCrosshairOpacity", "S3C_CrosshairOpacity"));
		SCR_SliderComponent S3C_opacitySliderComponent = SCR_SliderComponent.GetSliderComponent("S3C_CrosshairOpacity", m_wRoot, true);
		if (S3C_opacitySliderComponent)
			S3C_opacitySliderComponent.GetOnChangedFinal().Insert(S3C_OnCrosshairOpacityChanged);
		
		//Toggle 3PP Ads
		m_aSettingsBindings.Insert(new SCR_SettingBindingGameplay("S3C_GameplaySettings", "m_bUse3PPAds", "S3C_Use3PPAds"));
		SCR_SpinBoxComponent S3C_use3PPAdsSpinboxComponent = SCR_SpinBoxComponent.GetSpinBoxComponent("S3C_Use3PPAds", m_wRoot, true);
		if (S3C_use3PPAdsSpinboxComponent)
			S3C_use3PPAdsSpinboxComponent.m_OnChanged.Insert(S3C_On3PPAimingChanged);
		
		LoadSettings();
	}
}
