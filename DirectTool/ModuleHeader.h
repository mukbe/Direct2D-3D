#pragma once
//Manager
#include "./Systems/Keyboard.h"
#include "./Systems/Mouse.h"
#include "./Systems/Time.h"
#include "./Systems/SoundManager.h"
#include "./Utilities/Matrix2D.h"
#include "./Utilities/Matrix2x2.h"


#include "./Render/BufferManager.h"
#include "./Render/VertexLayout.h"
#include "./Render/States.h"
#include "./Render/ShaderBuffer.h"
#include "./Render/Shader.h"
#include "./Render/ShaderManager.h"
#include "./Render/Texture.h"

#include "./View/CameraManager.h"


#include "./Utilities/Math.h"
#include "./Utilities/Json.h"
#include "./Utilities/String.h"
#include "./Utilities/Logging.h"
#include "./Utilities/Path.h"
#include "./Utilities/Buffer.h"

#define KeyCode Keyboard::Get()
#define GameMouse Mouse::Get()
#define DeltaTime Time::Delta()

