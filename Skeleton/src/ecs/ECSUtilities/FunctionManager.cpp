
#include "FunctionManager.h"
#include "Entity.h"
#include "ConsoleManager.h"

//Creation time: Sat Sep  2 15:33:53 2023

#define _Console(info, value) Console::Output::PrintError( info , value )
#define _ErrorInfo(entity, script, function, title) entity + ": " + script + ": " + function + ": " + title + ": "
#define _DebugError(entity, script, function, title, error) _Console(_ErrorInfo(entity, script, function, title), error)

#define _ComponentErrorMessage(entityError, missingComponent) "The entity <" + entityError + "> does not contain <" + missingComponent + "> component"
#define _NullErrorMessage(i, expected) "Empty input given to input(" + i + "). Expected type: <" + expected + ">"
#define _InvalidInputErrorMessage(i, expected, given) "Expected <" + expected + "> but received <" + given + "> for input(" + i + ")"

#define DebugComponentError(entity, script, function, entityError, missingComponent) _DebugError(entity, script, function, "Entity error", _ComponentErrorMessage(entityError, #missingComponent))
#define DebugNullError(entity, script, function, i, expected) _DebugError(entity, script, function, "Empty value", _NullErrorMessage(i, expected))
#define DebugInvalidInputError(entity, script, function, i, expected, given) _DebugError(entity, script, function, "Invalid input", _InvalidInputErrorMessage(i, expected, given))


#include <Components/Animation.h>
#include <Components/BoxBody.h>
#include <Components/CircleBody.h>
#include <Components/EdgeBody.h>
#include <Components/Image.h>
#include <Components/MusicEmitter.h>
#include <Components/Overlay.h>
#include <Components/OverlayButton.h>
#include <Components/OverlayImage.h>
#include <Components/OverlayText.h>
#include <Components/ParticleSystem.h>
#include <Components/PhysicBody.h>
#include <Components/PlatformController.h>
#include <Components/SoundEmitter.h>
#include <Components/TopDownController.h>
#include <Components/Transform.h>
#include <Save.h>
#include <SceneManager.h>
#include <ScriptFunctionality.h>
#include <InputManager.h>
#include <PhysicsManager.h>
#include <RendererManager.h>
#include <SoundManager.h>
#include <EngineTime.h>


using namespace ECS;
using namespace Physics;
using namespace Input;
using namespace Scripting;
using namespace Sound;
using namespace Renderer;


