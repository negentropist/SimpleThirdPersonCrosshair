class S3C_GameplaySettings : ModuleGameSettings
{
	[Attribute(defvalue: "0.25", uiwidget: UIWidgets.Slider, params: "0 1 0.01", desc: "Changes the opacity of the 3PP crosshair")] // #locale
	float m_fCrosshairOpacity;
	
	[Attribute("1")]
	bool m_bUse3PPAds;
}