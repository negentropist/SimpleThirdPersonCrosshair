class S3C_CrosshairInfoDisplay  : SCR_InfoDisplayExtended {
	
	private BaseWeaponManagerComponent m_WeaponManager;
	private CharacterControllerComponent m_CharController;
	private CameraHandlerComponent m_CameraHandlerComponent;
	private SCR_ChimeraCharacter m_Character;
	private BaseContainer m_S3C_GameplaySettings;
	
	
	private ref WorkspaceWidget m_Workspace;
	private ref ImageWidget m_Crosshair;
	
	//------------------------------------------------------------------------------------------------
	
	private float Raycast(vector start, vector end, World world, out TraceParam param = null, array<IEntity> excludeArray = null)
	{
	    param = new TraceParam();
	
	    param.ExcludeArray = excludeArray; 
	    param.Flags = TraceFlags.ENTS | TraceFlags.WORLD;
	    param.LayerMask = EPhysicsLayerDefs.Projectile;
	    param.Start = start;
	    param.End = end;
	
	    return world.TraceMove(param, null);
	}
	
	void SetCrosshairOpacity(float opacity)
	{
		m_Crosshair.SetOpacity(opacity);	
	}
	
	//------------------------------------------------------------------------------------------------
	
	override void DisplayInit(IEntity owner)
	{
		m_Character = SCR_ChimeraCharacter.Cast(owner);
		m_CharController = m_Character.GetCharacterController();
		m_CameraHandlerComponent = m_CharController.GetCameraHandlerComponent();
		m_WeaponManager = BaseWeaponManagerComponent.Cast(m_Character.FindComponent(BaseWeaponManagerComponent));
		m_S3C_GameplaySettings = GetGame().GetGameUserSettings().GetModule("S3C_GameplaySettings");
		
		return;
	}

	override void DisplayStartDraw(IEntity owner)
	{
		if (!m_wRoot)
			return;
		
		m_Crosshair = ImageWidget.Cast(m_wRoot.FindAnyWidget("m_Crosshair"));
		m_Workspace = m_Crosshair.GetWorkspace();
		
		float opacity;
		m_S3C_GameplaySettings.Get("m_fCrosshairOpacity", opacity);
		SetCrosshairOpacity(opacity);
	}
	
	
	//------------------------------------------------------------------------------------------------
	
	override void DisplayUpdate(IEntity owner, float timeSlice)
	{       
		m_Crosshair.SetVisible(false);
		
		if (m_wRoot && m_Character && m_WeaponManager && m_CharController)
		{
			
			bool use3PPAds;
			m_S3C_GameplaySettings.Get("m_bUse3PPAds", use3PPAds);

			
			if (!use3PPAds && m_CharController.GetWeaponADSInput())
				return;

			if (m_CameraHandlerComponent.IsInThirdPerson() && m_CharController.IsWeaponRaised() && !m_CharController.IsPartiallyLowered() && !m_CharController.IsReloading())
			{
				vector muzzleTransform[4];
			    if (m_WeaponManager.GetCurrentMuzzleTransform(muzzleTransform)) 
				{
					BaseWorld world = GetGame().GetWorld();

				    if (m_Crosshair)
					{
						BaseWeaponComponent weaponComp = m_WeaponManager.GetCurrentWeapon();
						IEntity weapon = weaponComp.GetOwner();
						
						// Creates the raycast exclude list and add the weapon ent to it
						array<IEntity> excludedEntities = {weapon};
						
						// Add all weapon attachments to the raycast exclude list
						for (IEntity child = weapon.GetChildren(); child != null; null) 
						{
							if (child)
							{
								excludedEntities.Insert(child);
								child = child.GetSibling();
							}
						}
						
						//Raycast
						vector muzzleFwd = muzzleTransform[2];
						vector muzzleStart = muzzleTransform[3];
						vector muzzleEnd =  muzzleStart + (muzzleFwd * 100);
						
						TraceParam bulletTrajectoryParams;
						float bulletTrajectoryPercent = Raycast(muzzleStart, muzzleEnd, world, bulletTrajectoryParams, excludedEntities);
						float bulletTrajectoryDistance = 100*bulletTrajectoryPercent;

						vector bulletHitPos = muzzleStart + (muzzleFwd * bulletTrajectoryDistance);
					    vector bulletHitScreenPos = m_Workspace.ProjWorldToScreen(bulletHitPos, world, -1);
						
						//Set crosshair color to red if too close from player
						if (bulletTrajectoryPercent < 0.015) { m_Crosshair.SetColor(Color.Red) } else { m_Crosshair.SetColor(Color.White) };
						
						
						//Size based on distance
						float lerp;
						float progress = bulletTrajectoryPercent;
						float min = 0.0;
						float max = 0.1;
						float range = max-min;
						
						if (progress <= min) lerp = 1;
						else if (progress >= max) lerp = 0;
						else lerp = (max - progress) / range;
						
						lerp = 1-lerp;

						float size = 22.5 - (12.5*lerp);
						FrameSlot.SetSize(m_Crosshair, size, size);
						
						
						//Check if the player camera can see the crosshair 
						vector camPos[4];
						world.GetCurrentCamera(camPos);
						
						float crosshairVisibilityPercent = Raycast(camPos[3], bulletHitPos, world);
						if (crosshairVisibilityPercent > 0.9)
						{
							m_Crosshair.SetVisible(true);
					        FrameSlot.SetPos(m_Crosshair, bulletHitScreenPos[0], bulletHitScreenPos[1]);
						}
					}
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
}