void FunctionManager::CreateFunctionMap(std::unordered_map<std::string, CallableFunction>& map){

	map.emplace("Animation_ChangeAnimationPath",Animation_ChangeAnimationPath);
	map.emplace("Animation_AdvanceFrame",Animation_AdvanceFrame);
	map.emplace("Animation_AdvanceAnimation",Animation_AdvanceAnimation);
	map.emplace("Animation_SetFrame",Animation_SetFrame);
	map.emplace("Animation_SetAnimation",Animation_SetAnimation);
	map.emplace("Animation_GetFrame",Animation_GetFrame);
	map.emplace("Animation_GetAnimation",Animation_GetAnimation);
	map.emplace("Animation_SetAnimationLenght",Animation_SetAnimationLenght);
	map.emplace("Animation_getAnimationLenght",Animation_getAnimationLenght);
	map.emplace("BoxBody_getSize",BoxBody_getSize);
	map.emplace("BoxBody_setSize",BoxBody_setSize);
	map.emplace("BoxBody_setMass",BoxBody_setMass);
	map.emplace("BoxBody_getArea",BoxBody_getArea);
	map.emplace("Image_getTextureWidth",Image_getTextureWidth);
	map.emplace("Image_getTextureHeight",Image_getTextureHeight);
	map.emplace("Image_setSrcRect",Image_setSrcRect);
	map.emplace("Image_setRotaionPoint",Image_setRotaionPoint);
	map.emplace("Image_scaledSize",Image_scaledSize);
	map.emplace("Image_loadTexture",Image_loadTexture);
	map.emplace("Image_setFlipMode",Image_setFlipMode);
	map.emplace("MusicEmitter_loadMusic",MusicEmitter_loadMusic);
	map.emplace("MusicEmitter_play",MusicEmitter_play);
	map.emplace("MusicEmitter_pause",MusicEmitter_pause);
	map.emplace("MusicEmitter_stop",MusicEmitter_stop);
	map.emplace("MusicEmitter_resume",MusicEmitter_resume);
	map.emplace("MusicEmitter_playWithFadeIn",MusicEmitter_playWithFadeIn);
	map.emplace("MusicEmitter_fadeOut",MusicEmitter_fadeOut);
	map.emplace("MusicEmitter_isPlaying",MusicEmitter_isPlaying);
	map.emplace("MusicEmitter_isPaused",MusicEmitter_isPaused);
	map.emplace("MusicEmitter_rewind",MusicEmitter_rewind);
	map.emplace("MusicEmitter_setVolume",MusicEmitter_setVolume);
	map.emplace("MusicEmitter_getVolume",MusicEmitter_getVolume);
	map.emplace("MusicEmitter_shouldPlayOnStart",MusicEmitter_shouldPlayOnStart);
	map.emplace("MusicEmitter_setLoop",MusicEmitter_setLoop);
	map.emplace("MusicEmitter_isOnLoop",MusicEmitter_isOnLoop);
	map.emplace("Overlay_GetPlacement",Overlay_GetPlacement);
	map.emplace("Overlay_SetPositioned",Overlay_SetPositioned);
	map.emplace("Overlay_SetStreched",Overlay_SetStreched);
	map.emplace("Overlay_Move",Overlay_Move);
	map.emplace("Overlay_MoveTo",Overlay_MoveTo);
	map.emplace("Overlay_SetSize",Overlay_SetSize);
	map.emplace("Overlay_GetPosition",Overlay_GetPosition);
	map.emplace("Overlay_GetSize",Overlay_GetSize);
	map.emplace("Overlay_GetTop",Overlay_GetTop);
	map.emplace("Overlay_GetLeft",Overlay_GetLeft);
	map.emplace("Overlay_GetRight",Overlay_GetRight);
	map.emplace("Overlay_GetBottom",Overlay_GetBottom);
	map.emplace("Overlay_SetTop",Overlay_SetTop);
	map.emplace("Overlay_SetLeft",Overlay_SetLeft);
	map.emplace("Overlay_SetRight",Overlay_SetRight);
	map.emplace("Overlay_SetBottom",Overlay_SetBottom);
	map.emplace("Overlay_GetAnchor",Overlay_GetAnchor);
	map.emplace("Overlay_SetAnchor",Overlay_SetAnchor);
	map.emplace("Overlay_SetAnchorCenter",Overlay_SetAnchorCenter);
	map.emplace("Overlay_SetAnchorTopLeft",Overlay_SetAnchorTopLeft);
	map.emplace("Overlay_SetAnchorTopRight",Overlay_SetAnchorTopRight);
	map.emplace("Overlay_SetAnchorBottomLeft",Overlay_SetAnchorBottomLeft);
	map.emplace("Overlay_SetAnchorBottomRight",Overlay_SetAnchorBottomRight);
	map.emplace("Overlay_GetColor",Overlay_GetColor);
	map.emplace("Overlay_SetColor",Overlay_SetColor);
	map.emplace("Overlay_GetRenderScale",Overlay_GetRenderScale);
	map.emplace("Overlay_SetRenderScale",Overlay_SetRenderScale);
	map.emplace("Overlay_ResetRenderScale",Overlay_ResetRenderScale);
	map.emplace("Overlay_SetInteractable",Overlay_SetInteractable);
	map.emplace("Overlay_IsInteractable",Overlay_IsInteractable);
	map.emplace("OverlayImage_SetTexture",OverlayImage_SetTexture);
	map.emplace("OverlayImage_GetTexture",OverlayImage_GetTexture);
	map.emplace("OverlayText_GetFit",OverlayText_GetFit);
	map.emplace("OverlayText_SetFit",OverlayText_SetFit);
	map.emplace("OverlayText_GetVerticalAlignment",OverlayText_GetVerticalAlignment);
	map.emplace("OverlayText_GetHorizontalAlignment",OverlayText_GetHorizontalAlignment);
	map.emplace("OverlayText_SetVerticalAlignment",OverlayText_SetVerticalAlignment);
	map.emplace("OverlayText_SetHorizontalAlignment",OverlayText_SetHorizontalAlignment);
	map.emplace("OverlayText_GetFont",OverlayText_GetFont);
	map.emplace("OverlayText_SetFont",OverlayText_SetFont);
	map.emplace("OverlayText_GetText",OverlayText_GetText);
	map.emplace("OverlayText_SetText",OverlayText_SetText);
	map.emplace("OverlayText_GetPointSize",OverlayText_GetPointSize);
	map.emplace("OverlayText_SetPointSize",OverlayText_SetPointSize);
	map.emplace("ParticleSystem_startEmitting",ParticleSystem_startEmitting);
	map.emplace("ParticleSystem_isEmitting",ParticleSystem_isEmitting);
	map.emplace("ParticleSystem_loadTexture",ParticleSystem_loadTexture);
	map.emplace("ParticleSystem_addBurst",ParticleSystem_addBurst);
	map.emplace("PhysicBody_setTrigger",PhysicBody_setTrigger);
	map.emplace("PhysicBody_isTrigger",PhysicBody_isTrigger);
	map.emplace("PhysicBody_setFriction",PhysicBody_setFriction);
	map.emplace("PhysicBody_getFriction",PhysicBody_getFriction);
	map.emplace("PhysicBody_setBounciness",PhysicBody_setBounciness);
	map.emplace("PhysicBody_getBounciness",PhysicBody_getBounciness);
	map.emplace("PhysicBody_getOffSet",PhysicBody_getOffSet);
	map.emplace("PhysicBody_addOffSet",PhysicBody_addOffSet);
	map.emplace("PhysicBody_setRotationFreezed",PhysicBody_setRotationFreezed);
	map.emplace("PhysicBody_isRotationFreezed",PhysicBody_isRotationFreezed);
	map.emplace("PhysicBody_setBodyType",PhysicBody_setBodyType);
	map.emplace("PhysicBody_getBodyType",PhysicBody_getBodyType);
	map.emplace("PhysicBody_setLinearDrag",PhysicBody_setLinearDrag);
	map.emplace("PhysicBody_getLinearDrag",PhysicBody_getLinearDrag);
	map.emplace("PhysicBody_setAngularDrag",PhysicBody_setAngularDrag);
	map.emplace("PhysicBody_getAngularDrag",PhysicBody_getAngularDrag);
	map.emplace("PhysicBody_setGravityScale",PhysicBody_setGravityScale);
	map.emplace("PhysicBody_getGravityScale",PhysicBody_getGravityScale);
	map.emplace("PhysicBody_getCollisionNormal",PhysicBody_getCollisionNormal);
	map.emplace("PhysicBody_getCollisionPoint",PhysicBody_getCollisionPoint);
	map.emplace("PhysicBody_setCollisionLayer",PhysicBody_setCollisionLayer);
	map.emplace("PhysicBody_getCollisionLayer",PhysicBody_getCollisionLayer);
	map.emplace("PhysicBody_setLinearVelocity",PhysicBody_setLinearVelocity);
	map.emplace("PhysicBody_getLinearVelocity",PhysicBody_getLinearVelocity);
	map.emplace("PhysicBody_setAngularVelocity",PhysicBody_setAngularVelocity);
	map.emplace("PhysicBody_getAngularVelocity",PhysicBody_getAngularVelocity);
	map.emplace("PhysicBody_applyForce",PhysicBody_applyForce);
	map.emplace("PhysicBody_applyForceToCenter",PhysicBody_applyForceToCenter);
	map.emplace("PhysicBody_applyTorque",PhysicBody_applyTorque);
	map.emplace("PhysicBody_applyLinearImpulse",PhysicBody_applyLinearImpulse);
	map.emplace("PhysicBody_applyLinearImpulseToCenter",PhysicBody_applyLinearImpulseToCenter);
	map.emplace("PhysicBody_applyAngularImpulse",PhysicBody_applyAngularImpulse);
	map.emplace("SoundEmitter_loadSound",SoundEmitter_loadSound);
	map.emplace("SoundEmitter_play",SoundEmitter_play);
	map.emplace("SoundEmitter_pause",SoundEmitter_pause);
	map.emplace("SoundEmitter_stop",SoundEmitter_stop);
	map.emplace("SoundEmitter_resume",SoundEmitter_resume);
	map.emplace("SoundEmitter_playWithfadeIn",SoundEmitter_playWithfadeIn);
	map.emplace("SoundEmitter_fadeOut",SoundEmitter_fadeOut);
	map.emplace("SoundEmitter_isPlaying",SoundEmitter_isPlaying);
	map.emplace("SoundEmitter_isPaused",SoundEmitter_isPaused);
	map.emplace("SoundEmitter_setVolume",SoundEmitter_setVolume);
	map.emplace("SoundEmitter_getVolume",SoundEmitter_getVolume);
	map.emplace("SoundEmitter_shouldPlayOnStart",SoundEmitter_shouldPlayOnStart);
	map.emplace("SoundEmitter_setLoop",SoundEmitter_setLoop);
	map.emplace("SoundEmitter_isOnLoop",SoundEmitter_isOnLoop);
	map.emplace("SoundEmitter_enableSpatialSound",SoundEmitter_enableSpatialSound);
	map.emplace("SoundEmitter_isSpatialSoundEnabled",SoundEmitter_isSpatialSoundEnabled);
	map.emplace("SoundEmitter_setPanning",SoundEmitter_setPanning);
	map.emplace("SoundEmitter_getPanning",SoundEmitter_getPanning);
	map.emplace("SoundEmitter_setAudibleDistance",SoundEmitter_setAudibleDistance);
	map.emplace("SoundEmitter_getAudibleDistance",SoundEmitter_getAudibleDistance);
	map.emplace("Transform_GetLocalPosition",Transform_GetLocalPosition);
	map.emplace("Transform_GetWorldPosition",Transform_GetWorldPosition);
	map.emplace("Transform_SetLocalPosition",Transform_SetLocalPosition);
	map.emplace("Transform_SetLocalPositionX",Transform_SetLocalPositionX);
	map.emplace("Transform_SetLocalPositionY",Transform_SetLocalPositionY);
	map.emplace("Transform_SetWorldPosition",Transform_SetWorldPosition);
	map.emplace("Transform_GetLocalScale",Transform_GetLocalScale);
	map.emplace("Transform_GetWorldScale",Transform_GetWorldScale);
	map.emplace("Transform_SetScale",Transform_SetScale);
	map.emplace("Transform_SetScaleX",Transform_SetScaleX);
	map.emplace("Transform_SetScaleY",Transform_SetScaleY);
	map.emplace("Transform_SetWorldScale",Transform_SetWorldScale);
	map.emplace("Transform_GetLocalRotation",Transform_GetLocalRotation);
	map.emplace("Transform_GetWorldRotation",Transform_GetWorldRotation);
	map.emplace("Transform_SetLocalRotation",Transform_SetLocalRotation);
	map.emplace("Transform_SetWorldRotation",Transform_SetWorldRotation);
	map.emplace("Transform_Translate",Transform_Translate);
	map.emplace("Transform_TranslateX",Transform_TranslateX);
	map.emplace("Transform_TranslateY",Transform_TranslateY);
	map.emplace("Transform_Rotate",Transform_Rotate);
	map.emplace("Transform_Scale",Transform_Scale);
	map.emplace("Transform_SetTransformRelativeToNewParent",Transform_SetTransformRelativeToNewParent);
	map.emplace("InputManager_AnyKeyPressed",InputManager_AnyKeyPressed);
	map.emplace("InputManager_AnyKeyReleased",InputManager_AnyKeyReleased);
	map.emplace("InputManager_IsLetterPressed",InputManager_IsLetterPressed);
	map.emplace("InputManager_IsLetterDown",InputManager_IsLetterDown);
	map.emplace("InputManager_IsLetterUp",InputManager_IsLetterUp);
	map.emplace("InputManager_IsLetterReleased",InputManager_IsLetterReleased);
	map.emplace("InputManager_IsNumberPressed",InputManager_IsNumberPressed);
	map.emplace("InputManager_IsNumberDown",InputManager_IsNumberDown);
	map.emplace("InputManager_IsNumberUp",InputManager_IsNumberUp);
	map.emplace("InputManager_IsNumberReleased",InputManager_IsNumberReleased);
	map.emplace("InputManager_IsSpecialKeyPressed",InputManager_IsSpecialKeyPressed);
	map.emplace("InputManager_IsSpecialKeyDown",InputManager_IsSpecialKeyDown);
	map.emplace("InputManager_IsSpecialKeyUp",InputManager_IsSpecialKeyUp);
	map.emplace("InputManager_IsSpecialKeyReleased",InputManager_IsSpecialKeyReleased);
	map.emplace("InputManager_HorizontalMovement",InputManager_HorizontalMovement);
	map.emplace("InputManager_VerticalMovement",InputManager_VerticalMovement);
	map.emplace("InputManager_Jump",InputManager_Jump);
	map.emplace("InputManager_Action",InputManager_Action);
	map.emplace("InputManager_HasMouseMoved",InputManager_HasMouseMoved);
	map.emplace("InputManager_HasMouseWheelMoved",InputManager_HasMouseWheelMoved);
	map.emplace("InputManager_IsMouseButtonDown",InputManager_IsMouseButtonDown);
	map.emplace("InputManager_IsMouseButtonUp",InputManager_IsMouseButtonUp);
	map.emplace("InputManager_IsMouseButtonPressed",InputManager_IsMouseButtonPressed);
	map.emplace("InputManager_IsMouseButtonReleased",InputManager_IsMouseButtonReleased);
	map.emplace("InputManager_GetMousePosition",InputManager_GetMousePosition);
	map.emplace("InputManager_GetMouseWheelScroll",InputManager_GetMouseWheelScroll);
	map.emplace("InputManager_ConnectedControllersCount",InputManager_ConnectedControllersCount);
	map.emplace("InputManager_AnyControllerButtonPressed",InputManager_AnyControllerButtonPressed);
	map.emplace("InputManager_AnyControllerButtonReleased",InputManager_AnyControllerButtonReleased);
	map.emplace("InputManager_AnyControllerAxisMotion",InputManager_AnyControllerAxisMotion);
	map.emplace("InputManager_AnyControllerConnected",InputManager_AnyControllerConnected);
	map.emplace("InputManager_AnyControllerDisconnected",InputManager_AnyControllerDisconnected);
	map.emplace("InputManager_IsControllerButtonPressedWithId",InputManager_IsControllerButtonPressedWithId);
	map.emplace("InputManager_IsControllerButtonDownWithId",InputManager_IsControllerButtonDownWithId);
	map.emplace("InputManager_IsControllerButtonReleasedWithId",InputManager_IsControllerButtonReleasedWithId);
	map.emplace("InputManager_GetLeftTriggerValueWithId",InputManager_GetLeftTriggerValueWithId);
	map.emplace("InputManager_GetRightTriggerValueWithId",InputManager_GetRightTriggerValueWithId);
	map.emplace("InputManager_IsLeftTriggerDownWithId",InputManager_IsLeftTriggerDownWithId);
	map.emplace("InputManager_IsRightTriggerDownWithId",InputManager_IsRightTriggerDownWithId);
	map.emplace("InputManager_GetLeftStickHorizontalValueWithId",InputManager_GetLeftStickHorizontalValueWithId);
	map.emplace("InputManager_GetLeftStickVerticalValueWithId",InputManager_GetLeftStickVerticalValueWithId);
	map.emplace("InputManager_GetRightStickHorizontalValueWithId",InputManager_GetRightStickHorizontalValueWithId);
	map.emplace("InputManager_GetRightStickVerticalValueWithId",InputManager_GetRightStickVerticalValueWithId);
	map.emplace("InputManager_HasLeftStickMovedWithId",InputManager_HasLeftStickMovedWithId);
	map.emplace("InputManager_HasRightStickMovedWithId",InputManager_HasRightStickMovedWithId);
	map.emplace("InputManager_IsControllerButtonPressed",InputManager_IsControllerButtonPressed);
	map.emplace("InputManager_IsControllerButtonDown",InputManager_IsControllerButtonDown);
	map.emplace("InputManager_IsControllerButtonReleased",InputManager_IsControllerButtonReleased);
	map.emplace("InputManager_GetLeftTriggerValue",InputManager_GetLeftTriggerValue);
	map.emplace("InputManager_GetRightTriggerValue",InputManager_GetRightTriggerValue);
	map.emplace("InputManager_IsLeftTriggerDown",InputManager_IsLeftTriggerDown);
	map.emplace("InputManager_IsRightTriggerDown",InputManager_IsRightTriggerDown);
	map.emplace("InputManager_GetLeftStickHorizontalValue",InputManager_GetLeftStickHorizontalValue);
	map.emplace("InputManager_GetLeftStickVerticalValue",InputManager_GetLeftStickVerticalValue);
	map.emplace("InputManager_GetRightStickHorizontalValue",InputManager_GetRightStickHorizontalValue);
	map.emplace("InputManager_GetRightStickVerticalValue",InputManager_GetRightStickVerticalValue);
	map.emplace("InputManager_HasLeftStickMoved",InputManager_HasLeftStickMoved);
	map.emplace("InputManager_HasRightStickMoved",InputManager_HasRightStickMoved);
	map.emplace("PhysicsManager_getGravity",PhysicsManager_getGravity);
	map.emplace("PhysicsManager_setGravity",PhysicsManager_setGravity);
	map.emplace("PhysicsManager_addCollisionLayer",PhysicsManager_addCollisionLayer);
	map.emplace("PhysicsManager_removeCollisionLayer",PhysicsManager_removeCollisionLayer);
	map.emplace("PhysicsManager_setCollisionBetweenLayers",PhysicsManager_setCollisionBetweenLayers);
	map.emplace("PhysicsManager_layersCollide",PhysicsManager_layersCollide);
	map.emplace("PhysicsManager_layersExists",PhysicsManager_layersExists);
	map.emplace("RendererManager_getWidth",RendererManager_getWidth);
	map.emplace("RendererManager_getHeight",RendererManager_getHeight);
	map.emplace("RendererManager_toggleFullScreen",RendererManager_toggleFullScreen);
	map.emplace("RendererManager_showCursor",RendererManager_showCursor);
	map.emplace("RendererManager_resizeWindow",RendererManager_resizeWindow);
	map.emplace("RendererManager_renameWindow",RendererManager_renameWindow);
	map.emplace("RendererManager_repositionWindow",RendererManager_repositionWindow);
	map.emplace("RendererManager_SetWindowIcon",RendererManager_SetWindowIcon);
	map.emplace("SaveManager_SaveAll",SaveManager_SaveAll);
	map.emplace("SaveManager_Save",SaveManager_Save);
	map.emplace("SaveManager_Load",SaveManager_Load);
	map.emplace("SaveManager_SetSlot",SaveManager_SetSlot);
	map.emplace("SaveManager_GetSlot",SaveManager_GetSlot);
	map.emplace("SaveManager_Set",SaveManager_Set);
	map.emplace("SaveManager_Get",SaveManager_Get);
	map.emplace("SaveManager_ClearSlot",SaveManager_ClearSlot);
	map.emplace("SaveManager_Exists",SaveManager_Exists);
	map.emplace("SaveManager_DeleteSave",SaveManager_DeleteSave);
	map.emplace("SceneManager_ChangeScene",SceneManager_ChangeScene);
	map.emplace("SceneManager_ResetScene",SceneManager_ResetScene);
	map.emplace("SceneManager_EndGame",SceneManager_EndGame);
	map.emplace("SceneManager_getNumberOfScenes",SceneManager_getNumberOfScenes);
	map.emplace("SceneManager_GetCurrentScenePath",SceneManager_GetCurrentScenePath);
	map.emplace("Print",ScriptFunctionality_Print);
	map.emplace("Entity",ScriptFunctionality_Entity);
	map.emplace("Create_EntityWithTransform",ScriptFunctionality_Create_EntityWithTransform);
	map.emplace("Create_EntityWithOverlay",ScriptFunctionality_Create_EntityWithOverlay);
	map.emplace("Destroy_Entity",ScriptFunctionality_Destroy_Entity);
	map.emplace("Entity_Name",ScriptFunctionality_Entity_Name);
	map.emplace("Entity_CurrentName",ScriptFunctionality_Entity_CurrentName);
	map.emplace("Event_EntityEvent",ScriptFunctionality_Event_EntityEvent);
	map.emplace("Event_GlobalEvent",ScriptFunctionality_Event_GlobalEvent);
	map.emplace("Graph",ScriptFunctionality_Graph);
	map.emplace("Math_Add",ScriptFunctionality_Math_Add);
	map.emplace("Math_Subtract",ScriptFunctionality_Math_Subtract);
	map.emplace("Math_Multiply",ScriptFunctionality_Math_Multiply);
	map.emplace("Math_Divide",ScriptFunctionality_Math_Divide);
	map.emplace("Math_Power",ScriptFunctionality_Math_Power);
	map.emplace("Math_Root",ScriptFunctionality_Math_Root);
	map.emplace("Math_Max",ScriptFunctionality_Math_Max);
	map.emplace("Math_Min",ScriptFunctionality_Math_Min);
	map.emplace("Math_PlusOne",ScriptFunctionality_Math_PlusOne);
	map.emplace("Math_MinusOne",ScriptFunctionality_Math_MinusOne);
	map.emplace("Logic_Equals",ScriptFunctionality_Logic_Equals);
	map.emplace("Logic_NotEquals",ScriptFunctionality_Logic_NotEquals);
	map.emplace("Logic_Lesser",ScriptFunctionality_Logic_Lesser);
	map.emplace("Logic_Greater",ScriptFunctionality_Logic_Greater);
	map.emplace("Logic_LesserOrEqual",ScriptFunctionality_Logic_LesserOrEqual);
	map.emplace("Logic_GreaterOrEqual",ScriptFunctionality_Logic_GreaterOrEqual);
	map.emplace("Logic_And",ScriptFunctionality_Logic_And);
	map.emplace("Logic_Or",ScriptFunctionality_Logic_Or);
	map.emplace("Logic_Negate",ScriptFunctionality_Logic_Negate);
	map.emplace("Vector2D_Magnitude",ScriptFunctionality_Vector2D_Magnitude);
	map.emplace("Vector2D_X",ScriptFunctionality_Vector2D_X);
	map.emplace("Vector2D_Y",ScriptFunctionality_Vector2D_Y);
	map.emplace("Vector2D_Angle",ScriptFunctionality_Vector2D_Angle);
	map.emplace("Vector2D_AngleWithVector",ScriptFunctionality_Vector2D_AngleWithVector);
	map.emplace("Vector2D_Create",ScriptFunctionality_Vector2D_Create);
	map.emplace("Vector2D_Normalize",ScriptFunctionality_Vector2D_Normalize);
	map.emplace("Vector2D_Rotate",ScriptFunctionality_Vector2D_Rotate);
	map.emplace("Vector2D_Add",ScriptFunctionality_Vector2D_Add);
	map.emplace("Vector2D_Subtract",ScriptFunctionality_Vector2D_Subtract);
	map.emplace("Vector2D_Dot",ScriptFunctionality_Vector2D_Dot);
	map.emplace("Vector2D_Cross",ScriptFunctionality_Vector2D_Cross);
	map.emplace("Vector2D_Multiply",ScriptFunctionality_Vector2D_Multiply);
	map.emplace("Vector2D_Up",ScriptFunctionality_Vector2D_Up);
	map.emplace("Vector2D_Left",ScriptFunctionality_Vector2D_Left);
	map.emplace("Vector2D_Right",ScriptFunctionality_Vector2D_Right);
	map.emplace("Vector2D_Down",ScriptFunctionality_Vector2D_Down);
	map.emplace("Vector2D_One",ScriptFunctionality_Vector2D_One);
	map.emplace("Vector2D_Zero",ScriptFunctionality_Vector2D_Zero);
	map.emplace("Text_Equals",ScriptFunctionality_Text_Equals);
	map.emplace("Text_Concatenate",ScriptFunctionality_Text_Concatenate);
	map.emplace("Text_Substring",ScriptFunctionality_Text_Substring);
	map.emplace("Text_Begining",ScriptFunctionality_Text_Begining);
	map.emplace("Text_End",ScriptFunctionality_Text_End);
	map.emplace("Text_Trim",ScriptFunctionality_Text_Trim);
	map.emplace("Text_TrimBlanks",ScriptFunctionality_Text_TrimBlanks);
	map.emplace("Text_GetLetter",ScriptFunctionality_Text_GetLetter);
	map.emplace("Text_Find",ScriptFunctionality_Text_Find);
	map.emplace("Text_ToString",ScriptFunctionality_Text_ToString);
	map.emplace("Text_LeadingZeros",ScriptFunctionality_Text_LeadingZeros);
	map.emplace("Text_RemoveDecimals",ScriptFunctionality_Text_RemoveDecimals);
	map.emplace("Attribute_Set",ScriptFunctionality_Attribute_Set);
	map.emplace("Attribute_Get",ScriptFunctionality_Attribute_Get);
	map.emplace("Attribute_SetLocal",ScriptFunctionality_Attribute_SetLocal);
	map.emplace("Attribute_GetLocal",ScriptFunctionality_Attribute_GetLocal);
	map.emplace("Attribute_SetGlobal",ScriptFunctionality_Attribute_SetGlobal);
	map.emplace("Attribute_GetGlobal",ScriptFunctionality_Attribute_GetGlobal);
	map.emplace("RealTime_Now",ScriptFunctionality_RealTime_Now);
	map.emplace("RealTime_WeekDay",ScriptFunctionality_RealTime_WeekDay);
	map.emplace("RealTime_ShortWeekDay",ScriptFunctionality_RealTime_ShortWeekDay);
	map.emplace("RealTime_Month",ScriptFunctionality_RealTime_Month);
	map.emplace("RealTime_ShortMonth",ScriptFunctionality_RealTime_ShortMonth);
	map.emplace("RealTime_DayOfWeekIndex",ScriptFunctionality_RealTime_DayOfWeekIndex);
	map.emplace("RealTime_MonthIndex",ScriptFunctionality_RealTime_MonthIndex);
	map.emplace("RealTime_MonthDay",ScriptFunctionality_RealTime_MonthDay);
	map.emplace("RealTime_Year",ScriptFunctionality_RealTime_Year);
	map.emplace("RealTime_Hours",ScriptFunctionality_RealTime_Hours);
	map.emplace("RealTime_Minutes",ScriptFunctionality_RealTime_Minutes);
	map.emplace("RealTime_Seconds",ScriptFunctionality_RealTime_Seconds);
	map.emplace("RealTime_Since",ScriptFunctionality_RealTime_Since);
	map.emplace("RealTime_HourTime",ScriptFunctionality_RealTime_HourTime);
	map.emplace("RealTime_PreciseHourTime",ScriptFunctionality_RealTime_PreciseHourTime);
	map.emplace("RealTime_TimeStamp",ScriptFunctionality_RealTime_TimeStamp);
	map.emplace("RealTime_Date",ScriptFunctionality_RealTime_Date);
	map.emplace("Camera_GetPosition",ScriptFunctionality_Camera_GetPosition);
	map.emplace("Camera_SetPosition",ScriptFunctionality_Camera_SetPosition);
	map.emplace("Camera_GetScale",ScriptFunctionality_Camera_GetScale);
	map.emplace("Camera_SetScale",ScriptFunctionality_Camera_SetScale);
	map.emplace("Random_UnitValue",ScriptFunctionality_Random_UnitValue);
	map.emplace("Random_NumberBetween",ScriptFunctionality_Random_NumberBetween);
	map.emplace("Random_Between",ScriptFunctionality_Random_Between);
	map.emplace("DegreesTo_Radians",ScriptFunctionality_DegreesTo_Radians);
	map.emplace("RadiansTo_Degrees",ScriptFunctionality_RadiansTo_Degrees);
	map.emplace("Random_AngleBetween",ScriptFunctionality_Random_AngleBetween);
	map.emplace("Random_UnitVector",ScriptFunctionality_Random_UnitVector);
	map.emplace("Random_ScaledVector",ScriptFunctionality_Random_ScaledVector);
	map.emplace("Random_Color",ScriptFunctionality_Random_Color);
	map.emplace("Random_ColorBetween",ScriptFunctionality_Random_ColorBetween);
	map.emplace("OpenURL",ScriptFunctionality_OpenURL);
	map.emplace("SoundManager_setMasterVolume",SoundManager_setMasterVolume);
	map.emplace("SoundManager_setChannelsCapacity",SoundManager_setChannelsCapacity);
	map.emplace("Time_GetTimeSinceBegining",Time_GetTimeSinceBegining);
	map.emplace("Time_GetTimeSinceBeginingMilliseconds",Time_GetTimeSinceBeginingMilliseconds);
	map.emplace("Time_GetDeltaTime",Time_GetDeltaTime);
	map.emplace("Time_GetFrameRate",Time_GetFrameRate);
	map.emplace("Time_GetPhysicsDeltaTime",Time_GetPhysicsDeltaTime);
	map.emplace("Time_ScaleWithDeltaTime",Time_ScaleWithDeltaTime);
	map.emplace("Time_ScaleWithPhysicsDeltaTime",Time_ScaleWithPhysicsDeltaTime);

};
Scripting::Variable Animation_ChangeAnimationPath(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[3].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", std::to_string(3), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[4].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", std::to_string(4), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[5].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", std::to_string(5), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_ChangeAnimationPath", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	self->ChangeAnimationPath(vec[1].str, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float, vec[5].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Animation_AdvanceFrame(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_AdvanceFrame", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_AdvanceFrame", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	self->AdvanceFrame();
	return Scripting::Variable::Null();
}
Scripting::Variable Animation_AdvanceAnimation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_AdvanceAnimation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_AdvanceAnimation", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	self->AdvanceAnimation();
	return Scripting::Variable::Null();
}
Scripting::Variable Animation_SetFrame(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetFrame", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetFrame", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetFrame", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	self->SetFrame(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Animation_SetAnimation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetAnimation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetAnimation", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetAnimation", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	self->SetAnimation(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Animation_GetFrame(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_GetFrame", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_GetFrame", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	int ret = self->GetFrame();
	return ret;
}
Scripting::Variable Animation_GetAnimation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_GetAnimation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_GetAnimation", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	int ret = self->GetAnimation();
	return ret;
}
Scripting::Variable Animation_SetAnimationLenght(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetAnimationLenght", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetAnimationLenght", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_SetAnimationLenght", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	self->SetAnimationLenght(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Animation_getAnimationLenght(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_getAnimationLenght", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Animation* self = vec[0].value.entity->getComponent<Animation>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Animation_getAnimationLenght", vec[0].value.entity->getEntityName(), Animation);
		return Scripting::Variable::Null();
	}
	int ret = self->getAnimationLenght();
	return ret;
}
Scripting::Variable BoxBody_getSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_getSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	BoxBody* self = vec[0].value.entity->getComponent<BoxBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_getSize", vec[0].value.entity->getEntityName(), BoxBody);
		return Scripting::Variable::Null();
	}
	Vector2D ret = self->getSize();
	return ret;
}
Scripting::Variable BoxBody_setSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setSize", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setSize", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	BoxBody* self = vec[0].value.entity->getComponent<BoxBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setSize", vec[0].value.entity->getEntityName(), BoxBody);
		return Scripting::Variable::Null();
	}
	self->setSize(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable BoxBody_setMass(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setMass", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setMass", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	BoxBody* self = vec[0].value.entity->getComponent<BoxBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_setMass", vec[0].value.entity->getEntityName(), BoxBody);
		return Scripting::Variable::Null();
	}
	self->setMass(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable BoxBody_getArea(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_getArea", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	BoxBody* self = vec[0].value.entity->getComponent<BoxBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "BoxBody_getArea", vec[0].value.entity->getEntityName(), BoxBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getArea();
	return ret;
}
Scripting::Variable Image_getTextureWidth(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_getTextureWidth", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_getTextureWidth", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	int ret = self->getTextureWidth();
	return ret;
}
Scripting::Variable Image_getTextureHeight(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_getTextureHeight", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_getTextureHeight", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	int ret = self->getTextureHeight();
	return ret;
}
Scripting::Variable Image_setSrcRect(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setSrcRect", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setSrcRect", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setSrcRect", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[3].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setSrcRect", std::to_string(3), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[4].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setSrcRect", std::to_string(4), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setSrcRect", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setSrcRect(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setRotaionPoint(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setRotaionPoint", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setRotaionPoint", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setRotaionPoint", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setRotaionPoint", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setRotaionPoint(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_scaledSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_scaledSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_scaledSize", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->scaledSize();
	return ret;
}
Scripting::Variable Image_loadTexture(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_loadTexture", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_loadTexture", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_loadTexture", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->loadTexture(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable Image_setFlipMode(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setFlipMode", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setFlipMode", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Image* self = vec[0].value.entity->getComponent<Image>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Image_setFlipMode", vec[0].value.entity->getEntityName(), Image);
		return Scripting::Variable::Null();
	}
	self->setFlipMode(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_loadMusic(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_loadMusic", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_loadMusic", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_loadMusic", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->loadMusic(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_play(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_play", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_play", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->play();
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_pause(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_pause", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_pause", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->pause();
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_stop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_stop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_stop", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->stop();
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_resume(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_resume", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_resume", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->resume();
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_playWithFadeIn(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_playWithFadeIn", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_playWithFadeIn", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_playWithFadeIn", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_playWithFadeIn", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->playWithFadeIn(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_fadeOut(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_fadeOut", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_fadeOut", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_fadeOut", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->fadeOut(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_isPlaying(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_isPlaying", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_isPlaying", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isPlaying();
	return ret;
}
Scripting::Variable MusicEmitter_isPaused(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_isPaused", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_isPaused", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isPaused();
	return ret;
}
Scripting::Variable MusicEmitter_rewind(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_rewind", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_rewind", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->rewind();
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_setVolume(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_setVolume", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_setVolume", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_setVolume", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->setVolume(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_getVolume(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_getVolume", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_getVolume", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	float ret = self->getVolume();
	return ret;
}
Scripting::Variable MusicEmitter_shouldPlayOnStart(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_shouldPlayOnStart", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_shouldPlayOnStart", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_shouldPlayOnStart", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->shouldPlayOnStart(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_setLoop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_setLoop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_setLoop", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_setLoop", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	self->setLoop(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable MusicEmitter_isOnLoop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_isOnLoop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	MusicEmitter* self = vec[0].value.entity->getComponent<MusicEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "MusicEmitter_isOnLoop", vec[0].value.entity->getEntityName(), MusicEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isOnLoop();
	return ret;
}
Scripting::Variable Overlay_GetPlacement(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetPlacement", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetPlacement", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetPlacement();
	return ret;
}
Scripting::Variable Overlay_SetPositioned(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetPositioned", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetPositioned", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetPositioned", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetPositioned", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetPositioned(vec[1].vector, vec[2].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetStreched(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetStreched", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetStreched", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetStreched", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[3].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetStreched", std::to_string(3), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[4].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetStreched", std::to_string(4), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetStreched", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetStreched(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_Move(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_Move", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_Move", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_Move", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->Move(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_MoveTo(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_MoveTo", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_MoveTo", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_MoveTo", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->MoveTo(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetSize", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetSize", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetSize(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetPosition(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetPosition", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetPosition", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetPosition();
	return ret;
}
Scripting::Variable Overlay_GetSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetSize", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetSize();
	return ret;
}
Scripting::Variable Overlay_GetTop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetTop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetTop", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetTop();
	return ret;
}
Scripting::Variable Overlay_GetLeft(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetLeft", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetLeft();
	return ret;
}
Scripting::Variable Overlay_GetRight(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetRight", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetRight();
	return ret;
}
Scripting::Variable Overlay_GetBottom(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetBottom", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetBottom", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	int ret = self->GetBottom();
	return ret;
}
Scripting::Variable Overlay_SetTop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetTop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetTop", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetTop", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetTop(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetLeft(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetLeft", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetLeft", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetLeft(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetRight(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetRight", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetRight", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetRight(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetBottom(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetBottom", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetBottom", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetBottom", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetBottom(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetAnchor(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetAnchor", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetAnchor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetAnchor();
	return ret;
}
Scripting::Variable Overlay_SetAnchor(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchor", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchor", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchor(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorCenter(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorCenter", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorCenter", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorCenter();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorTopLeft(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorTopLeft", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorTopLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorTopLeft();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorTopRight(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorTopRight", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorTopRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorTopRight();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorBottomLeft(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorBottomLeft", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorBottomLeft", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorBottomLeft();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetAnchorBottomRight(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorBottomRight", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetAnchorBottomRight", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetAnchorBottomRight();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetColor(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetColor", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetColor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	Utilities::Color ret = self->GetColor();
	return ret;
}
Scripting::Variable Overlay_SetColor(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetColor", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Color){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetColor", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetColor", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetColor(vec[1].value.color);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_GetRenderScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetRenderScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_GetRenderScale", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	float ret = self->GetRenderScale();
	return ret;
}
Scripting::Variable Overlay_SetRenderScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetRenderScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetRenderScale", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetRenderScale", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetRenderScale(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_ResetRenderScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_ResetRenderScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_ResetRenderScale", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->ResetRenderScale();
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_SetInteractable(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetInteractable", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetInteractable", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_SetInteractable", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	self->SetInteractable(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable Overlay_IsInteractable(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_IsInteractable", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Overlay* self = vec[0].value.entity->getComponent<Overlay>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Overlay_IsInteractable", vec[0].value.entity->getEntityName(), Overlay);
		return Scripting::Variable::Null();
	}
	bool ret = self->IsInteractable();
	return ret;
}
Scripting::Variable OverlayImage_SetTexture(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayImage_SetTexture", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayImage_SetTexture", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayImage* self = vec[0].value.entity->getComponent<OverlayImage>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayImage_SetTexture", vec[0].value.entity->getEntityName(), OverlayImage);
		return Scripting::Variable::Null();
	}
	self->SetTexture(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayImage_GetTexture(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayImage_GetTexture", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayImage* self = vec[0].value.entity->getComponent<OverlayImage>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayImage_GetTexture", vec[0].value.entity->getEntityName(), OverlayImage);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetTexture();
	return ret;
}
Scripting::Variable OverlayText_GetFit(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetFit", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetFit", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetFit();
	return ret;
}
Scripting::Variable OverlayText_SetFit(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetFit", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetFit", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetFit", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetFit(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetVerticalAlignment(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetVerticalAlignment", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetVerticalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetVerticalAlignment();
	return ret;
}
Scripting::Variable OverlayText_GetHorizontalAlignment(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetHorizontalAlignment", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetHorizontalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetHorizontalAlignment();
	return ret;
}
Scripting::Variable OverlayText_SetVerticalAlignment(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetVerticalAlignment", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetVerticalAlignment", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetVerticalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetVerticalAlignment(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_SetHorizontalAlignment(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetHorizontalAlignment", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetHorizontalAlignment", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetHorizontalAlignment", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetHorizontalAlignment(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetFont(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetFont", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetFont", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetFont();
	return ret;
}
Scripting::Variable OverlayText_SetFont(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetFont", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetFont", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetFont", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetFont(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetText(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetText", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetText", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	std::string ret = self->GetText();
	return ret;
}
Scripting::Variable OverlayText_SetText(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetText", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetText", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetText", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetText(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable OverlayText_GetPointSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetPointSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_GetPointSize", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	int ret = self->GetPointSize();
	return ret;
}
Scripting::Variable OverlayText_SetPointSize(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetPointSize", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetPointSize", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	OverlayText* self = vec[0].value.entity->getComponent<OverlayText>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "OverlayText_SetPointSize", vec[0].value.entity->getEntityName(), OverlayText);
		return Scripting::Variable::Null();
	}
	self->SetPointSize(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable ParticleSystem_startEmitting(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_startEmitting", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	ParticleSystem* self = vec[0].value.entity->getComponent<ParticleSystem>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_startEmitting", vec[0].value.entity->getEntityName(), ParticleSystem);
		return Scripting::Variable::Null();
	}
	self->startEmitting();
	return Scripting::Variable::Null();
}
Scripting::Variable ParticleSystem_isEmitting(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_isEmitting", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	ParticleSystem* self = vec[0].value.entity->getComponent<ParticleSystem>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_isEmitting", vec[0].value.entity->getEntityName(), ParticleSystem);
		return Scripting::Variable::Null();
	}
	bool ret = self->isEmitting();
	return ret;
}
Scripting::Variable ParticleSystem_loadTexture(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_loadTexture", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_loadTexture", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	ParticleSystem* self = vec[0].value.entity->getComponent<ParticleSystem>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_loadTexture", vec[0].value.entity->getEntityName(), ParticleSystem);
		return Scripting::Variable::Null();
	}
	self->loadTexture(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable ParticleSystem_addBurst(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[3].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", std::to_string(3), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[4].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", std::to_string(4), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[5].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", std::to_string(5), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	ParticleSystem* self = vec[0].value.entity->getComponent<ParticleSystem>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "ParticleSystem_addBurst", vec[0].value.entity->getEntityName(), ParticleSystem);
		return Scripting::Variable::Null();
	}
	self->addBurst(vec[1].value.Float, vec[2].value.Float, vec[3].value.Float, vec[4].value.Float, vec[5].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_setTrigger(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setTrigger", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setTrigger", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setTrigger", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setTrigger(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_isTrigger(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_isTrigger", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_isTrigger", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	bool ret = self->isTrigger();
	return ret;
}
Scripting::Variable PhysicBody_setFriction(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setFriction", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setFriction", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setFriction", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setFriction(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getFriction(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getFriction", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getFriction", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getFriction();
	return ret;
}
Scripting::Variable PhysicBody_setBounciness(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setBounciness", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setBounciness", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setBounciness", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setBounciness(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getBounciness(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getBounciness", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getBounciness", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getBounciness();
	return ret;
}
Scripting::Variable PhysicBody_getOffSet(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getOffSet", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getOffSet", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	Vector2D ret = self->getOffSet();
	return ret;
}
Scripting::Variable PhysicBody_addOffSet(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_addOffSet", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_addOffSet", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_addOffSet", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_addOffSet", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->addOffSet(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_setRotationFreezed(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setRotationFreezed", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setRotationFreezed", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setRotationFreezed", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setRotationFreezed(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_isRotationFreezed(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_isRotationFreezed", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_isRotationFreezed", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	bool ret = self->isRotationFreezed();
	return ret;
}
Scripting::Variable PhysicBody_setBodyType(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setBodyType", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setBodyType", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setBodyType", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setBodyType(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getBodyType(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getBodyType", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getBodyType", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	int ret = self->getBodyType();
	return ret;
}
Scripting::Variable PhysicBody_setLinearDrag(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearDrag", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearDrag", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearDrag", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setLinearDrag(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getLinearDrag(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getLinearDrag", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getLinearDrag", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getLinearDrag();
	return ret;
}
Scripting::Variable PhysicBody_setAngularDrag(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setAngularDrag", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setAngularDrag", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setAngularDrag", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setAngularDrag(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getAngularDrag(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getAngularDrag", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getAngularDrag", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getAngularDrag();
	return ret;
}
Scripting::Variable PhysicBody_setGravityScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setGravityScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setGravityScale", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setGravityScale", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setGravityScale(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getGravityScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getGravityScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getGravityScale", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getGravityScale();
	return ret;
}
Scripting::Variable PhysicBody_getCollisionNormal(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getCollisionNormal", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getCollisionNormal", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	Vector2D ret = self->getCollisionNormal();
	return ret;
}
Scripting::Variable PhysicBody_getCollisionPoint(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getCollisionPoint", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getCollisionPoint", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	Vector2D ret = self->getCollisionPoint();
	return ret;
}
Scripting::Variable PhysicBody_setCollisionLayer(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setCollisionLayer", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setCollisionLayer", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setCollisionLayer", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setCollisionLayer(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getCollisionLayer(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getCollisionLayer", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getCollisionLayer", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	cstring ret = self->getCollisionLayer();
	return ret;
}
Scripting::Variable PhysicBody_setLinearVelocity(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearVelocity", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearVelocity", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearVelocity", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setLinearVelocity", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setLinearVelocity(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getLinearVelocity(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getLinearVelocity", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getLinearVelocity", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	Vector2D ret = self->getLinearVelocity();
	return ret;
}
Scripting::Variable PhysicBody_setAngularVelocity(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setAngularVelocity", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setAngularVelocity", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_setAngularVelocity", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->setAngularVelocity(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_getAngularVelocity(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getAngularVelocity", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_getAngularVelocity", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	float ret = self->getAngularVelocity();
	return ret;
}
Scripting::Variable PhysicBody_applyForce(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForce", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForce", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForce", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForce", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->applyForce(vec[1].vector, vec[2].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_applyForceToCenter(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForceToCenter", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForceToCenter", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyForceToCenter", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->applyForceToCenter(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_applyTorque(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyTorque", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyTorque", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyTorque", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->applyTorque(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_applyLinearImpulse(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulse", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulse", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulse", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulse", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->applyLinearImpulse(vec[1].vector, vec[2].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_applyLinearImpulseToCenter(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulseToCenter", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulseToCenter", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyLinearImpulseToCenter", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->applyLinearImpulseToCenter(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicBody_applyAngularImpulse(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyAngularImpulse", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyAngularImpulse", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	PhysicBody* self = vec[0].value.entity->getComponent<PhysicBody>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "PhysicBody_applyAngularImpulse", vec[0].value.entity->getEntityName(), PhysicBody);
		return Scripting::Variable::Null();
	}
	self->applyAngularImpulse(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_loadSound(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_loadSound", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::String){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_loadSound", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_loadSound", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->loadSound(vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_play(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_play", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_play", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->play();
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_pause(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_pause", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_pause", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->pause();
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_stop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_stop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_stop", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->stop();
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_resume(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_resume", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_resume", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->resume();
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_playWithfadeIn(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_playWithfadeIn", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_playWithfadeIn", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[2].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_playWithfadeIn", std::to_string(2), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_playWithfadeIn", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->playWithfadeIn(vec[1].value.Float, vec[2].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_fadeOut(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_fadeOut", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_fadeOut", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_fadeOut", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->fadeOut(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_isPlaying(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isPlaying", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isPlaying", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isPlaying();
	return ret;
}
Scripting::Variable SoundEmitter_isPaused(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isPaused", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isPaused", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isPaused();
	return ret;
}
Scripting::Variable SoundEmitter_setVolume(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setVolume", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setVolume", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setVolume", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->setVolume(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_getVolume(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_getVolume", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_getVolume", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	int ret = self->getVolume();
	return ret;
}
Scripting::Variable SoundEmitter_shouldPlayOnStart(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_shouldPlayOnStart", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_shouldPlayOnStart", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_shouldPlayOnStart", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->shouldPlayOnStart(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_setLoop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setLoop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setLoop", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setLoop", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->setLoop(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_isOnLoop(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isOnLoop", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isOnLoop", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isOnLoop();
	return ret;
}
Scripting::Variable SoundEmitter_enableSpatialSound(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_enableSpatialSound", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Bool){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_enableSpatialSound", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_enableSpatialSound", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->enableSpatialSound(vec[1].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_isSpatialSoundEnabled(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isSpatialSoundEnabled", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_isSpatialSoundEnabled", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	bool ret = self->isSpatialSoundEnabled();
	return ret;
}
Scripting::Variable SoundEmitter_setPanning(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setPanning", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setPanning", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setPanning", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->setPanning(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_getPanning(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_getPanning", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_getPanning", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	float ret = self->getPanning();
	return ret;
}
Scripting::Variable SoundEmitter_setAudibleDistance(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setAudibleDistance", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setAudibleDistance", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_setAudibleDistance", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	self->setAudibleDistance(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundEmitter_getAudibleDistance(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_getAudibleDistance", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	SoundEmitter* self = vec[0].value.entity->getComponent<SoundEmitter>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "SoundEmitter_getAudibleDistance", vec[0].value.entity->getEntityName(), SoundEmitter);
		return Scripting::Variable::Null();
	}
	float ret = self->getAudibleDistance();
	return ret;
}
Scripting::Variable Transform_GetLocalPosition(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetLocalPosition", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetLocalPosition", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetLocalPosition();
	return ret;
}
Scripting::Variable Transform_GetWorldPosition(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetWorldPosition", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetWorldPosition", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetWorldPosition();
	return ret;
}
Scripting::Variable Transform_SetLocalPosition(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPosition", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPosition", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPosition", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetLocalPosition(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetLocalPositionX(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPositionX", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPositionX", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPositionX", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetLocalPositionX(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetLocalPositionY(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPositionY", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPositionY", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalPositionY", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetLocalPositionY(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetWorldPosition(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldPosition", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldPosition", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldPosition", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetWorldPosition(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_GetLocalScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetLocalScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetLocalScale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetLocalScale();
	return ret;
}
Scripting::Variable Transform_GetWorldScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetWorldScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetWorldScale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	Utilities::Vector2D ret = self->GetWorldScale();
	return ret;
}
Scripting::Variable Transform_SetScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScale", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetScale(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetScaleX(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScaleX", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScaleX", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScaleX", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetScaleX(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetScaleY(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScaleY", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScaleY", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetScaleY", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetScaleY(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetWorldScale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldScale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldScale", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldScale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetWorldScale(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_GetLocalRotation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetLocalRotation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetLocalRotation", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	float ret = self->GetLocalRotation();
	return ret;
}
Scripting::Variable Transform_GetWorldRotation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetWorldRotation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_GetWorldRotation", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	float ret = self->GetWorldRotation();
	return ret;
}
Scripting::Variable Transform_SetLocalRotation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalRotation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalRotation", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetLocalRotation", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetLocalRotation(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetWorldRotation(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldRotation", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldRotation", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetWorldRotation", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetWorldRotation(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_Translate(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Translate", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Vector2D){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Translate", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Translate", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->Translate(vec[1].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_TranslateX(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_TranslateX", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_TranslateX", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_TranslateX", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->TranslateX(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_TranslateY(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_TranslateY", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_TranslateY", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_TranslateY", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->TranslateY(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_Rotate(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Rotate", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Rotate", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Rotate", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->Rotate(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_Scale(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Scale", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	if(vec[1].type != Scripting::Variable::Type::Float){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Scale", std::to_string(1), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_Scale", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->Scale(vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Transform_SetTransformRelativeToNewParent(std::vector<Scripting::Variable>const& vec){

	if(vec[0].type != Scripting::Variable::Type::Entity){
		DebugInvalidInputError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetTransformRelativeToNewParent", std::to_string(0), std::string(""),  ""); 
		return Scripting::Variable::Null();
	}

	Transform* self = vec[0].value.entity->getComponent<Transform>();
	if(self == nullptr){
		DebugComponentError(ScriptFunctionality_Entity_CurrentName({}).str, ScriptFunctionality_Graph({}).str, "Transform_SetTransformRelativeToNewParent", vec[0].value.entity->getEntityName(), Transform);
		return Scripting::Variable::Null();
	}
	self->SetTransformRelativeToNewParent();
	return Scripting::Variable::Null();
}
Scripting::Variable InputManager_AnyKeyPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyKeyPressed();
	return ret;
}
Scripting::Variable InputManager_AnyKeyReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyKeyReleased();
	return ret;
}
Scripting::Variable InputManager_IsLetterPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsLetterPressed(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsLetterDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsLetterDown(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsLetterUp(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsLetterUp(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsLetterReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsLetterReleased(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsNumberPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsNumberPressed(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsNumberDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsNumberDown(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsNumberUp(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsNumberUp(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsNumberReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsNumberReleased(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsSpecialKeyPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsSpecialKeyPressed(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsSpecialKeyDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsSpecialKeyDown(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsSpecialKeyUp(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsSpecialKeyUp(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsSpecialKeyReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsSpecialKeyReleased(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_HorizontalMovement(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->HorizontalMovement();
	return ret;
}
Scripting::Variable InputManager_VerticalMovement(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->VerticalMovement();
	return ret;
}
Scripting::Variable InputManager_Jump(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->Jump();
	return ret;
}
Scripting::Variable InputManager_Action(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->Action();
	return ret;
}
Scripting::Variable InputManager_HasMouseMoved(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->HasMouseMoved();
	return ret;
}
Scripting::Variable InputManager_HasMouseWheelMoved(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->HasMouseWheelMoved();
	return ret;
}
Scripting::Variable InputManager_IsMouseButtonDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsMouseButtonDown(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsMouseButtonUp(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsMouseButtonUp(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsMouseButtonPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsMouseButtonPressed(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsMouseButtonReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsMouseButtonReleased(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetMousePosition(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	cVector2D ret = manager->GetMousePosition();
	return ret;
}
Scripting::Variable InputManager_GetMouseWheelScroll(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	int ret = manager->GetMouseWheelScroll();
	return ret;
}
Scripting::Variable InputManager_ConnectedControllersCount(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	int ret = manager->ConnectedControllersCount();
	return ret;
}
Scripting::Variable InputManager_AnyControllerButtonPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyControllerButtonPressed();
	return ret;
}
Scripting::Variable InputManager_AnyControllerButtonReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyControllerButtonReleased();
	return ret;
}
Scripting::Variable InputManager_AnyControllerAxisMotion(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyControllerAxisMotion();
	return ret;
}
Scripting::Variable InputManager_AnyControllerConnected(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyControllerConnected();
	return ret;
}
Scripting::Variable InputManager_AnyControllerDisconnected(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->AnyControllerDisconnected();
	return ret;
}
Scripting::Variable InputManager_IsControllerButtonPressedWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsControllerButtonPressedWithId(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsControllerButtonDownWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsControllerButtonDownWithId(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsControllerButtonReleasedWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsControllerButtonReleasedWithId(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetLeftTriggerValueWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetLeftTriggerValueWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetRightTriggerValueWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetRightTriggerValueWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsLeftTriggerDownWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsLeftTriggerDownWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsRightTriggerDownWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsRightTriggerDownWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetLeftStickHorizontalValueWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetLeftStickHorizontalValueWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetLeftStickVerticalValueWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetLeftStickVerticalValueWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetRightStickHorizontalValueWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetRightStickHorizontalValueWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetRightStickVerticalValueWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetRightStickVerticalValueWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_HasLeftStickMovedWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->HasLeftStickMovedWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_HasRightStickMovedWithId(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->HasRightStickMovedWithId(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsControllerButtonPressed(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsControllerButtonPressed(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsControllerButtonDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsControllerButtonDown(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_IsControllerButtonReleased(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsControllerButtonReleased(vec[0].value.Float);
	return ret;
}
Scripting::Variable InputManager_GetLeftTriggerValue(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetLeftTriggerValue();
	return ret;
}
Scripting::Variable InputManager_GetRightTriggerValue(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetRightTriggerValue();
	return ret;
}
Scripting::Variable InputManager_IsLeftTriggerDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsLeftTriggerDown();
	return ret;
}
Scripting::Variable InputManager_IsRightTriggerDown(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->IsRightTriggerDown();
	return ret;
}
Scripting::Variable InputManager_GetLeftStickHorizontalValue(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetLeftStickHorizontalValue();
	return ret;
}
Scripting::Variable InputManager_GetLeftStickVerticalValue(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetLeftStickVerticalValue();
	return ret;
}
Scripting::Variable InputManager_GetRightStickHorizontalValue(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetRightStickHorizontalValue();
	return ret;
}
Scripting::Variable InputManager_GetRightStickVerticalValue(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	float ret = manager->GetRightStickVerticalValue();
	return ret;
}
Scripting::Variable InputManager_HasLeftStickMoved(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->HasLeftStickMoved();
	return ret;
}
Scripting::Variable InputManager_HasRightStickMoved(std::vector<Scripting::Variable>const& vec){
	InputManager* manager = InputManager::instance();
	bool ret = manager->HasRightStickMoved();
	return ret;
}
Scripting::Variable PhysicsManager_getGravity(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	Utilities::Vector2D ret = manager->getGravity();
	return ret;
}
Scripting::Variable PhysicsManager_setGravity(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->setGravity(vec[0].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsManager_addCollisionLayer(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->addCollisionLayer(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsManager_removeCollisionLayer(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->removeCollisionLayer(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsManager_setCollisionBetweenLayers(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	manager->setCollisionBetweenLayers(vec[0].str, vec[1].str, vec[2].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable PhysicsManager_layersCollide(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	bool ret = manager->layersCollide(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable PhysicsManager_layersExists(std::vector<Scripting::Variable>const& vec){
	PhysicsManager* manager = PhysicsManager::instance();
	bool ret = manager->layersExists(vec[0].str);
	return ret;
}
Scripting::Variable RendererManager_getWidth(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	int ret = manager->getWidth();
	return ret;
}
Scripting::Variable RendererManager_getHeight(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	int ret = manager->getHeight();
	return ret;
}
Scripting::Variable RendererManager_toggleFullScreen(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	manager->toggleFullScreen();
	return Scripting::Variable::Null();
}
Scripting::Variable RendererManager_showCursor(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	manager->showCursor(vec[0].value.Bool);
	return Scripting::Variable::Null();
}
Scripting::Variable RendererManager_resizeWindow(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	manager->resizeWindow(vec[0].value.Float, vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable RendererManager_renameWindow(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	manager->renameWindow(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable RendererManager_repositionWindow(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	manager->repositionWindow(vec[0].value.Float, vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable RendererManager_SetWindowIcon(std::vector<Scripting::Variable>const& vec){
	RendererManager* manager = RendererManager::instance();
	bool ret = manager->SetWindowIcon(vec[0].str);
	return ret;
}
Scripting::Variable SaveManager_SaveAll(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->SaveAll();
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Save(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->Save(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Load(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->Load(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_SetSlot(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->SetSlot(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_GetSlot(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	Scripting::Variable ret = manager->GetSlot(vec[0].str);
	return ret;
}
Scripting::Variable SaveManager_Set(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->Set(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Get(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	Scripting::Variable ret = manager->Get(vec[0].str);
	return ret;
}
Scripting::Variable SaveManager_ClearSlot(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->ClearSlot();
	return Scripting::Variable::Null();
}
Scripting::Variable SaveManager_Exists(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	bool ret = manager->Exists(vec[0].value.Float);
	return ret;
}
Scripting::Variable SaveManager_DeleteSave(std::vector<Scripting::Variable>const& vec){
	SaveManager* manager = SaveManager::instance();
	manager->DeleteSave(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_ChangeScene(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->ChangeScene(vec[0].str, vec[1].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_ResetScene(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->ResetScene();
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_EndGame(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	manager->EndGame();
	return Scripting::Variable::Null();
}
Scripting::Variable SceneManager_getNumberOfScenes(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	int ret = manager->getNumberOfScenes();
	return ret;
}
Scripting::Variable SceneManager_GetCurrentScenePath(std::vector<Scripting::Variable>const& vec){
	SceneManager* manager = SceneManager::instance();
	std::string ret = manager->GetCurrentScenePath();
	return ret;
}
Scripting::Variable ScriptFunctionality_Print(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Print(vec[0]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Entity(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	ECS::Entity* ret = manager->Entity();
	return ret;
}
Scripting::Variable ScriptFunctionality_Create_EntityWithTransform(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Create_EntityWithTransform(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Create_EntityWithOverlay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Create_EntityWithOverlay(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Destroy_Entity(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Destroy_Entity(vec[0].value.entity);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Entity_Name(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Entity_Name(vec[0].value.entity);
	return ret;
}
Scripting::Variable ScriptFunctionality_Entity_CurrentName(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Entity_CurrentName();
	return ret;
}
Scripting::Variable ScriptFunctionality_Event_EntityEvent(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Event_EntityEvent(vec[0].value.entity, vec[1].str);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Event_GlobalEvent(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Event_GlobalEvent(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Graph(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Graph();
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Add(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Add(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Subtract(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Subtract(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Multiply(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Multiply(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Divide(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Divide(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Power(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Power(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Root(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Root(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Max(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Max(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_Min(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_Min(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_PlusOne(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_PlusOne(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Math_MinusOne(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Math_MinusOne(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Equals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Equals(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_NotEquals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_NotEquals(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Lesser(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Lesser(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Greater(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Greater(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_LesserOrEqual(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_LesserOrEqual(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_GreaterOrEqual(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_GreaterOrEqual(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_And(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_And(vec[0].value.Bool, vec[1].value.Bool);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Or(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Or(vec[0].value.Bool, vec[1].value.Bool);
	return ret;
}
Scripting::Variable ScriptFunctionality_Logic_Negate(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Logic_Negate(vec[0].value.Bool);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Magnitude(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_Magnitude(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_X(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_X(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Y(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_Y(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Angle(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_Angle(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_AngleWithVector(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Vector2D_AngleWithVector(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Create(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Create(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Normalize(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Normalize(vec[0].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Rotate(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Rotate(vec[0].vector, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Add(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Add(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Subtract(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Subtract(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Dot(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Dot(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Cross(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Cross(vec[0].vector, vec[1].vector);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Multiply(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Multiply(vec[0].vector, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Up(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Up();
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Left(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Left();
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Right(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Right();
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Down(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Down();
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_One(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_One();
	return ret;
}
Scripting::Variable ScriptFunctionality_Vector2D_Zero(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Vector2D_Zero();
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_Equals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	bool ret = manager->Text_Equals(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_Concatenate(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_Concatenate(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_Substring(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_Substring(vec[0].str, vec[1].value.Float, vec[2].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_Begining(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_Begining(vec[0].str, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_End(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_End(vec[0].str, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_Trim(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_Trim(vec[0].str, vec[1].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_TrimBlanks(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_TrimBlanks(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_GetLetter(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	char ret = manager->Text_GetLetter(vec[0].str, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_Find(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->Text_Find(vec[0].str, vec[1].value.Char);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_ToString(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_ToString(vec[0]);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_LeadingZeros(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_LeadingZeros(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Text_RemoveDecimals(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->Text_RemoveDecimals(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Attribute_Set(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Attribute_Set(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Attribute_Get(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Scripting::Variable ret = manager->Attribute_Get(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Attribute_SetLocal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Attribute_SetLocal(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Attribute_GetLocal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Scripting::Variable ret = manager->Attribute_GetLocal(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_Attribute_SetGlobal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Attribute_SetGlobal(vec[0].str, vec[1]);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Attribute_GetGlobal(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Scripting::Variable ret = manager->Attribute_GetGlobal(vec[0].str);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Now(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_Now();
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_WeekDay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_WeekDay(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_ShortWeekDay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_ShortWeekDay(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Month(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_Month(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_ShortMonth(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_ShortMonth(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_DayOfWeekIndex(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_DayOfWeekIndex(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_MonthIndex(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_MonthIndex(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_MonthDay(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_MonthDay(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Year(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_Year(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Hours(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_Hours(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Minutes(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_Minutes(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Seconds(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	int ret = manager->RealTime_Seconds(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Since(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_Since(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_HourTime(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_HourTime(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_PreciseHourTime(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_PreciseHourTime(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_TimeStamp(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_TimeStamp(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RealTime_Date(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	std::string ret = manager->RealTime_Date(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Camera_GetPosition(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Utilities::Vector2D ret = manager->Camera_GetPosition();
	return ret;
}
Scripting::Variable ScriptFunctionality_Camera_SetPosition(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Camera_SetPosition(vec[0].vector);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Camera_GetScale(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Camera_GetScale();
	return ret;
}
Scripting::Variable ScriptFunctionality_Camera_SetScale(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->Camera_SetScale(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable ScriptFunctionality_Random_UnitValue(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Random_UnitValue();
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_NumberBetween(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Random_NumberBetween(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_Between(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Random_Between(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_DegreesTo_Radians(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->DegreesTo_Radians(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_RadiansTo_Degrees(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->RadiansTo_Degrees(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_AngleBetween(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	float ret = manager->Random_AngleBetween(vec[0].value.Float, vec[1].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_UnitVector(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Random_UnitVector();
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_ScaledVector(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Vector2D ret = manager->Random_ScaledVector(vec[0].value.Float);
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_Color(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Color ret = manager->Random_Color();
	return ret;
}
Scripting::Variable ScriptFunctionality_Random_ColorBetween(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	Color ret = manager->Random_ColorBetween(vec[0].value.color, vec[1].value.color);
	return ret;
}
Scripting::Variable ScriptFunctionality_OpenURL(std::vector<Scripting::Variable>const& vec){
	ScriptFunctionality* manager = ScriptFunctionality::instance();
	manager->OpenURL(vec[0].str);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundManager_setMasterVolume(std::vector<Scripting::Variable>const& vec){
	SoundManager* manager = SoundManager::instance();
	manager->setMasterVolume(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable SoundManager_setChannelsCapacity(std::vector<Scripting::Variable>const& vec){
	SoundManager* manager = SoundManager::instance();
	manager->setChannelsCapacity(vec[0].value.Float);
	return Scripting::Variable::Null();
}
Scripting::Variable Time_GetTimeSinceBegining(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetTimeSinceBegining();
	return ret;
}
Scripting::Variable Time_GetTimeSinceBeginingMilliseconds(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetTimeSinceBeginingMilliseconds();
	return ret;
}
Scripting::Variable Time_GetDeltaTime(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetDeltaTime();
	return ret;
}
Scripting::Variable Time_GetFrameRate(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetFrameRate();
	return ret;
}
Scripting::Variable Time_GetPhysicsDeltaTime(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->GetPhysicsDeltaTime();
	return ret;
}
Scripting::Variable Time_ScaleWithDeltaTime(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->ScaleWithDeltaTime(vec[0].value.Float);
	return ret;
}
Scripting::Variable Time_ScaleWithPhysicsDeltaTime(std::vector<Scripting::Variable>const& vec){
	Time* manager = Time::instance();
	float ret = manager->ScaleWithPhysicsDeltaTime(vec[0].value.Float);
	return ret;
}
