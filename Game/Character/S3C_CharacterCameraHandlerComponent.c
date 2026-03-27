modded class SCR_CharacterCameraHandlerComponent : CameraHandlerComponent
{
	
	private bool m_S3C_bUse3PPAds;

	override void Init()
	{
		super.Init();
		BaseContainer S3C_gameplaySettings = GetGame().GetGameUserSettings().GetModule("S3C_GameplaySettings");
		S3C_gameplaySettings.Get("m_bUse3PPAds", m_S3C_bUse3PPAds);
	}
	
	
	override void OnCameraSwitchPressed()
	{
		if (m_S3C_bUse3PPAds)
		{
			bool S3C_isADS = m_ControllerComponent.IsWeaponADS();
			super.OnCameraSwitchPressed();
			if (S3C_isADS) // S3C: OnCameraSwitchPressed() automatically turns off ADS when calling SetThirdPerson(false)
			{
				m_ControllerComponent.SetWeaponADS(true);
			}
		}
		else
		{
			super.OnCameraSwitchPressed();
		}
		
	}
	
	void S3C_SetUse3PPAds(bool val)
	{
		m_S3C_bUse3PPAds = val;
	}
	
	
	
	//------------------------------------------------------------------------------------------------
	override int CameraSelector()
	{
		m_AnimationComponent.GetMovementState(m_CharMovementState);
		
		#ifdef ENABLE_DIAG 
		if (IsDebugView())
		{
			return CharacterCameraSet.CHARACTERCAMERA_DEBUG;
		}
		#endif

		if (!IsInThirdPerson())
		{
			ECharacterLifeState lifeState = m_ControllerComponent.GetLifeState();
			if (lifeState == ECharacterLifeState.DEAD)
				return CharacterCameraSet.CHARACTERCAMERA_1ST_BONE_TRANSFORM;
		
			if (lifeState == ECharacterLifeState.INCAPACITATED)
				return CharacterCameraSet.CHARACTERCAMERA_1ST_UNCONSCIOUS;
		}
		else
		{
			if (m_ControllerComponent.IsUnconscious())
				return CharacterCameraSet.CHARACTERCAMERA_3RD_UNCONSCIOUS;
		}
		
		if (m_ControllerComponent.IsSwimming())
		{
			if (IsInThirdPerson())
				return CharacterCameraSet.CHARACTERCAMERA_3RD_FLOAT;
			else
				return CharacterCameraSet.CHARACTERCAMERA_1ST_FLOAT;
		}

		if (SCR_BinocularsComponent.IsZoomedView())
			return CharacterCameraSet.CHARACTERCAMERA_BINOCULARS;
		
		//! game camera selection
		if (m_ControllerComponent.IsWeaponADS() || (m_ControllerComponent.GetInputContext().IsWeaponADS() && m_ControllerComponent.IsChangingStance()))
		
			if (!m_S3C_bUse3PPAds)
				return CharacterCameraSet.CHARACTERCAMERA_ADS;
			else
				if (!IsInThirdPerson())
					return CharacterCameraSet.CHARACTERCAMERA_ADS;
		
		
		if (m_ControllerComponent.IsGadgetRaisedModeWanted() && !m_OwnerCharacter.IsInVehicle())
			return CharacterCameraSet.CHARACTERCAMERA_1ST;

		CompartmentAccessComponent compartmentAccess = m_OwnerCharacter.GetCompartmentAccessComponent();
		
		if (m_ControllerComponent.GetInspect() && !m_OwnerCharacter.IsInVehicle())
			return CharacterCameraSet.CHARACTERCAMERA_1ST;
		
		bool isRolling = false;
		if (m_ControllerComponent.IsRoll() && m_CmdHandler)
		{
			CharacterCommandMove moveCmd = m_CmdHandler.GetCommandMove();
			//! we need to stabilize camera before roll action is complete, so by that time player is ready to aim at target
			if (moveCmd && !moveCmd.IsBlendingOutRoll())
				isRolling = true;
		}
		
		if (IsInThirdPerson())
		{
			if( m_OwnerCharacter.IsInVehicle() && compartmentAccess && !compartmentAccess.IsGettingOut() )
			{
				bool isTurretAds = false;
				bool inTurret = CheckIsInTurret(isTurretAds);
				if (inTurret)
				{
					if (isTurretAds)
						return CharacterCameraSet.CHARACTERCAMERA_ADS_VEHICLE;

					return CharacterCameraSet.CHARACTERCAMERA_3RD_TURRET;
				}
				
				if (m_ControllerComponent.GetInspect())
					return CharacterCameraSet.CHARACTERCAMERA_1ST_VEHICLE;

				return CharacterCameraSet.CHARACTERCAMERA_3RD_VEHICLE;
			}
			
			if (compartmentAccess && (compartmentAccess.IsGettingOut() || compartmentAccess.IsGettingIn()))
				return CharacterCameraSet.CHARACTERCAMERA_3RD_ERC;

			if( m_CharMovementState.m_CommandTypeId == ECharacterCommandIDs.CLIMB )
				return CharacterCameraSet.CHARACTERCAMERA_3RD_CLIMB;
			
			if (m_ControllerComponent.GetScrInputContext().m_iLoiteringType == 1)
			{
				SCR_CharacterCommandHandlerComponent scrCmdHandler = SCR_CharacterCommandHandlerComponent.Cast(m_CmdHandler);
				if (scrCmdHandler && scrCmdHandler.IsLoitering())
					return CharacterCameraSet.CHARACTERCAMERA_3RD_SITTING;
			}
			
			if( m_CharMovementState.m_iStanceIdx == ECharacterStance.PRONE )
			{
				if (ShouldForceFirstPersonInThirdPerson(m_ControllerComponent))
				{
					if (isRolling)
						return CharacterCameraSet.CHARACTERCAMERA_1ST_BONE_TRANSFORM;
					
					return CharacterCameraSet.CHARACTERCAMERA_1ST;
				}
				
				return CharacterCameraSet.CHARACTERCAMERA_3RD_PRO;
			}
			else if( m_CharMovementState.m_iStanceIdx == ECharacterStance.CROUCH )
			{
				if (ShouldForceFirstPersonInThirdPerson(m_ControllerComponent))
					return CharacterCameraSet.CHARACTERCAMERA_1ST;
				
				return CharacterCameraSet.CHARACTERCAMERA_3RD_CRO;
			}
			
			if (ShouldForceFirstPersonInThirdPerson(m_ControllerComponent))
				return CharacterCameraSet.CHARACTERCAMERA_1ST;
			
			return CharacterCameraSet.CHARACTERCAMERA_3RD_ERC;
		}
		else if (compartmentAccess && (compartmentAccess.IsGettingIn() || compartmentAccess.IsGettingOut()))
		{
			return CharacterCameraSet.CHARACTERCAMERA_1ST_VEHICLE_TRANSITION;
		}
		else if( m_OwnerCharacter.IsInVehicle() )
		{
			bool isTurretAds = false;
			bool inTurret = CheckIsInTurret(isTurretAds);
			if (inTurret)
			{
				if (isTurretAds)
					return CharacterCameraSet.CHARACTERCAMERA_ADS_VEHICLE;

				return CharacterCameraSet.CHARACTERCAMERA_1ST_TURRET;
			}
			else if( CheckVehicleADS() )
			{
				return CharacterCameraSet.CHARACTERCAMERA_ADS_VEHICLE;
			}

			return CharacterCameraSet.CHARACTERCAMERA_1ST_VEHICLE;
		}
		//! Setup here all conditions where we want to use camera transformation from bone directly
		//! Roll case
		else if (isRolling)
		{
			return CharacterCameraSet.CHARACTERCAMERA_1ST_BONE_TRANSFORM;
		}

		return CharacterCameraSet.CHARACTERCAMERA_1ST;
	}
	
	override protected bool ShouldForceFirstPersonInThirdPerson(CharacterControllerComponent controller)
	{
		if (!m_ControllerComponent.GetWeaponADSInput())
			return false;

		if (m_ControllerComponent.IsSprinting() || m_ControllerComponent.IsSwimming())
			return false;
		
		if (m_S3C_bUse3PPAds)
			if (m_ControllerComponent.GetWeaponADSInput() && IsInThirdPerson()) // S3C: Do not force 1PP if in 3PP ADS
				return false;
		
		return true;
	}
}
