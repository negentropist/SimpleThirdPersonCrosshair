// For test purpose only, deletes the CameraBase entity on Empty Arland which prevents from seeing the player camera

[EntityEditorProps(category: "GameScripted/Misc", description: "")]
class DeleteCameraBaseEntityClass : GenericEntityClass
{
}

class DeleteCameraBaseEntity : GenericEntity
{
	//------------------------------------------------------------------------------------------------
	void DeleteCamera()
	{
		IEntity cam = GetGame().GetWorld().FindEntityByName("CameraBase1");
		if (cam)
		{
			RplComponent.DeleteRplEntity(cam, false);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		SCR_BaseGameMode gm = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gm)
		{
			gm.GetOnGameStart().Insert(DeleteCamera);
		}
	}

	//------------------------------------------------------------------------------------------------
	void DeleteCameraBaseEntity(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
	}
}


